#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <mmsystem.h>
#include <tchar.h>

#define ID_MENU_LANGUAGE 301
#define ID_MENU_LANG_EN 302
#define ID_MENU_LANG_HU 303
#define ID_MENU_LANG_DE 304
#define ID_MENU_LANG_IT 305
#define ID_MENU_LANG_ES 306
#define ID_MENU_LANG_FR 307
#define ID_MENU_LANG_RU 308
#define ID_MENU_RESET_COUNT 309
#define TOAST_WINDOW_CLASS L"PomodoroToastClass"
#define TIMER_ID 1
#define TIMER_INTERVAL_MS 1000
#define ID_TOAST_ACTION 2001
#define ID_TOAST_CLOSE 2002
#define ID_TOAST_RESET 2003

// Structure for localized strings
typedef struct {
    WCHAR menu_start_pomodoro[64];
    WCHAR menu_start_break[64];
    WCHAR menu_start_long_break[64];
    WCHAR menu_clock_sound[64];
    WCHAR menu_autostart[64];
    WCHAR menu_show_dialog[64];
    WCHAR menu_settings[64];
    WCHAR menu_language[64];
    WCHAR menu_about[64];
    WCHAR menu_exit[64];
    WCHAR menu_reset_count[64];
    WCHAR tooltip_pomodoro[64];
    WCHAR tooltip_break[64];
    WCHAR settings_title[64];
    WCHAR settings_pomodoro[64];
    WCHAR settings_short_break[64];
    WCHAR settings_long_break[64];
    WCHAR settings_enable_sound[64];
    WCHAR settings_autostart[64];
    WCHAR error_invalid_time[128];
    WCHAR notify_pomodoro_complete[128];
    WCHAR notify_break_complete[128];
    WCHAR notify_long_break_complete[128];
} LANG;

// Language definitions
static LANG lang_en = {
    L"Start Pomodoro",
    L"Start Break",
    L"Start Long Break",
    L"Clock Sound",
    L"Start on System Startup",
    L"Show Completion Dialog",
    L"Settings",
    L"Language",
    L"About",
    L"Exit",
    L"Reset Pomodoro Sessions",
    L"Click to start a pomodoro",
    L"Click to start a break",
    L"Pomodoro Settings",
    L"Pomodoro duration (minutes):",
    L"Short break duration (minutes):",
    L"Long break duration (minutes):",
    L"Enable clock sound",
    L"Start on system startup",
    L"Invalid time values! Must be positive and reasonable.",
    L"Pomodoro Complete!",
    L"Break Complete!",
    L"Long Break Complete!"
};

static LANG lang_hu = {
    L"Pomodoro indítása",
    L"Szünet indítása",
    L"Hosszú szünet indítása",
    L"Óra hang",
    L"Indítás rendszerindításkor",
    L"Befejezési ablak megjelenítése",
    L"Beállítások",
    L"Nyelv",
    L"Névjegy",
    L"Kilépés",
    L"Pomodoro körök törlése",
    L"Kattints egy pomodoro indításához",
    L"Kattints egy szünet indításához",
    L"Pomodoro beállítások",
    L"Pomodoro időtartam (perc):",
    L"Rövid szünet időtartama (perc):",
    L"Hosszú szünet időtartama (perc):",
    L"Óra hang engedélyezése",
    L"Indítás rendszerindításkor",
    L"Érvénytelen időértékek! Pozitívnak és ésszerűnek kell lenniük.",
    L"Pomodoro kész!",
    L"Szünet kész!",
    L"Hosszú szünet kész!"
};

static LANG lang_de = {
    L"Pomodoro starten",
    L"Pause starten",
    L"Lange Pause starten",
    L"Uhrenton",
    L"Beim Systemstart starten",
    L"Abschlussdialog anzeigen",
    L"Einstellungen",
    L"Sprache",
    L"Über",
    L"Beenden",
    L"Pomodoro-Sitzungen zurücksetzen",
    L"Klicken Sie, um einen Pomodoro zu starten",
    L"Klicken Sie, um eine Pause zu starten",
    L"Pomodoro-Einstellungen",
    L"Pomodoro-Dauer (Minuten):",
    L"Kurze Pausendauer (Minuten):",
    L"Lange Pausendauer (Minuten):",
    L"Uhrenton aktivieren",
    L"Beim Systemstart starten",
    L"Ungültige Zeitwerte! Müssen positiv und angemessen sein.",
    L"Pomodoro abgeschlossen!",
    L"Pause abgeschlossen!",
    L"Lange Pause abgeschlossen!"
};

static LANG lang_it = {
    L"Avvia Pomodoro",
    L"Avvia Pausa",
    L"Avvia Pausa Lunga",
    L"Suono Orologio",
    L"Avvia all'Avvio del Sistema",
    L"Mostra dialogo completamento",
    L"Impostazioni",
    L"Lingua",
    L"Info",
    L"Esci",
    L"Reimposta sessioni Pomodoro",
    L"Clicca per avviare un pomodoro",
    L"Clicca per avviare una pausa",
    L"Impostazioni Pomodoro",
    L"Durata pomodoro (minuti):",
    L"Durata pausa breve (minuti):",
    L"Durata pausa lunga (minuti):",
    L"Abilita suono orologio",
    L"Avvia all'avvio del sistema",
    L"Valori temporali non validi! Devono essere positivi e ragionevoli.",
    L"Pomodoro Completato!",
    L"Pausa Completata!",
    L"Pausa Lunga Completata!"
};

static LANG lang_es = {
    L"Iniciar Pomodoro",
    L"Iniciar Descanso",
    L"Iniciar Descanso Largo",
    L"Sonido del Reloj",
    L"Iniciar con el Sistema",
    L"Mostrar diálogo finalización",
    L"Configuración",
    L"Idioma",
    L"Acerca de",
    L"Salir",
    L"Reiniciar sesiones Pomodoro",
    L"Clic para iniciar un pomodoro",
    L"Clic para iniciar un descanso",
    L"Configuración de Pomodoro",
    L"Duración del pomodoro (minutos):",
    L"Duración del descanso corto (minutos):",
    L"Duración del descanso largo (minutos):",
    L"Habilitar sonido del reloj",
    L"Iniciar con el sistema",
    L"¡Valores de tiempo inválidos! Deben ser positivos y razonables.",
    L"¡Pomodoro completado!",
    L"¡Descanso completado!",
    L"¡Descanso largo completado!"
};

static LANG lang_fr = {
    L"Démarrer Pomodoro",
    L"Démarrer Pause",
    L"Démarrer Pause Longue",
    L"Son de l'Horloge",
    L"Démarrer avec le Système",
    L"Afficher dialogue de fin",
    L"Paramètres",
    L"Langue",
    L"À propos",
    L"Quitter",
    L"Réinitialiser sessions Pomodoro",
    L"Cliquez pour démarrer un pomodoro",
    L"Cliquez pour démarrer une pause",
    L"Paramètres Pomodoro",
    L"Durée pomodoro (minutes):",
    L"Durée pause courte (minutes):",
    L"Durée pause longue (minutes):",
    L"Activer le son de l'horloge",
    L"Démarrer avec le système",
    L"Valeurs de temps invalides! Doivent être positives et raisonnables.",
    L"Pomodoro Terminé!",
    L"Pause Terminée!",
    L"Pause Longue Terminée!"
};

static LANG lang_ru = {
    L"Запустить Помодоро",
    L"Запустить Перерыв",
    L"Запустить Длинный Перерыв",
    L"Звук Часов",
    L"Запускать при Старте Системы",
    L"Показывать диалог завершения",
    L"Настройки",
    L"Язык",
    L"О программе",
    L"Выход",
    L"Сбросить сессии Помодоро",
    L"Нажмите для запуска помодоро",
    L"Нажмите для запуска перерыва",
    L"Настройки Помодоро",
    L"Длительность помодоро (минуты):",
    L"Длительность короткого перерыва (минуты):",
    L"Длительность длинного перерыва (минуты):",
    L"Включить звук часов",
    L"Запускать при старте системы",
    L"Недопустимые значения времени! Должны быть положительными и разумными.",
    L"Помодоро завершено!",
    L"Перерыв завершен!",
    L"Длинный перерыв завершен!"
};

