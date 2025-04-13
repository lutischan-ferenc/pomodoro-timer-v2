#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mmsystem.h>

// Timer settings
typedef struct {
    int pomodoro_duration;
    int short_break_duration;
    int long_break_duration;
    int enable_clock_sound;
} TimerSettings;

TimerSettings settings = {25, 5, 15, 1};
int pomodoro_count = 0;
int is_running = 0;
int tick_sound = 1;
int is_in_pomodoro = 0;
int remaining_seconds = 0;
NOTIFYICONDATA nid = {0};
HANDLE timer_thread_handle = NULL;
int autostart_enabled = 0;
static HICON last_icon = NULL;
static wchar_t last_text[16] = {0};
static int last_dots = -1;
static int screenWidth = 0, screenHeight = 0;

// Resource IDs
#define IDD_SETTINGS 100
#define IDD_ABOUT 101
#define IDC_POMODORO 101
#define IDC_SHORT_BREAK 102
#define IDC_LONG_BREAK 103
#define IDC_OK 104
#define IDC_CANCEL 105
#define IDC_CLOCK_SOUND 106
#define IDC_AUTOSTART 107
#define IDC_WEBSITE 108

// Forward declarations
void load_settings();
void save_settings();
void update_tray_icon(HWND hwnd, const wchar_t* text, int dots, int seconds);
void play_tick_sound();
int is_autostart_enabled();
void set_autostart(int enable);
INT_PTR CALLBACK SettingsDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ShowSettingsDialog(HWND hwndParent);
void ShowAboutDialog(HWND hwndParent);
INT_PTR CALLBACK AboutDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

void init_system_metrics() {
    if (!screenWidth) screenWidth = GetSystemMetrics(SM_CXSCREEN);
    if (!screenHeight) screenHeight = GetSystemMetrics(SM_CYSCREEN);
}

// Generate dynamic tray icon
HICON create_tray_icon(const wchar_t* text, int dots) {
    if (last_icon && wcscmp(text, last_text) == 0 && dots == last_dots) {
        return last_icon;
    }

    if (last_icon) {
        DestroyIcon(last_icon);
        last_icon = NULL;
    }

    HDC hdc = CreateCompatibleDC(NULL);
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = 32;
    bmi.bmiHeader.biHeight = -32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* bits;
    HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
    SelectObject(hdc, hBitmap);

    HBRUSH bgBrush = CreateSolidBrush(RGB(139, 0, 0));
    RECT rect = {0, 0, 32, 32};
    FillRect(hdc, &rect, bgBrush);
    DeleteObject(bgBrush);

    HFONT hFont = CreateFontW(27, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                             OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_SWISS, L"Arial");
    SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    SIZE textSize;
    GetTextExtentPoint32W(hdc, text, wcslen(text), &textSize);
    int textX = (32 - textSize.cx) / 2;
    int textY = (32 - textSize.cy) / 2 - 2;
    TextOutW(hdc, textX, textY, text, wcslen(text));

    HBRUSH dotBrush = CreateSolidBrush(RGB(144, 238, 144));
    for (int i = 0; i < dots; i++) {
        int dotX = 4 + i * 8;
        int dotY = 30;
        Ellipse(hdc, dotX - 3, dotY - 3, dotX + 3, dotY + 3);
    }
    DeleteObject(dotBrush);
    DeleteObject(hFont);

    HBITMAP hMask = CreateCompatibleBitmap(hdc, 32, 32);
    HDC hdcMask = CreateCompatibleDC(hdc);
    SelectObject(hdcMask, hMask);
    SetBkColor(hdc, RGB(139, 0, 0));
    BitBlt(hdcMask, 0, 0, 32, 32, hdc, 0, 0, SRCCOPY);

    ICONINFO iconInfo;
    memset(&iconInfo, 0, sizeof(iconInfo));
    iconInfo.fIcon = TRUE;
    iconInfo.hbmColor = hBitmap;
    iconInfo.hbmMask = hMask;
    HICON hIcon = CreateIconIndirect(&iconInfo);

    DeleteDC(hdcMask);
    DeleteDC(hdc);
    DeleteObject(hBitmap);
    DeleteObject(hMask);

    last_icon = hIcon;
    wcscpy(last_text, text);
    last_dots = dots;

    return hIcon;
}