static LANG *g_lang = &lang_en;
static HMENU g_hLangMenu = NULL;
static HMENU g_hMenu = NULL;

// Timer settings structure
typedef struct {
    int pomodoro_duration;
    int short_break_duration;
    int long_break_duration;
    int enable_clock_sound;
    int show_completion_dialog;
} TimerSettings;

// Global variables
TimerSettings settings = {25, 5, 15, 1, 1};
int pomodoro_count = 0;
int is_running = 0;
static int is_paused = 0;
int is_in_pomodoro = 0;
int is_in_long_break = 0;
int remaining_seconds = 0;
NOTIFYICONDATA nid = {0};
static ULONGLONG timer_deadline = 0;
static int last_beeped_second = -1;
int autostart_enabled = 0;
static HICON last_icon = NULL;
static HICON g_shell_icon = NULL;
static BOOL g_shell_icon_owned = FALSE;
static wchar_t last_text[16] = {0};
static int last_dots = -1;
static int screenWidth = 0, screenHeight = 0;
static HWND g_hToastWnd = NULL;
static HWND g_main_hwnd = NULL; // main invisible window handle
static int toast_is_pomodoro = 0;
static int toast_is_long_break = 0;
static HWND g_hToastButton = NULL;
static HWND g_hToastCloseButton = NULL;
static BOOL g_menu_initialized = FALSE;
static BOOL g_toast_class_registered = FALSE;
static BOOL g_tray_icon_removed = FALSE;
static HFONT g_hToastButtonFont = NULL;

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
#define IDC_COFFEE 109

// Function prototypes
void load_settings();
void save_settings();
void update_tray_icon(HWND hwnd, const wchar_t* text, int dots, int seconds);
void play_resource_sound(const char* resourceName);
int is_autostart_enabled();
void RefreshMenuText(void);
BOOL set_autostart(int enable);
INT_PTR CALLBACK SettingsDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AboutDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ShowSettingsDialog(HWND hwndParent);
void ShowAboutDialog(HWND hwndParent);
void ShowCompletionNotification(HWND hwnd, int is_pomodoro_complete, int is_long_break);
void format_display_text(int seconds, wchar_t* buffer, int buffer_size);
void InitializeContextMenu(void);
void UpdateMenuCheckmarks(void);

// Initialize system metrics for dialog positioning
void init_system_metrics() {
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
}

// Helper function to get language ID from LANG pointer
DWORD GetLanguageIdFromLang(LANG *lang) {
    if (lang == &lang_en) return ID_MENU_LANG_EN;
    if (lang == &lang_hu) return ID_MENU_LANG_HU;
    if (lang == &lang_de) return ID_MENU_LANG_DE;
    if (lang == &lang_it) return ID_MENU_LANG_IT;
    if (lang == &lang_es) return ID_MENU_LANG_ES;
    if (lang == &lang_fr) return ID_MENU_LANG_FR;
    return ID_MENU_LANG_RU;
}

// Helper function to get LANG pointer from language ID
LANG* GetLangFromLanguageId(DWORD langId) {
    switch (langId) {
        case ID_MENU_LANG_EN: return &lang_en;
        case ID_MENU_LANG_HU: return &lang_hu;
        case ID_MENU_LANG_DE: return &lang_de;
        case ID_MENU_LANG_IT: return &lang_it;
        case ID_MENU_LANG_ES: return &lang_es;
        case ID_MENU_LANG_FR: return &lang_fr;
        case ID_MENU_LANG_RU: return &lang_ru;
        default: return &lang_en;
    }
}

// Helper function to format display text based on remaining seconds
void format_display_text(int seconds, wchar_t* buffer, int buffer_size) {
    int value;
    if (!buffer || buffer_size <= 0) return;
    if (seconds < 0) seconds = 0;
    value = (seconds < 60) ? seconds : seconds / 60;
    if (swprintf(buffer, (size_t)buffer_size, L"%d", value) < 0)
        buffer[0] = L'\0';
}

// Initialize cached context menu (called once)
void InitializeContextMenu(void) {
    if (g_menu_initialized) return;

    g_hMenu = CreatePopupMenu();
    if (!g_hMenu) {
        OutputDebugStringW(L"Pomodoro Timer: failed to create context menu.\\n");
        return;
    }
    AppendMenu(g_hMenu, MF_STRING, 1, g_lang->menu_start_pomodoro);
    AppendMenu(g_hMenu, MF_STRING, 2, g_lang->menu_start_break);
    AppendMenu(g_hMenu, MF_STRING, 3, g_lang->menu_start_long_break);
    AppendMenu(g_hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(g_hMenu, MF_STRING, 4, g_lang->menu_clock_sound);
    AppendMenu(g_hMenu, MF_STRING, 5, g_lang->menu_autostart);
    AppendMenu(g_hMenu, MF_STRING, 9, g_lang->menu_show_dialog);
    AppendMenu(g_hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(g_hMenu, MF_STRING, 6, g_lang->menu_settings);

    // Create language submenu
    g_hLangMenu = CreatePopupMenu();
    if (!g_hLangMenu) {
        DestroyMenu(g_hMenu);
        g_hMenu = NULL;
        OutputDebugStringW(L"Pomodoro Timer: failed to create language menu.\\n");
        return;
    }
    AppendMenu(g_hLangMenu, MF_STRING, ID_MENU_LANG_EN, _T("English"));
    AppendMenu(g_hLangMenu, MF_STRING, ID_MENU_LANG_HU, _T("Magyar"));
    AppendMenu(g_hLangMenu, MF_STRING, ID_MENU_LANG_DE, _T("Deutsch"));
    AppendMenu(g_hLangMenu, MF_STRING, ID_MENU_LANG_IT, _T("Italiano"));
    AppendMenu(g_hLangMenu, MF_STRING, ID_MENU_LANG_ES, _T("Español"));
    AppendMenu(g_hLangMenu, MF_STRING, ID_MENU_LANG_FR, _T("Français"));
    AppendMenu(g_hLangMenu, MF_STRING, ID_MENU_LANG_RU, _T("Русский"));
    AppendMenu(g_hMenu, MF_POPUP, (UINT_PTR)g_hLangMenu, g_lang->menu_language);

    AppendMenu(g_hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(g_hMenu, MF_STRING, 7, g_lang->menu_about);
    AppendMenu(g_hMenu, MF_STRING, ID_MENU_RESET_COUNT, g_lang->menu_reset_count);
    AppendMenu(g_hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(g_hMenu, MF_STRING, 8, g_lang->menu_exit);

    UpdateMenuCheckmarks();
    g_menu_initialized = TRUE;
}

// Update menu checkmarks without recreating menu
void UpdateMenuCheckmarks(void) {
    if (!g_hMenu) return;

    CheckMenuItem(g_hMenu, 4, MF_BYCOMMAND | (settings.enable_clock_sound ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(g_hMenu, 5, MF_BYCOMMAND | (autostart_enabled ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(g_hMenu, 9, MF_BYCOMMAND | (settings.show_completion_dialog ? MF_CHECKED : MF_UNCHECKED));

    if (g_hLangMenu) {
        DWORD currentLangId = GetLanguageIdFromLang(g_lang);
        CheckMenuRadioItem(g_hLangMenu, ID_MENU_LANG_EN, ID_MENU_LANG_RU, currentLangId, MF_BYCOMMAND);
    }
}

// Sets the application language and refreshes the UI
void SetLanguage(LANG *newLang) {
    g_lang = newLang;
    RefreshMenuText();
}

// Saves the selected language to the registry
void SaveLanguageSelectionToRegistry(void) {
    HKEY hKey;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\PomodoroTimer", 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        DWORD langId = GetLanguageIdFromLang(g_lang);
        if (RegSetValueExW(hKey, L"Language", 0, REG_DWORD,
            (const BYTE *)&langId, sizeof(DWORD)) != ERROR_SUCCESS) {
            OutputDebugStringW(L"Pomodoro Timer: failed to save language selection.\\n");
        }
        RegCloseKey(hKey);
    }
}

// Loads the selected language from the registry
BOOL LoadLanguageSelectionFromRegistry(void) {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\PomodoroTimer", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD langId = ID_MENU_LANG_EN, dataSize = sizeof(DWORD), type = 0;
        if (RegQueryValueExW(hKey, L"Language", NULL, &type, (LPBYTE)&langId, &dataSize) == ERROR_SUCCESS &&
            type == REG_DWORD && dataSize == sizeof(DWORD)) {
            g_lang = GetLangFromLanguageId(langId);
            RegCloseKey(hKey);
            return TRUE;
        }
        RegCloseKey(hKey);
    }
    return FALSE;
}

// Refreshes menu text with current language
void RefreshMenuText(void) {
    if (!g_hMenu) return;

    ModifyMenu(g_hMenu, 1, MF_BYCOMMAND | MF_STRING, 1, g_lang->menu_start_pomodoro);
    ModifyMenu(g_hMenu, 2, MF_BYCOMMAND | MF_STRING, 2, g_lang->menu_start_break);
    ModifyMenu(g_hMenu, 3, MF_BYCOMMAND | MF_STRING, 3, g_lang->menu_start_long_break);
    ModifyMenu(g_hMenu, 4, MF_BYCOMMAND | MF_STRING, 4, g_lang->menu_clock_sound);
    ModifyMenu(g_hMenu, 5, MF_BYCOMMAND | MF_STRING, 5, g_lang->menu_autostart);
    ModifyMenu(g_hMenu, 9, MF_BYCOMMAND | MF_STRING, 9, g_lang->menu_show_dialog);
    ModifyMenu(g_hMenu, 6, MF_BYCOMMAND | MF_STRING, 6, g_lang->menu_settings);
    ModifyMenu(g_hMenu, 7, MF_BYCOMMAND | MF_STRING, 7, g_lang->menu_about);
    ModifyMenu(g_hMenu, ID_MENU_RESET_COUNT, MF_BYCOMMAND | MF_STRING, ID_MENU_RESET_COUNT, g_lang->menu_reset_count);
    ModifyMenu(g_hMenu, 8, MF_BYCOMMAND | MF_STRING, 8, g_lang->menu_exit);
    ModifyMenu(g_hMenu, (UINT_PTR)g_hLangMenu, MF_BYCOMMAND | MF_POPUP, (UINT_PTR)g_hLangMenu, g_lang->menu_language);

    UpdateMenuCheckmarks();
}

// Create dynamic tray icon with text and dots
HICON create_tray_icon(const wchar_t* text, int dots) {
    // Return cached icon if nothing has changed
    if (last_icon && wcscmp(text, last_text) == 0 && dots == last_dots) {
        return last_icon;
    }

    // Create device context and bitmap
    HDC hdc = CreateCompatibleDC(NULL);
    if (!hdc) return NULL;
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = 32;
    bmi.bmiHeader.biHeight = -32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* bits = NULL;
    HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
    if (!hBitmap || !bits) {
        if (hBitmap) DeleteObject(hBitmap);
        DeleteDC(hdc);
        return NULL;
    }
    SelectObject(hdc, hBitmap);

    // Draw background (dark red)
    HBRUSH bgBrush = CreateSolidBrush(RGB(139, 0, 0));
    RECT rect = {0, 0, 32, 32};
    if (bgBrush) {
        FillRect(hdc, &rect, bgBrush);
        DeleteObject(bgBrush);
    } else {
        DeleteDC(hdc);
        DeleteObject(hBitmap);
        return NULL;
    }

    // Set up text rendering
    HFONT hFont = CreateFontW(
        27, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial"
    );
    if (!hFont) {
        DeleteDC(hdc);
        DeleteObject(hBitmap);
        return NULL;
    }
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    // Draw text centered
    SIZE textSize = {0, 0};
    GetTextExtentPoint32W(hdc, text, (int)wcslen(text), &textSize);
    int textX = (32 - textSize.cx) / 2;
    int textY = (32 - textSize.cy) / 2 - 2;

    // Slight adjustment for play button
    if (wcscmp(text, L"\u25BA") == 0) {
        textX += 2;
    }

    TextOutW(hdc, textX, textY, text, wcslen(text));

    // Draw progress dots (light green)
    HBRUSH dotBrush = CreateSolidBrush(RGB(144, 238, 144));
    HBRUSH oldBrush = dotBrush ? (HBRUSH)SelectObject(hdc, dotBrush) : NULL;
    if (dotBrush && oldBrush) for (int i = 0; i < dots && i < 4; i++) {
        int dotX = 4 + i * 8;
        int dotY = 30;
        Ellipse(hdc, dotX - 3, dotY - 3, dotX + 3, dotY + 3);
    }
    if (oldBrush) SelectObject(hdc, oldBrush);
    if (dotBrush) DeleteObject(dotBrush);
    if (oldFont) SelectObject(hdc, oldFont);
    DeleteObject(hFont);

    // The icon mask must be a monochrome bitmap. For a 32-bit color icon,
    // make the mask fully opaque and use the color bitmap's alpha channel.
    BYTE *pixelBytes = (BYTE *)bits;
    for (int pixel = 0; pixel < 32 * 32; ++pixel)
        pixelBytes[pixel * 4 + 3] = 255;

    HBITMAP hMask = CreateBitmap(32, 32, 1, 1, NULL);
    HDC hdcMask = CreateCompatibleDC(hdc);
    if (!hMask || !hdcMask) {
        if (hMask) DeleteObject(hMask);
        if (hdcMask) DeleteDC(hdcMask);
        DeleteDC(hdc);
        DeleteObject(hBitmap);
        return NULL;
    }
    SelectObject(hdcMask, hMask);
    PatBlt(hdcMask, 0, 0, 32, 32, BLACKNESS);

    // Create icon
    ICONINFO iconInfo = {0};
    iconInfo.fIcon = TRUE;
    iconInfo.hbmColor = hBitmap;
    iconInfo.hbmMask = hMask;
    HICON hIcon = CreateIconIndirect(&iconInfo);

    // Clean up
    DeleteDC(hdcMask);
    DeleteDC(hdc);
    DeleteObject(hBitmap);
    DeleteObject(hMask);

    // Cache for later
    if (!hIcon) {
        last_dots = -1;
        last_text[0] = L'\0';
        return NULL;
    }
    last_icon = hIcon;
    wcsncpy(last_text, text, sizeof(last_text)/sizeof(last_text[0]) - 1);
    last_text[sizeof(last_text)/sizeof(last_text[0]) - 1] = L'\0';
    last_dots = dots;

    return hIcon;
}

// Update system tray icon and tooltip
void update_tray_icon(HWND hwnd, const wchar_t* text, int dots, int seconds) {
    if (!text) text = L"";
    if (dots < 0) dots = 0;
    if (dots > 4) dots = 4;
    wchar_t tooltip[128];
    if (seconds > 0) {
        int min = seconds / 60, sec = seconds % 60;
        if (swprintf(tooltip, sizeof(tooltip) / sizeof(tooltip[0]), L"%02d:%02d", min, sec) < 0)
            tooltip[0] = L'\0';
    } else {
        if (is_in_pomodoro) {
            wcsncpy(tooltip, g_lang->tooltip_break, sizeof(tooltip)/sizeof(tooltip[0]) - 1);
            tooltip[sizeof(tooltip)/sizeof(tooltip[0]) - 1] = L'\0';
        } else {
            wcsncpy(tooltip, g_lang->tooltip_pomodoro, sizeof(tooltip)/sizeof(tooltip[0]) - 1);
            tooltip[sizeof(tooltip)/sizeof(tooltip[0]) - 1] = L'\0';
        }
    }
    wcsncpy(nid.szTip, tooltip, sizeof(nid.szTip)/sizeof(nid.szTip[0]) - 1);
    nid.szTip[sizeof(nid.szTip)/sizeof(nid.szTip[0]) - 1] = L'\0';

    HICON newIcon = create_tray_icon(text, dots);
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    if (newIcon) nid.hIcon = newIcon;

    if (newIcon && Shell_NotifyIconW(NIM_MODIFY, &nid)) {
        HICON oldShellIcon = g_shell_icon;
        BOOL oldIconOwned = g_shell_icon_owned;
        g_shell_icon = newIcon;
        g_shell_icon_owned = TRUE;
        if (oldIconOwned && oldShellIcon && oldShellIcon != newIcon)
            DestroyIcon(oldShellIcon);
    } else if (newIcon) {
        OutputDebugStringW(L"Pomodoro Timer: Shell_NotifyIcon(NIM_MODIFY) failed.\\n");
        if (newIcon != g_shell_icon) {
            DestroyIcon(newIcon);
            last_icon = NULL;
            last_text[0] = L'\0';
            last_dots = -1;
        }
    }
}

// Play sound from resource
void play_resource_sound(const char* resourceName) {
    HRSRC hRes = FindResourceA(NULL, resourceName, "WAV");
    if (hRes) {
        HGLOBAL hData = LoadResource(NULL, hRes);
        if (hData) {
            LPVOID pData = LockResource(hData);
            if (pData) PlaySoundA((LPCSTR)pData, NULL, SND_MEMORY | SND_ASYNC);
            UnlockResource(hData);
        }
    }
}

static void start_clock_sound(void) {
    if (settings.enable_clock_sound) {
        HRSRC hRes = FindResourceA(NULL, "CLOCK_WAV", "WAV");
        if (hRes) {
            HGLOBAL hData = LoadResource(NULL, hRes);
            if (hData) {
                LPVOID pData = LockResource(hData);
                if (pData) PlaySoundA((LPCSTR)pData, NULL, SND_MEMORY | SND_ASYNC | SND_LOOP);
                UnlockResource(hData);
            }
        }
    }
}

static void stop_timer(HWND hwnd) {
    is_running = 0;
    is_paused = 0;
    KillTimer(hwnd, TIMER_ID);
    timer_deadline = 0;
    remaining_seconds = 0;
    is_in_long_break = 0;
    PlaySoundW(NULL, NULL, 0);
}

static void pause_timer(HWND hwnd) {
    if (!is_running) return;
    ULONGLONG now = GetTickCount64();
    if (timer_deadline <= now) return; // Let the next tick process completion.
    remaining_seconds = (timer_deadline > now)
        ? (int)((timer_deadline - now + 999ULL) / 1000ULL) : 0;
    KillTimer(hwnd, TIMER_ID);
    timer_deadline = 0;
    is_running = 0;
    is_paused = 1;
    PlaySoundW(NULL, NULL, 0);
    wchar_t display_text[16];
    format_display_text(remaining_seconds, display_text, (int)(sizeof(display_text) / sizeof(display_text[0])));
    update_tray_icon(hwnd, display_text, pomodoro_count, remaining_seconds);
}

static void resume_timer(HWND hwnd) {
    if (!is_paused || remaining_seconds <= 0) {
        is_paused = 0;
        return;
    }
    timer_deadline = GetTickCount64() + (ULONGLONG)remaining_seconds * 1000ULL;
    is_paused = 0;
    is_running = 1;
    if (!SetTimer(hwnd, TIMER_ID, TIMER_INTERVAL_MS, NULL)) {
        is_running = 0;
        is_paused = 1;
        timer_deadline = 0;
        MessageBoxW(hwnd, L"Could not resume the timer.", L"Pomodoro Timer", MB_OK | MB_ICONERROR);
        return;
    }
    start_clock_sound();
    wchar_t display_text[16];
    format_display_text(remaining_seconds, display_text, (int)(sizeof(display_text) / sizeof(display_text[0])));
    update_tray_icon(hwnd, display_text, pomodoro_count, remaining_seconds);
}

// Start timer with specified duration
void start_timer(HWND hwnd, int duration_minutes, int is_long_break) {
    if (!hwnd || duration_minutes < 1 || duration_minutes > 120) {
        if (hwnd) MessageBoxW(hwnd, L"Invalid timer duration.", L"Pomodoro Timer", MB_OK | MB_ICONERROR);
        return;
    }

    if (is_running || is_paused) stop_timer(hwnd);

    remaining_seconds = duration_minutes * 60;
    timer_deadline = GetTickCount64() + (ULONGLONG)remaining_seconds * 1000ULL;
    last_beeped_second = -1;
    is_running = 1;
    is_in_long_break = (is_long_break != 0);

    if (!SetTimer(hwnd, TIMER_ID, TIMER_INTERVAL_MS, NULL)) {
        is_running = 0;
        timer_deadline = 0;
        remaining_seconds = 0;
        is_in_long_break = 0;
        PlaySoundW(NULL, NULL, 0);
        MessageBoxW(hwnd, L"Could not start the timer.", L"Pomodoro Timer", MB_OK | MB_ICONERROR);
        update_tray_icon(hwnd, L"\u25BA", pomodoro_count, 0);
        return;
    }

    start_clock_sound();
    wchar_t display_text[16];
    format_display_text(remaining_seconds, display_text, (int)(sizeof(display_text) / sizeof(display_text[0])));
    update_tray_icon(hwnd, display_text, pomodoro_count, remaining_seconds);
}

static void handle_timer_tick(HWND hwnd) {
    if (!is_running) return;

    ULONGLONG now = GetTickCount64();
    if (now >= timer_deadline) {
        int was_pomodoro = is_in_pomodoro;
        int is_long_break = is_in_long_break;

        stop_timer(hwnd);
        if (was_pomodoro) {
            if (pomodoro_count < 4) pomodoro_count++;
            is_long_break = pomodoro_count == 4;
        }

        update_tray_icon(hwnd, L"\u25BA", pomodoro_count, 0);
        play_resource_sound("DING_WAV");
        if (settings.show_completion_dialog) {
            ShowCompletionNotification(hwnd, was_pomodoro, is_long_break);
        }
        return;
    }

    ULONGLONG remaining_ms = timer_deadline - now;
    remaining_seconds = (int)((remaining_ms + 999) / 1000);
    if (remaining_seconds > 0 && remaining_seconds <= 10 &&
        remaining_seconds != last_beeped_second && settings.enable_clock_sound) {
        Beep(440, 100);
        last_beeped_second = remaining_seconds;
    }

    wchar_t display_text[16];
    format_display_text(remaining_seconds, display_text, sizeof(display_text) / sizeof(display_text[0]));
    update_tray_icon(hwnd, display_text, pomodoro_count, remaining_seconds);
}

// Check if autostart is enabled in registry.
int is_autostart_enabled(void) {
    HKEY hKey;
    LONG result = RegOpenKeyExW(HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_QUERY_VALUE, &hKey);
    if (result != ERROR_SUCCESS) return 0;

    DWORD type = 0, size = 0;
    result = RegQueryValueExW(hKey, L"PomodoroTimer", NULL, &type, NULL, &size);
    RegCloseKey(hKey);
    return result == ERROR_SUCCESS &&
        (type == REG_SZ || type == REG_EXPAND_SZ) && size >= sizeof(WCHAR);
}

// Enable or disable autostart in registry. Return TRUE only on success.
BOOL set_autostart(int enable) {
    HKEY hKey;
    LONG result = RegCreateKeyExW(HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0,
        KEY_SET_VALUE, NULL, &hKey, NULL);
    if (result != ERROR_SUCCESS) return FALSE;

    if (enable) {
        WCHAR path[MAX_PATH];
        DWORD length = GetModuleFileNameW(NULL, path, MAX_PATH);
        if (length == 0 || length >= MAX_PATH) {
            RegCloseKey(hKey);
            return FALSE;
        }

        WCHAR quotedPath[MAX_PATH + 3];
        int written = swprintf(quotedPath, MAX_PATH + 3, L"\"%ls\"", path);
        if (written < 0 || written >= MAX_PATH + 3) {
            RegCloseKey(hKey);
            return FALSE;
        }
        result = RegSetValueExW(hKey, L"PomodoroTimer", 0, REG_SZ,
            (const BYTE *)quotedPath,
            (DWORD)((wcslen(quotedPath) + 1) * sizeof(WCHAR)));
    } else {
        result = RegDeleteValueW(hKey, L"PomodoroTimer");
        if (result == ERROR_FILE_NOT_FOUND) result = ERROR_SUCCESS;
    }

    RegCloseKey(hKey);
    return result == ERROR_SUCCESS;
}

// Show settings dialog
void ShowSettingsDialog(HWND hwndParent) {
    INT_PTR result = DialogBoxW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDD_SETTINGS), hwndParent, SettingsDlgProc);
    if (result == -1)
        MessageBoxW(hwndParent, L"Could not open the settings dialog.", L"Pomodoro Timer", MB_OK | MB_ICONERROR);
}

// Parse an integer from a Unicode edit control without accepting trailing junk.
static BOOL GetDlgItemIntStrict(HWND hwndDlg, int controlId, long minValue, long maxValue, int *outValue) {
    WCHAR text[64];
    WCHAR *end = NULL;
    long value;
    int length = GetDlgItemTextW(hwndDlg, controlId, text, (int)(sizeof(text) / sizeof(text[0])));
    if (length <= 0 || length >= (int)(sizeof(text) / sizeof(text[0])) - 1) return FALSE;

    errno = 0;
    value = wcstol(text, &end, 10);
    if (errno == ERANGE || end == text || value < minValue || value > maxValue) return FALSE;
    while (*end == L' ' || *end == L'\t' || *end == L'\r' || *end == L'\n') ++end;
    if (*end != L'\0') return FALSE;
    *outValue = (int)value;
    return TRUE;
}

// Settings dialog procedure
INT_PTR CALLBACK SettingsDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_INITDIALOG: {
            // Set dialog title localized
            SetWindowTextW(hwndDlg, g_lang->settings_title);
            // Set label texts (assume label IDs: 201, 202, 203)
            SetDlgItemTextW(hwndDlg, 201, g_lang->settings_pomodoro);
            SetDlgItemTextW(hwndDlg, 202, g_lang->settings_short_break);
            SetDlgItemTextW(hwndDlg, 203, g_lang->settings_long_break);
            SetDlgItemTextW(hwndDlg, IDC_CLOCK_SOUND, g_lang->settings_enable_sound);
            SetDlgItemTextW(hwndDlg, IDC_AUTOSTART, g_lang->settings_autostart);

            // Set edit values
            WCHAR buf[16];
            swprintf(buf, sizeof(buf) / sizeof(buf[0]), L"%d", settings.pomodoro_duration);
            SetDlgItemTextW(hwndDlg, IDC_POMODORO, buf);
            swprintf(buf, sizeof(buf) / sizeof(buf[0]), L"%d", settings.short_break_duration);
            SetDlgItemTextW(hwndDlg, IDC_SHORT_BREAK, buf);
            swprintf(buf, sizeof(buf) / sizeof(buf[0]), L"%d", settings.long_break_duration);
            SetDlgItemTextW(hwndDlg, IDC_LONG_BREAK, buf);
            SendDlgItemMessageW(hwndDlg, IDC_CLOCK_SOUND, BM_SETCHECK, settings.enable_clock_sound ? BST_CHECKED : BST_UNCHECKED, 0);
            SendDlgItemMessageW(hwndDlg, IDC_AUTOSTART, BM_SETCHECK, autostart_enabled ? BST_CHECKED : BST_UNCHECKED, 0);

            // Center dialog on screen
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
                case IDC_OK:
                case IDOK: {
                    int pomodoro, short_break, long_break;
                    BOOL valuesValid =
                        GetDlgItemIntStrict(hwndDlg, IDC_POMODORO, 1, 120, &pomodoro) &&
                        GetDlgItemIntStrict(hwndDlg, IDC_SHORT_BREAK, 1, 60, &short_break) &&
                        GetDlgItemIntStrict(hwndDlg, IDC_LONG_BREAK, 1, 120, &long_break);

                    if (valuesValid) {
                        int requestedAutostart;
                        settings.pomodoro_duration = pomodoro;
                        settings.short_break_duration = short_break;
                        settings.long_break_duration = long_break;
                        settings.enable_clock_sound =
                            SendDlgItemMessageW(hwndDlg, IDC_CLOCK_SOUND, BM_GETCHECK, 0, 0) == BST_CHECKED;
                        requestedAutostart =
                            SendDlgItemMessageW(hwndDlg, IDC_AUTOSTART, BM_GETCHECK, 0, 0) == BST_CHECKED;

                        if (set_autostart(requestedAutostart)) {
                            autostart_enabled = requestedAutostart;
                        } else {
                            MessageBoxW(hwndDlg, L"Could not update Windows autostart setting.",
                                L"Pomodoro Timer", MB_OK | MB_ICONWARNING);
                        }
                        if (is_running && settings.enable_clock_sound) start_clock_sound();
                        else if (!settings.enable_clock_sound) PlaySoundW(NULL, NULL, 0);
                        save_settings();
                        UpdateMenuCheckmarks();
                        EndDialog(hwndDlg, IDOK);
                    } else {
                        MessageBoxW(hwndDlg, g_lang->error_invalid_time, L"Error", MB_OK | MB_ICONERROR);
                    }
                    return TRUE;
                }
                case IDC_CANCEL:
                case IDCANCEL:
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
    INT_PTR result = DialogBoxW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDD_ABOUT), hwndParent, AboutDlgProc);
    if (result == -1)
        MessageBoxW(hwndParent, L"Could not open the About dialog.", L"Pomodoro Timer", MB_OK | MB_ICONERROR);
}

// About dialog procedure
INT_PTR CALLBACK AboutDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HFONT hLinkFont = NULL;
    static HBRUSH hBrush = NULL;

    switch (uMsg) {
        case WM_INITDIALOG: {
            // Create underlined font for link
            HFONT hFont = (HFONT)SendDlgItemMessageW(hwndDlg, IDC_WEBSITE, WM_GETFONT, 0, 0);
            LOGFONTW lf = {0};
            if (hFont && GetObjectW(hFont, sizeof(lf), &lf) == sizeof(lf)) {
                lf.lfUnderline = TRUE;
                hLinkFont = CreateFontIndirectW(&lf);
            }
            if (hLinkFont) {
                SendDlgItemMessageW(hwndDlg, IDC_WEBSITE, WM_SETFONT, (WPARAM)hLinkFont, TRUE);
                SendDlgItemMessageW(hwndDlg, IDC_COFFEE, WM_SETFONT, (WPARAM)hLinkFont, TRUE);
            }

            // Set link text
            SetDlgItemTextW(hwndDlg, IDC_WEBSITE, L"Visit our website");

            // Create brush for WM_CTLCOLORSTATIC
            hBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));

            // Center dialog on screen
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
            if (GetDlgCtrlID(hwndStatic) == IDC_WEBSITE || GetDlgCtrlID(hwndStatic) == IDC_COFFEE) {
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
            if (LOWORD(wParam) == IDC_COFFEE && HIWORD(wParam) == STN_CLICKED) {
                ShellExecuteW(NULL, L"open", L"https://coff.ee/lutischanf", NULL, NULL, SW_SHOWNORMAL);
                return TRUE;
            }
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                // Clean up resources and close dialog
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
        case WM_CLOSE:
            // Clean up resources
            if (hLinkFont) {
                DeleteObject(hLinkFont);
                hLinkFont = NULL;
            }
            if (hBrush) {
                DeleteObject(hBrush);
                hBrush = NULL;
            }
            if (uMsg == WM_CLOSE)
                EndDialog(hwndDlg, IDCANCEL);
            return TRUE;
    }
    return FALSE;
}

// Toast window procedure
LRESULT CALLBACK ToastWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // No auto-close timer: keep toast until user acts or clicks background
            return 0;
        }
        case WM_COMMAND:
            if (LOWORD(wParam) == ID_TOAST_ACTION && HIWORD(wParam) == BN_CLICKED) {
                // Button clicked: start the appropriate timer on the main window
                if (toast_is_pomodoro) {
                    // start break
                    is_in_pomodoro = 0; // now we're in break
                    // treat as long break if either the toast indicated it or the counter reached 4
                    if (toast_is_long_break || pomodoro_count >= 4) {
                        // do not reset here; keep the completed count until a new pomodoro starts
                        start_timer(g_main_hwnd, settings.long_break_duration, 1);
                     } else {
                             start_timer(g_main_hwnd, settings.short_break_duration, 0);
                     }
                 } else {
                     // start pomodoro
                     is_in_pomodoro = 1; // now we're in pomodoro
                     // starting a new pomodoro -> reset completed counter only if cycle complete
                     if (pomodoro_count >= 4) pomodoro_count = 0;
                     start_timer(g_main_hwnd, settings.pomodoro_duration, 0);
                 }
                 DestroyWindow(hwnd);
             } else if (LOWORD(wParam) == ID_TOAST_CLOSE && HIWORD(wParam) == BN_CLICKED) {
                 // Close button clicked
                 DestroyWindow(hwnd);
             } else if (LOWORD(wParam) == ID_TOAST_RESET && HIWORD(wParam) == BN_CLICKED) {
                 pomodoro_count = 0;
                 InvalidateRect(hwnd, NULL, TRUE);
                 if (is_running || is_paused) {
                     wchar_t display_text[16];
                     format_display_text(remaining_seconds, display_text,
                         (int)(sizeof(display_text) / sizeof(display_text[0])));
                     update_tray_icon(g_main_hwnd, display_text, pomodoro_count, remaining_seconds);
                 } else {
                     update_tray_icon(g_main_hwnd, L"\u25BA", pomodoro_count, 0);
                 }
             }
             return 0;
        case WM_DESTROY:
            g_hToastWnd = NULL;
            g_hToastButton = NULL;
            g_hToastCloseButton = NULL;
            return 0;
        case WM_LBUTTONDOWN:
            DestroyWindow(hwnd);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Draw reddish background (use same red as the tray icon)
            RECT rect;
            GetClientRect(hwnd, &rect);
            HBRUSH hBrush = CreateSolidBrush(RGB(139, 0, 0));
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);

            // Draw subtle border (darker)
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(100, 0, 0));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, 0, 0, rect.right, rect.bottom);
            SelectObject(hdc, hOldBrush);
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);

            // Draw progress dots (4) above the action button
            int totalDots = 4;
            int dotR = 6; // radius
            int spacing = 12;
            int dotDiameter = dotR * 2;
            int totalWidth = totalDots * dotDiameter + (totalDots - 1) * spacing;
            int startX = (rect.right - totalWidth) / 2;
            int dotsY = rect.bottom - 40 - 15 - 16; // above button area (btnH=40, gap=15, dots_radius_space=16)

            // Determine how many dots should be green: show up to 4 completed pomodoros
            int greenCount = pomodoro_count;
            if (greenCount < 0) greenCount = 0;
            if (greenCount > 4) greenCount = greenCount % 4; // fallback, but normally 0..4

            for (int i = 0; i < totalDots; i++) {
                int cx = startX + i * (dotDiameter + spacing);
                int left = cx;
                int top = dotsY - dotR;
                int right = cx + dotDiameter;
                int bottom = dotsY + dotR;

                HBRUSH fill = CreateSolidBrush((i < greenCount) ? RGB(144, 238, 144) : RGB(0, 0, 0));
                HBRUSH old = (HBRUSH)SelectObject(hdc, fill);
                Ellipse(hdc, left, top, right, bottom);
                SelectObject(hdc, old);
                DeleteObject(fill);
            }

            // Draw message text in white
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));
            HFONT hFont = CreateFontW(20, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                                     DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY, DEFAULT_PITCH, L"Segoe UI");
            HFONT hOldFont = hFont ? (HFONT)SelectObject(hdc, hFont) : NULL;

            RECT textRect = {15, 15, rect.right - 15, rect.bottom - 80};
            const wchar_t* message = (const wchar_t*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
            if (message) {
                DrawTextW(hdc, message, -1, &textRect, DT_LEFT | DT_WORDBREAK);
            }

            if (hOldFont) SelectObject(hdc, hOldFont);
            if (hFont) DeleteObject(hFont);

            EndPaint(hwnd, &ps);
            return 0;
        }
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