// Update system tray icon
void update_tray_icon(HWND hwnd, const wchar_t* text, int dots, int seconds) {
    char tooltip[8];
    int min = seconds / 60, sec = seconds % 60;
    tooltip[0] = '0' + (min / 10);
    tooltip[1] = '0' + (min % 10);
    tooltip[2] = ':';
    tooltip[3] = '0' + (sec / 10);
    tooltip[4] = '0' + (sec % 10);
    tooltip[5] = '\0';
    strncpy(nid.szTip, tooltip, sizeof(nid.szTip));
    HICON hIcon = create_tray_icon(text, dots);
    nid.hIcon = hIcon;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

// Play sound from resource
void play_resource_sound(const char* resourceName) {
    HRSRC hRes = FindResourceA(NULL, resourceName, "WAV");
    if (hRes) {
        HGLOBAL hData = LoadResource(NULL, hRes);
        if (hData) {
            LPVOID pData = LockResource(hData);
            PlaySoundA((LPCSTR)pData, NULL, SND_MEMORY | SND_ASYNC);
            UnlockResource(hData);
        }
    }
}

// Timer thread
DWORD WINAPI timer_thread(LPVOID lpParam) {
    while (is_running) {
        if (remaining_seconds <= 0) {
            is_running = 0;
            if (is_in_pomodoro) {
                pomodoro_count++;
                if (pomodoro_count > 4) pomodoro_count = 1;
            }
            update_tray_icon((HWND)lpParam, L"\u25BA", pomodoro_count, 0);
            play_resource_sound("DING_WAV");
            return 0;
        }
        remaining_seconds--;

        wchar_t display_text[16];
        play_tick_sound();
        if (remaining_seconds < 60) {
            _itow(remaining_seconds, display_text, 10);
        } else {
            _itow(remaining_seconds / 60, display_text, 10);
        }
        update_tray_icon((HWND)lpParam, display_text, pomodoro_count, remaining_seconds);
        Sleep(1000);
    }
    return 0;
}

// Start timer
void start_timer(HWND hwnd, int duration_minutes) {
    if (is_running) {
        is_running = 0;
        if (timer_thread_handle != NULL) {
            WaitForSingleObject(timer_thread_handle, INFINITE);
            CloseHandle(timer_thread_handle);
            timer_thread_handle = NULL;
        }
    }

    remaining_seconds = duration_minutes * 60;
    is_running = 1;
    timer_thread_handle = CreateThread(NULL, 0, timer_thread, hwnd, 0, NULL);
}

// Start looping tick sound
void play_tick_sound() {
    if (!settings.enable_clock_sound) return;
    if (tick_sound == 1) {
        play_resource_sound("TICK_WAV");
        tick_sound = 0;
    } else {
        play_resource_sound("TOCK_WAV");
        tick_sound = 1;
    }
}

// Check if autostart is enabled
int is_autostart_enabled() {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD type;
        if (RegQueryValueExA(hKey, "PomodoroTimer", NULL, &type, NULL, NULL) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return 1;
        }
        RegCloseKey(hKey);
    }
    return 0;
}

// Enable or disable autostart
void set_autostart(int enable) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        if (enable) {
            char path[MAX_PATH];
            GetModuleFileNameA(NULL, path, MAX_PATH);
            RegSetValueExA(hKey, "PomodoroTimer", 0, REG_SZ, (BYTE*)path, strlen(path) + 1);
        } else {
            RegDeleteValueA(hKey, "PomodoroTimer");
        }
        RegCloseKey(hKey);
    }
}

// Show settings dialog
void ShowSettingsDialog(HWND hwndParent) {
    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SETTINGS), hwndParent, SettingsDlgProc);
}