// Register toast window class
void RegisterToastWindowClass(void) {
    if (g_toast_class_registered) return;

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = ToastWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = TOAST_WINDOW_CLASS;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    if (RegisterClassW(&wc) || GetLastError() == ERROR_CLASS_ALREADY_EXISTS)
        g_toast_class_registered = TRUE;
    else
        OutputDebugStringW(L"Pomodoro Timer: failed to register toast window class.\n");
}

// Show completion notification as toast
void ShowCompletionNotification(HWND hwnd, int is_pomodoro_complete, int is_long_break) {
    const wchar_t* message;

    if (is_pomodoro_complete) {
        message = g_lang->notify_pomodoro_complete;
    } else if (is_long_break) {
        message = g_lang->notify_long_break_complete;
    } else {
        message = g_lang->notify_break_complete;
    }

    // Close previous toast if exists
    if (g_hToastWnd) {
        DestroyWindow(g_hToastWnd);
        g_hToastWnd = NULL;
    }

    // Register toast window class if not already done
    RegisterToastWindowClass();
    if (!g_toast_class_registered) {
        OutputDebugStringW(L"Pomodoro Timer: toast window class unavailable.\\n");
        return;
    }

    // Position the notification on the monitor containing the taskbar.
    HWND taskbarWnd = FindWindowW(L"Shell_TrayWnd", NULL);
    HMONITOR monitor = taskbarWnd
        ? MonitorFromWindow(taskbarWnd, MONITOR_DEFAULTTONEAREST)
        : MonitorFromPoint((POINT){GetSystemMetrics(SM_CXSCREEN) - 1, GetSystemMetrics(SM_CYSCREEN) - 1},
                           MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO monitorInfo = {0};
    monitorInfo.cbSize = sizeof(monitorInfo);
    RECT workArea = {0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
    if (monitor && GetMonitorInfoW(monitor, &monitorInfo))
        workArea = monitorInfo.rcWork;

    int toastWidth = 300;
    int toastHeight = 150;
    int margin = 12;
    int xPos = workArea.right - toastWidth - margin;
    int yPos = workArea.bottom - toastHeight - margin;
    if (xPos < workArea.left + margin) xPos = workArea.left + margin;
    if (yPos < workArea.top + margin) yPos = workArea.top + margin;

    // Create toast window with message in window title (for debugging)
    // Store message pointer for use in WM_PAINT
    static wchar_t toastMessage[256] = {0};
    int msgLen = wcslen(message);
    int maxLen = sizeof(toastMessage)/sizeof(toastMessage[0]) - 1;
    if (msgLen > maxLen) msgLen = maxLen;
    wcsncpy(toastMessage, message, msgLen);
    toastMessage[msgLen] = L'\0';

    g_hToastWnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW,
        TOAST_WINDOW_CLASS,
        L"Pomodoro Timer",
        WS_POPUP,
        xPos, yPos, toastWidth, toastHeight,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );

    if (g_hToastWnd) {
        // store toast state
        toast_is_pomodoro = is_pomodoro_complete;
        toast_is_long_break = is_long_break;

        // Store message in window user data for access in WM_PAINT
        SetWindowLongPtrW(g_hToastWnd, GWLP_USERDATA, (LONG_PTR)toastMessage);

        // Create action button (centered at bottom)
        int btnW = 200, btnH = 40;
        int btnX = (toastWidth - btnW) / 2;
        int btnY = toastHeight - btnH - 15;
        const wchar_t* btnText = is_pomodoro_complete ? g_lang->menu_start_break : g_lang->menu_start_pomodoro;
        g_hToastButton = CreateWindowW(L"BUTTON", btnText,
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            btnX, btnY, btnW, btnH,
            g_hToastWnd, (HMENU)ID_TOAST_ACTION, GetModuleHandle(NULL), NULL);
        if (!g_hToastButton) {
            DestroyWindow(g_hToastWnd);
            return;
        }

        // Set button font
        if (!g_hToastButtonFont) {
            g_hToastButtonFont = CreateFontW(17, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                     DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY, DEFAULT_PITCH, L"Segoe UI");
        }
        if (g_hToastButtonFont)
            SendMessageW(g_hToastButton, WM_SETFONT, (WPARAM)g_hToastButtonFont, TRUE);

        // Explicitly set button text (force correct label)
        SetWindowTextW(g_hToastButton, btnText);

        // Force repaint to update dots and button
        InvalidateRect(g_hToastWnd, NULL, TRUE);
        UpdateWindow(g_hToastWnd);

        // Create small close button in top-right corner
        int closeW = 22, closeH = 22;
        int closeX = toastWidth - closeW - 8;
        int closeY = 8;
        g_hToastCloseButton = CreateWindowW(L"BUTTON", L"✕",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER,
            closeX, closeY, closeW, closeH,
            g_hToastWnd, (HMENU)ID_TOAST_CLOSE, GetModuleHandle(NULL), NULL);
        if (g_hToastCloseButton) SendMessageW(g_hToastCloseButton, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

        // Create small reset button next to the dots
        int totalDots = 4;
        int dotR = 6;
        int spacing = 12;
        int dotDiameter = dotR * 2;
        int totalWidth = totalDots * dotDiameter + (totalDots - 1) * spacing;
        int dotsCenterY = toastHeight - 40 - 15 - 16;
        int dotsStartX = (toastWidth - totalWidth) / 2;

        int resetW = 22, resetH = 22;
        int resetX = dotsStartX + totalWidth + 15;
        int resetY = dotsCenterY - (resetH / 2);

        HWND hResetBtn = CreateWindowW(L"BUTTON", L"↺",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER,
            resetX, resetY, resetW, resetH,
            g_hToastWnd, (HMENU)ID_TOAST_RESET, GetModuleHandle(NULL), NULL);
        if (hResetBtn) SendMessageW(hResetBtn, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

        // Show as a notification without stealing keyboard focus.
        ShowWindow(g_hToastWnd, SW_SHOWNOACTIVATE);
        UpdateWindow(g_hToastWnd);
        SetWindowPos(g_hToastWnd, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    } else {
        OutputDebugStringW(L"ShowCompletionNotification: failed to create toast window\n");
    }
}

// Main window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_USER + 1: // Tray icon message
            if (LOWORD(lParam) == WM_LBUTTONUP) {
                // Left click pauses/resumes the active timer; otherwise it starts the next phase.
                if (is_running) {
                    pause_timer(hwnd);
                } else if (is_paused) {
                    resume_timer(hwnd);
                } else {
                    if (is_in_pomodoro) {
                        is_in_pomodoro = 0;
                        if (pomodoro_count == 4) {
                            start_timer(hwnd, settings.long_break_duration, 1);
                        } else {
                            start_timer(hwnd, settings.short_break_duration, 0);
                        }
                    } else {
                        is_in_pomodoro = 1;
                        if (pomodoro_count >= 4) pomodoro_count = 0;
                        start_timer(hwnd, settings.pomodoro_duration, 0);
                    }
                }
            } else if (LOWORD(lParam) == WM_RBUTTONUP) {
                // Right click: show context menu
                InitializeContextMenu();
                if (!g_hMenu) break;
                UpdateMenuCheckmarks();

                POINT pt;
                GetCursorPos(&pt);
                SetForegroundWindow(hwnd);
                int cmd = TrackPopupMenu(g_hMenu, TPM_RETURNCMD | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);

                // Handle menu commands
                switch (cmd) {
                    case 1: // Start Pomodoro
                        is_in_pomodoro = 1;
                        // user started a new pomodoro manually -> reset completed counter only if cycle complete
                        if (pomodoro_count >= 4) pomodoro_count = 0;
                        start_timer(hwnd, settings.pomodoro_duration, 0);
                        break;
                    case 2: // Start Break
                        is_in_pomodoro = 0;
                        start_timer(hwnd, settings.short_break_duration, 0);
                        break;
                    case 3: // Start Long Break
                        is_in_pomodoro = 0;
                        start_timer(hwnd, settings.long_break_duration, 1);
                        break;
                    case 4: // Toggle Clock Sound
                        settings.enable_clock_sound = !settings.enable_clock_sound;
                        if (is_running && settings.enable_clock_sound) start_clock_sound();
                        else if (!settings.enable_clock_sound) PlaySoundW(NULL, NULL, 0);
                        save_settings();
                        UpdateMenuCheckmarks();
                        break;
                    case 5: // Toggle Autostart
                        if (set_autostart(!autostart_enabled)) {
                            autostart_enabled = !autostart_enabled;
                        } else {
                            MessageBoxW(hwnd, L"Could not change the autostart setting.",
                                L"Pomodoro Timer", MB_OK | MB_ICONWARNING);
                        }
                        UpdateMenuCheckmarks();
                        break;
                    case 9: // Toggle Completion Dialog
                        settings.show_completion_dialog = !settings.show_completion_dialog;
                        save_settings();
                        UpdateMenuCheckmarks();
                        break;
                    case 6: // Settings
                        ShowSettingsDialog(hwnd);
                        break;
                    case 7: // About
                        ShowAboutDialog(hwnd);
                        break;
                    case ID_MENU_RESET_COUNT:
                        pomodoro_count = 0;
                        if (is_running) {
                             wchar_t display_text[16];
                             format_display_text(remaining_seconds, display_text, sizeof(display_text)/sizeof(display_text[0]));
                             update_tray_icon(hwnd, display_text, pomodoro_count, remaining_seconds);
                        } else {
                             update_tray_icon(hwnd, L"\u25BA", pomodoro_count, 0);
                        }
                        break;
                    case 8: // Exit
                        DestroyWindow(hwnd);
                        break;
                    case ID_MENU_LANG_EN:
                        SetLanguage(&lang_en);
                        SaveLanguageSelectionToRegistry();
                        break;
                    case ID_MENU_LANG_HU:
                        SetLanguage(&lang_hu);
                        SaveLanguageSelectionToRegistry();
                        break;
                    case ID_MENU_LANG_DE:
                        SetLanguage(&lang_de);
                        SaveLanguageSelectionToRegistry();
                        break;
                    case ID_MENU_LANG_IT:
                        SetLanguage(&lang_it);
                        SaveLanguageSelectionToRegistry();
                        break;
                    case ID_MENU_LANG_ES:
                        SetLanguage(&lang_es);
                        SaveLanguageSelectionToRegistry();
                        break;
                    case ID_MENU_LANG_FR:
                        SetLanguage(&lang_fr);
                        SaveLanguageSelectionToRegistry();
                        break;
                    case ID_MENU_LANG_RU:
                        SetLanguage(&lang_ru);
                        SaveLanguageSelectionToRegistry();
                        break;
                }
            }
            break;
        case WM_DESTROY:
            // Clean up before exit
            if (g_hToastWnd) DestroyWindow(g_hToastWnd);
            stop_timer(hwnd);
            if (g_hMenu) {
                DestroyMenu(g_hMenu);
                g_hMenu = NULL;
            }
            if (Shell_NotifyIconW(NIM_DELETE, &nid)) {
                g_tray_icon_removed = TRUE;
                HICON iconToRelease = g_shell_icon;
                if (g_shell_icon_owned && iconToRelease) DestroyIcon(iconToRelease);
                if (last_icon == iconToRelease) last_icon = NULL;
                g_shell_icon = NULL;
                g_shell_icon_owned = FALSE;
            }
            PostQuitMessage(0);
            break;
        case WM_TIMER:
            if (wParam == TIMER_ID) {
                handle_timer_tick(hwnd);
                return 0;
            }
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Helper function to parse JSON integer value by key
// Skip JSON whitespace.
static const char *json_skip_ws(const char *p) {
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') ++p;
    return p;
}

// Parse a JSON string token, including escaped characters, and return the position
// immediately after its closing quote. The caller only needs to compare simple keys.
static const char *json_skip_string(const char *p) {
    if (*p != '"') return NULL;
    ++p;
    while (*p) {
        if (*p == '\\') {
            if (!p[1]) return NULL;
            p += 2;
        } else if (*p == '"') {
            return p + 1;
        } else {
            ++p;
        }
    }
    return NULL;
}

// Parse a numeric property by matching JSON key tokens, not substrings in values.
static int parse_json_int(const char *json, const char *key, int default_value) {
    const char *p;
    size_t key_len;
    if (!json || !key) return default_value;
    key_len = strlen(key);
    p = json;

    while (*p) {
        if (*p != '"') {
            ++p;
            continue;
        }

        const char *string_end = json_skip_string(p);
        if (!string_end) return default_value;

        // Match only a plain, unescaped JSON property name.
        if ((size_t)(string_end - p) == key_len + 2 &&
            memcmp(p + 1, key, key_len) == 0) {
            const char *value_start = json_skip_ws(string_end);
            if (*value_start++ != ':') {
                p = string_end;
                continue;
            }
            value_start = json_skip_ws(value_start);

            errno = 0;
            char *value_end = NULL;
            long value = strtol(value_start, &value_end, 10);
            if (value_start == value_end || errno == ERANGE ||
                value < INT_MIN || value > INT_MAX)
                return default_value;

            value_end = (char *)json_skip_ws(value_end);
            if (*value_end != ',' && *value_end != '}')
                return default_value;
            return (int)value;
        }
        p = string_end;
    }
    return default_value;
}

// Load settings from file
void load_settings() {
    TimerSettings loaded = {25, 5, 15, 1, 1};
    FILE* fp = fopen("pomodoro_settings.json", "r");
    if (fp) {
        char buf[1024] = {0};
        size_t bytes_read = fread(buf, 1, sizeof(buf) - 1, fp);
        int file_too_large = fgetc(fp) != EOF;
        if (!ferror(fp) && !file_too_large) {
            buf[bytes_read] = '\0';
            loaded.pomodoro_duration = parse_json_int(buf, "pomodoro_duration", loaded.pomodoro_duration);
            loaded.short_break_duration = parse_json_int(buf, "short_break_duration", loaded.short_break_duration);
            loaded.long_break_duration = parse_json_int(buf, "long_break_duration", loaded.long_break_duration);
            loaded.enable_clock_sound = parse_json_int(buf, "enable_clock_sound", loaded.enable_clock_sound);
            loaded.show_completion_dialog = parse_json_int(buf, "show_completion_dialog", loaded.show_completion_dialog);
        }
        fclose(fp);
    }

    if (loaded.pomodoro_duration < 1 || loaded.pomodoro_duration > 120) loaded.pomodoro_duration = 25;
    if (loaded.short_break_duration < 1 || loaded.short_break_duration > 60) loaded.short_break_duration = 5;
    if (loaded.long_break_duration < 1 || loaded.long_break_duration > 120) loaded.long_break_duration = 15;
    if (loaded.enable_clock_sound != 0 && loaded.enable_clock_sound != 1) loaded.enable_clock_sound = 1;
    if (loaded.show_completion_dialog != 0 && loaded.show_completion_dialog != 1) loaded.show_completion_dialog = 1;
    settings = loaded;
}

// Save settings to file
void save_settings(void) {
    FILE *fp = fopen("pomodoro_settings.json", "w");
    if (!fp) {
        OutputDebugStringW(L"Pomodoro Timer: could not open settings file for writing.\n");
        return;
    }

    if (fprintf(fp,
        "{\"pomodoro_duration\":%d,\"short_break_duration\":%d,\"long_break_duration\":%d,\"enable_clock_sound\":%d,\"show_completion_dialog\":%d}",
        settings.pomodoro_duration, settings.short_break_duration, settings.long_break_duration,
        settings.enable_clock_sound, settings.show_completion_dialog) < 0) {
        OutputDebugStringW(L"Pomodoro Timer: failed to write settings file.\n");
    }
    if (fclose(fp) != 0)
        OutputDebugStringW(L"Pomodoro Timer: failed to close settings file.\n");
}

// Main entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SetLastError(ERROR_SUCCESS);
    HANDLE hMutex = CreateMutexW(NULL, TRUE, L"Local\\PomodoroTimerSingleInstance");
    if (!hMutex) {
        MessageBoxW(NULL, L"Could not create the single-instance mutex.", L"Pomodoro Timer",
            MB_OK | MB_ICONERROR);
        return 1;
    }
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(hMutex);
        MessageBoxW(NULL, L"The program is already running. Exiting.", L"Warning",
            MB_OK | MB_ICONWARNING);
        return 0;
    }

    // Initialize
    init_system_metrics();
    load_settings();
    autostart_enabled = is_autostart_enabled();

    // Initialize language
    if (!LoadLanguageSelectionFromRegistry()) {
        LANGID langId = GetUserDefaultUILanguage();
        switch (PRIMARYLANGID(langId)) {
            case LANG_ENGLISH: g_lang = &lang_en; break;
            case LANG_HUNGARIAN: g_lang = &lang_hu; break;
            case LANG_GERMAN: g_lang = &lang_de; break;
            case LANG_ITALIAN: g_lang = &lang_it; break;
            case LANG_SPANISH: g_lang = &lang_es; break;
            case LANG_FRENCH: g_lang = &lang_fr; break;
            case LANG_RUSSIAN: g_lang = &lang_ru; break;
            default: g_lang = &lang_en; break;
        }
        SaveLanguageSelectionToRegistry();
    }

    // Create window class
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Pomodoro";
    if (!RegisterClassW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
        MessageBoxW(NULL, L"Could not register the main window class.", L"Pomodoro Timer",
            MB_OK | MB_ICONERROR);
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
        return 1;
    }

    // Create invisible window
    HWND hwnd = CreateWindowW(L"Pomodoro", L"Pomodoro", 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
    if (!hwnd) {
        MessageBoxW(NULL, L"Could not create the main window.", L"Pomodoro Timer",
            MB_OK | MB_ICONERROR);
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
        return 1;
    }
    g_main_hwnd = hwnd;

    // Setup tray icon
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_USER + 1;
    nid.hIcon = LoadIconW(NULL, IDI_APPLICATION);
    g_shell_icon = nid.hIcon;
    g_shell_icon_owned = FALSE;
    wcsncpy(nid.szTip, g_lang->tooltip_pomodoro, sizeof(nid.szTip)/sizeof(nid.szTip[0]) - 1);
    nid.szTip[sizeof(nid.szTip)/sizeof(nid.szTip[0]) - 1] = L'\0';
    if (!Shell_NotifyIconW(NIM_ADD, &nid)) {
        MessageBoxW(hwnd, L"Could not add the tray icon. Check notification area settings.",
            L"Pomodoro Timer", MB_OK | MB_ICONERROR);
        DestroyWindow(hwnd);
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
        return 1;
    }

    // Show initial icon
    update_tray_icon(hwnd, L"\u25BA", pomodoro_count, 0);

    // Main message loop
    MSG msg;
    BOOL quitMessage = TRUE;
    int messageResult;
    while ((messageResult = (int)GetMessageW(&msg, NULL, 0, 0)) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    if (messageResult == -1) {
        OutputDebugStringW(L"Pomodoro Timer: GetMessageW failed.\n");
        quitMessage = FALSE;
    }

    // Clean up
    if (last_icon && last_icon != g_shell_icon) {
        DestroyIcon(last_icon);
        last_icon = NULL;
    }
    if (g_tray_icon_removed && g_shell_icon_owned && g_shell_icon) {
        DestroyIcon(g_shell_icon);
        g_shell_icon = NULL;
        g_shell_icon_owned = FALSE;
    }
    if (g_hToastButtonFont) {
        DeleteObject(g_hToastButtonFont);
        g_hToastButtonFont = NULL;
    }
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);

    return quitMessage ? 0 : 1;
}