// Settings dialog procedure
INT_PTR CALLBACK SettingsDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_INITDIALOG: {
            char buf[16];
            snprintf(buf, sizeof(buf), "%d", settings.pomodoro_duration);
            SetDlgItemTextA(hwndDlg, IDC_POMODORO, buf);
            snprintf(buf, sizeof(buf), "%d", settings.short_break_duration);
            SetDlgItemTextA(hwndDlg, IDC_SHORT_BREAK, buf);
            snprintf(buf, sizeof(buf), "%d", settings.long_break_duration);
            SetDlgItemTextA(hwndDlg, IDC_LONG_BREAK, buf);
            SendDlgItemMessageA(hwndDlg, IDC_CLOCK_SOUND, BM_SETCHECK, settings.enable_clock_sound ? BST_CHECKED : BST_UNCHECKED, 0);
            SendDlgItemMessageA(hwndDlg, IDC_AUTOSTART, BM_SETCHECK, autostart_enabled ? BST_CHECKED : BST_UNCHECKED, 0);

            // Center dialog
            RECT rect;
            GetWindowRect(hwndDlg, &rect);
            int dlgWidth = rect.right - rect.left;
            int dlgHeight = rect.bottom - rect.top;
            int xPos = (screenWidth - dlgWidth) / 2;
            int yPos = (screenHeight - dlgHeight) / 2;
            SetWindowPos(hwndDlg, NULL, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            return TRUE;
        }
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDC_OK: {
                    char buf[16];
                    int pomodoro, short_break, long_break;
                    GetDlgItemTextA(hwndDlg, IDC_POMODORO, buf, sizeof(buf));
                    pomodoro = atoi(buf);
                    GetDlgItemTextA(hwndDlg, IDC_SHORT_BREAK, buf, sizeof(buf));
                    short_break = atoi(buf);
                    GetDlgItemTextA(hwndDlg, IDC_LONG_BREAK, buf, sizeof(buf));
                    long_break = atoi(buf);
                    if (pomodoro > 0 && pomodoro <= 120 && short_break > 0 && short_break <= 60 && long_break > 0 && long_break <= 120) {
                        settings.pomodoro_duration = pomodoro;
                        settings.short_break_duration = short_break;
                        settings.long_break_duration = long_break;
                        settings.enable_clock_sound = SendDlgItemMessageA(hwndDlg, IDC_CLOCK_SOUND, BM_GETCHECK, 0, 0) == BST_CHECKED;
                        autostart_enabled = SendDlgItemMessageA(hwndDlg, IDC_AUTOSTART, BM_GETCHECK, 0, 0) == BST_CHECKED;
                        set_autostart(autostart_enabled);
                        save_settings();
                        EndDialog(hwndDlg, IDOK);
                    } else {
                        MessageBoxA(hwndDlg, "Invalid time values! Must be positive and reasonable.", "Error", MB_ICONERROR);
                    }
                    return TRUE;
                }
                case IDC_CANCEL:
                    EndDialog(hwndDlg, IDCANCEL);
                    return TRUE;
            }
            break;
        case WM_CLOSE:
            EndDialog(hwndDlg, IDCANCEL);
            return TRUE;
    }
    return FALSE;
}

// Show about dialog
void ShowAboutDialog(HWND hwndParent) {
    INT_PTR result = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwndParent, AboutDlgProc);
}

// About dialog procedure
INT_PTR CALLBACK AboutDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HFONT hLinkFont = NULL;
    static HBRUSH hBrush = NULL;

    switch (uMsg) {
        case WM_INITDIALOG: {
            // Create underlined font for link
            HFONT hFont = (HFONT)SendDlgItemMessageW(hwndDlg, IDC_WEBSITE, WM_GETFONT, 0, 0);
            LOGFONTW lf;
            GetObjectW(hFont, sizeof(LOGFONTW), &lf);
            lf.lfUnderline = TRUE;
            hLinkFont = CreateFontIndirectW(&lf);
            SendDlgItemMessageW(hwndDlg, IDC_WEBSITE, WM_SETFONT, (WPARAM)hLinkFont, TRUE);

            // Set link text
            SetDlgItemTextW(hwndDlg, IDC_WEBSITE, L"Visit our website");

            // Create brush for WM_CTLCOLORSTATIC
            hBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));

            // Center dialog
            RECT rect;
            GetWindowRect(hwndDlg, &rect);
            int dlgWidth = rect.right - rect.left;
            int dlgHeight = rect.bottom - rect.top;
            int xPos = (screenWidth - dlgWidth) / 2;
            int yPos = (screenHeight - dlgHeight) / 2;
            SetWindowPos(hwndDlg, NULL, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            return TRUE;
        }
        case WM_CTLCOLORSTATIC: {
            HWND hwndStatic = (HWND)lParam;
            if (GetDlgCtrlID(hwndStatic) == IDC_WEBSITE) {
                HDC hdc = (HDC)wParam;
                SetTextColor(hdc, RGB(0, 0, 255));
                SetBkMode(hdc, TRANSPARENT);
                return (LRESULT)hBrush;
            }
            break;
        }
        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_WEBSITE && HIWORD(wParam) == STN_CLICKED) {
                ShellExecuteW(NULL, L"open", L"https://github.com/lutischan-ferenc/pomodoro-timer-v2", NULL, NULL, SW_SHOWNORMAL);
                return TRUE;
            }
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                if (hLinkFont) {
                    DeleteObject(hLinkFont);
                    hLinkFont = NULL;
                }
                if (hBrush) {
                    DeleteObject(hBrush);
                    hBrush = NULL;
                }
                EndDialog(hwndDlg, LOWORD(wParam));
                return TRUE;
            }
            break;
        case WM_DESTROY:
            if (hLinkFont) {
                DeleteObject(hLinkFont);
                hLinkFont = NULL;
            }
            if (hBrush) {
                DeleteObject(hBrush);
                hBrush = NULL;
            }
            break;
        case WM_CLOSE:
            if (hLinkFont) {
                DeleteObject(hLinkFont);
                hLinkFont = NULL;
            }
            if (hBrush) {
                DeleteObject(hBrush);
                hBrush = NULL;
            }
            EndDialog(hwndDlg, IDCANCEL);
            return TRUE;
    }
    return FALSE;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_USER + 1:
            if (LOWORD(lParam) == WM_LBUTTONUP) {
                if (is_running) {
                    is_running = 0;
                    update_tray_icon(hwnd, L"\u25BA", pomodoro_count, 0);
                    Shell_NotifyIcon(NIM_MODIFY, &nid);
                } else {
                    if (is_in_pomodoro) {
                        is_in_pomodoro = 0;
                        start_timer(hwnd, pomodoro_count == 4 ? settings.long_break_duration : settings.short_break_duration);
                    } else {
                        is_in_pomodoro = 1;
                        start_timer(hwnd, settings.pomodoro_duration);
                    }
                }
            } else if (LOWORD(lParam) == WM_RBUTTONUP) {
                HMENU hMenu = CreatePopupMenu();
                AppendMenu(hMenu, MF_STRING, 1, "Start Pomodoro");
                AppendMenu(hMenu, MF_STRING, 2, "Start Break");
                AppendMenu(hMenu, MF_STRING, 3, "Start Long Break");
                AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                AppendMenu(hMenu, MF_STRING | (settings.enable_clock_sound ? MF_CHECKED : 0), 4, "Clock Sound");
                AppendMenu(hMenu, MF_STRING | (autostart_enabled ? MF_CHECKED : 0), 5, "Autostart");
                AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                AppendMenu(hMenu, MF_STRING, 6, "Settings");
                AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                AppendMenu(hMenu, MF_STRING, 7, "About");
                AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                AppendMenu(hMenu, MF_STRING, 8, "Exit");

                POINT pt;
                GetCursorPos(&pt);
                SetForegroundWindow(hwnd);
                int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, 0, hwnd, NULL);
                DestroyMenu(hMenu);

                switch (cmd) {
                    case 1:
                        is_in_pomodoro = 1;
                        start_timer(hwnd, settings.pomodoro_duration);
                        break;
                    case 2:
                        is_in_pomodoro = 0;
                        start_timer(hwnd, settings.short_break_duration);
                        break;
                    case 3:
                        is_in_pomodoro = 0;
                        start_timer(hwnd, settings.long_break_duration);
                        break;
                    case 4:
                        settings.enable_clock_sound = !settings.enable_clock_sound;
                        save_settings();
                        break;
                    case 5:
                        autostart_enabled = !autostart_enabled;
                        set_autostart(autostart_enabled);
                        break;
                    case 6:
                        ShowSettingsDialog(hwnd);
                        break;
                    case 7:
                        ShowAboutDialog(hwnd);
                        break;
                    case 8:
                        Shell_NotifyIcon(NIM_DELETE, &nid);
                        PostQuitMessage(0);
                        break;
                }
            }
            break;
        case WM_DESTROY:
            is_running = 0;
            if (timer_thread_handle != NULL) {
                WaitForSingleObject(timer_thread_handle, INFINITE);
                CloseHandle(timer_thread_handle);
                timer_thread_handle = NULL;
            }
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Load settings
void load_settings() {
    FILE* fp = fopen("pomodoro_settings.json", "r");
    if (fp) {
        char buf[256] = {0};
        if (fread(buf, 1, sizeof(buf) - 1, fp) > 0) {
            if (sscanf(buf, "{\"pomodoro_duration\":%d,\"short_break_duration\":%d,\"long_break_duration\":%d,\"enable_clock_sound\":%d}",
                       &settings.pomodoro_duration, &settings.short_break_duration, &settings.long_break_duration, &settings.enable_clock_sound) != 4) {
                settings.pomodoro_duration = 25;
                settings.short_break_duration = 5;
                settings.long_break_duration = 15;
                settings.enable_clock_sound = 1;
            }
        }
        fclose(fp);
    }
}

// Save settings
void save_settings() {
    FILE* fp = fopen("pomodoro_settings.json", "w");
    if (fp) {
        fprintf(fp, "{\"pomodoro_duration\":%d,\"short_break_duration\":%d,\"long_break_duration\":%d,\"enable_clock_sound\":%d}",
                settings.pomodoro_duration, settings.short_break_duration, settings.long_break_duration, settings.enable_clock_sound);
        fclose(fp);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    init_system_metrics();
    load_settings();
    autostart_enabled = is_autostart_enabled();

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "PomodoroTimer";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("PomodoroTimer", "Pomodoro Timer", 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);

    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_USER + 1;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    strncpy(nid.szTip, "Pomodoro Timer", sizeof(nid.szTip));
    Shell_NotifyIcon(NIM_ADD, &nid);

    update_tray_icon(hwnd, L"\u25BA", pomodoro_count, 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (last_icon) {
        DestroyIcon(last_icon);
        last_icon = NULL;
    }

    return 0;
}