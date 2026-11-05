#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0601
#define __USE_MINGW_ANSI_STDIO 1
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
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
    WCHAR error_title[64];
    WCHAR warn_title[64];
    WCHAR error_start_timer[128];
    WCHAR error_already_running[128];
    WCHAR error_system[128];
} LANG;

static LANG lang_en = {
    L"Start Pomodoro", L"Start Break", L"Start Long Break",
    L"Clock Sound", L"Start on System Startup", L"Show Completion Dialog",
    L"Settings", L"Language", L"About", L"Exit",
    L"Reset Pomodoro Sessions",
    L"Click to start a pomodoro", L"Click to start a break",
    L"Pomodoro Settings", L"Pomodoro duration (minutes):",
    L"Short break duration (minutes):", L"Long break duration (minutes):",
    L"Enable clock sound", L"Start on system startup",
    L"Invalid time values! Must be positive and reasonable.",
    L"Pomodoro Complete!", L"Break Complete!", L"Long Break Complete!",
    L"Error", L"Warning",
    L"Could not start the timer.",
    L"The program is already running. Exiting.",
    L"A system error occurred."
};

static LANG lang_hu = {
    L"Pomodoro indítása", L"Szünet indítása", L"Hosszú szünet indítása",
    L"Óra hang", L"Indítás rendszerindításkor", L"Befejezési ablak megjelenítése",
    L"Beállítások", L"Nyelv", L"Névjegy", L"Kilépés",
    L"Pomodoro körök törlése",
    L"Kattints egy pomodoro indításához", L"Kattints egy szünet indításához",
    L"Pomodoro beállítások", L"Pomodoro időtartam (perc):",
    L"Rövid szünet időtartama (perc):", L"Hosszú szünet időtartama (perc):",
    L"Óra hang engedélyezése", L"Indítás rendszerindításkor",
    L"Érvénytelen időértékek! Pozitívnak és ésszerűnek kell lenniük.",
    L"Pomodoro kész!", L"Szünet kész!", L"Hosszú szünet kész!",
    L"Hiba", L"Figyelmeztetés",
    L"Nem sikerült elindítani az időzítőt.",
    L"A program már fut. Kilépés.",
    L"Rendszerhiba történt."
};

static LANG lang_de = {
    L"Pomodoro starten", L"Pause starten", L"Lange Pause starten",
    L"Uhrenton", L"Beim Systemstart starten", L"Abschlussdialog anzeigen",
    L"Einstellungen", L"Sprache", L"Über", L"Beenden",
    L"Pomodoro-Sitzungen zurücksetzen",
    L"Klicken Sie, um einen Pomodoro zu starten",
    L"Klicken Sie, um eine Pause zu starten",
    L"Pomodoro-Einstellungen", L"Pomodoro-Dauer (Minuten):",
    L"Kurze Pausendauer (Minuten):", L"Lange Pausendauer (Minuten):",
    L"Uhrenton aktivieren", L"Beim Systemstart starten",
    L"Ungültige Zeitwerte! Müssen positiv und angemessen sein.",
    L"Pomodoro abgeschlossen!", L"Pause abgeschlossen!",
    L"Lange Pause abgeschlossen!",
    L"Fehler", L"Warnung",
    L"Der Timer konnte nicht gestartet werden.",
    L"Das Programm läuft bereits. Beenden.",
    L"Ein Systemfehler ist aufgetreten."
};

static LANG lang_it = {
    L"Avvia Pomodoro", L"Avvia Pausa", L"Avvia Pausa Lunga",
    L"Suono Orologio", L"Avvia all'Avvio del Sistema",
    L"Mostra dialogo completamento",
    L"Impostazioni", L"Lingua", L"Info", L"Esci",
    L"Reimposta sessioni Pomodoro",
    L"Clicca per avviare un pomodoro", L"Clicca per avviare una pausa",
    L"Impostazioni Pomodoro", L"Durata pomodoro (minuti):",
    L"Durata pausa breve (minuti):", L"Durata pausa lunga (minuti):",
    L"Abilita suono orologio", L"Avvia all'avvio del sistema",
    L"Valori temporali non validi! Devono essere positivi e ragionevoli.",
    L"Pomodoro Completato!", L"Pausa Completata!",
    L"Pausa Lunga Completata!",
    L"Errore", L"Avviso",
    L"Impossibile avviare il timer.",
    L"Il programma è già in esecuzione. Uscita.",
    L"Si è verificato un errore di sistema."
};

static LANG lang_es = {
    L"Iniciar Pomodoro", L"Iniciar Descanso", L"Iniciar Descanso Largo",
    L"Sonido del Reloj", L"Iniciar con el Sistema",
    L"Mostrar diálogo finalización",
    L"Configuración", L"Idioma", L"Acerca de", L"Salir",
    L"Reiniciar sesiones Pomodoro",
    L"Clic para iniciar un pomodoro", L"Clic para iniciar un descanso",
    L"Configuración de Pomodoro", L"Duración del pomodoro (minutos):",
    L"Duración del descanso corto (minutos):",
    L"Duración del descanso largo (minutos):",
    L"Habilitar sonido del reloj", L"Iniciar con el sistema",
    L"¡Valores de tiempo inválidos! Deben ser positivos y razonables.",
    L"¡Pomodoro completado!", L"¡Descanso completado!",
    L"¡Descanso largo completado!",
    L"Error", L"Advertencia",
    L"No se pudo iniciar el temporizador.",
    L"El programa ya se está ejecutando. Saliendo.",
    L"Se produjo un error del sistema."
};

static LANG lang_fr = {
    L"Démarrer Pomodoro", L"Démarrer Pause", L"Démarrer Pause Longue",
    L"Son de l'Horloge", L"Démarrer avec le Système",
    L"Afficher dialogue de fin",
    L"Paramètres", L"Langue", L"À propos", L"Quitter",
    L"Réinitialiser sessions Pomodoro",
    L"Cliquez pour démarrer un pomodoro",
    L"Cliquez pour démarrer une pause",
    L"Paramètres Pomodoro", L"Durée pomodoro (minutes):",
    L"Durée pause courte (minutes):", L"Durée pause longue (minutes):",
    L"Activer le son de l'horloge", L"Démarrer avec le système",
    L"Valeurs de temps invalides! Doivent être positives et raisonnables.",
    L"Pomodoro Terminé!", L"Pause Terminée!", L"Pause Longue Terminée!",
    L"Erreur", L"Avertissement",
    L"Impossible de démarrer le minuteur.",
    L"Le programme est déjà en cours d'exécution. Fermeture.",
    L"Une erreur système s'est produite."
};

static LANG lang_ru = {
    L"Запустить Помодоро", L"Запустить Перерыв",
    L"Запустить Длинный Перерыв",
    L"Звук Часов", L"Запускать при Старте Системы",
    L"Показывать диалог завершения",
    L"Настройки", L"Язык", L"О программе", L"Выход",
    L"Сбросить сессии Помодоро",
    L"Нажмите для запуска помодоро", L"Нажмите для запуска перерыва",
    L"Настройки Помодоро", L"Длительность помодоро (минуты):",
    L"Длительность короткого перерыва (минуты):",
    L"Длительность длинного перерыва (минуты):",
    L"Включить звук часов", L"Запускать при старте системы",
    L"Недопустимые значения времени! Должны быть положительными и разумными.",
    L"Помодоро завершено!", L"Перерыв завершен!",
    L"Длинный перерыв завершен!",
    L"Ошибка", L"Предупреждение",
    L"Не удалось запустить таймер.",
    L"Программа уже запущена. Выход.",
    L"Произошла системная ошибка."
};

static LANG *g_lang = &lang_en;
static HMENU g_hLangMenu = NULL;
static HMENU g_hMenu = NULL;

typedef struct {
    int pomodoro_duration;
    int short_break_duration;
    int long_break_duration;
    int enable_clock_sound;
    int show_completion_dialog;
} TimerSettings;

TimerSettings settings = {25, 5, 15, 1, 1};
int pomodoro_count = 0;
int is_running = 0;
int is_in_pomodoro = 0;
int is_in_long_break = 0;
int remaining_seconds = 0;
NOTIFYICONDATA nid = {0};
static ULONGLONG timer_deadline = 0;
static int last_beeped_second = -1;
int autostart_enabled = 0;
static HICON last_icon = NULL;
static wchar_t last_text[16] = {0};
static int last_dots = -1;
static int screenWidth = 0, screenHeight = 0;
static HWND g_hToastWnd = NULL;
static HWND g_main_hwnd = NULL;
static int toast_is_pomodoro = 0;
static int toast_is_long_break = 0;
static HWND g_hToastButton = NULL;
static HWND g_hToastCloseButton = NULL;
static BOOL g_menu_initialized = FALSE;

/* MCI alapú óra-hang állapot */
static BOOL g_clock_mci_open = FALSE;
static char g_clock_temp_path[MAX_PATH] = {0};

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

void load_settings();
void save_settings();
void update_tray_icon(HWND hwnd, const wchar_t* text, int dots, int seconds);
void play_resource_sound(const char* resourceName);
static void start_clock_sound(void);
static void stop_clock_sound(void);
int is_autostart_enabled();
void RefreshMenuText(void);
void set_autostart(int enable);
INT_PTR CALLBACK SettingsDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AboutDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ShowSettingsDialog(HWND hwndParent);
void ShowAboutDialog(HWND hwndParent);
void ShowCompletionNotification(HWND hwnd, int is_pomodoro_complete, int is_long_break);
void format_display_text(int seconds, wchar_t* buffer, int buffer_size);
void InitializeContextMenu(void);
void UpdateMenuCheckmarks(void);

void init_system_metrics() {
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
}

DWORD GetLanguageIdFromLang(LANG *lang) {
    if (lang == &lang_en) return ID_MENU_LANG_EN;
    if (lang == &lang_hu) return ID_MENU_LANG_HU;
    if (lang == &lang_de) return ID_MENU_LANG_DE;
    if (lang == &lang_it) return ID_MENU_LANG_IT;
    if (lang == &lang_es) return ID_MENU_LANG_ES;
    if (lang == &lang_fr) return ID_MENU_LANG_FR;
    return ID_MENU_LANG_RU;
}

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

void format_display_text(int seconds, wchar_t* buffer, int buffer_size) {
    if (seconds < 60) {
        swprintf(buffer, (size_t)buffer_size, L"%d", seconds);
    } else {
        swprintf(buffer, (size_t)buffer_size, L"%d", seconds / 60);
    }
}

void InitializeContextMenu(void) {
    if (g_menu_initialized) return;
    g_hMenu = CreatePopupMenu();
    AppendMenuW(g_hMenu, MF_STRING, 1, g_lang->menu_start_pomodoro);
    AppendMenuW(g_hMenu, MF_STRING, 2, g_lang->menu_start_break);
    AppendMenuW(g_hMenu, MF_STRING, 3, g_lang->menu_start_long_break);
    AppendMenuW(g_hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(g_hMenu, MF_STRING, 4, g_lang->menu_clock_sound);
    AppendMenuW(g_hMenu, MF_STRING, 5, g_lang->menu_autostart);
    AppendMenuW(g_hMenu, MF_STRING, 9, g_lang->menu_show_dialog);
    AppendMenuW(g_hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(g_hMenu, MF_STRING, 6, g_lang->menu_settings);
    g_hLangMenu = CreatePopupMenu();
    AppendMenuW(g_hLangMenu, MF_STRING, ID_MENU_LANG_EN, L"English");
    AppendMenuW(g_hLangMenu, MF_STRING, ID_MENU_LANG_HU, L"Magyar");
    AppendMenuW(g_hLangMenu, MF_STRING, ID_MENU_LANG_DE, L"Deutsch");
    AppendMenuW(g_hLangMenu, MF_STRING, ID_MENU_LANG_IT, L"Italiano");
    AppendMenuW(g_hLangMenu, MF_STRING, ID_MENU_LANG_ES, L"Español");
    AppendMenuW(g_hLangMenu, MF_STRING, ID_MENU_LANG_FR, L"Français");
    AppendMenuW(g_hLangMenu, MF_STRING, ID_MENU_LANG_RU, L"Русский");
    AppendMenuW(g_hMenu, MF_POPUP, (UINT_PTR)g_hLangMenu, g_lang->menu_language);
    AppendMenuW(g_hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(g_hMenu, MF_STRING, 7, g_lang->menu_about);
    AppendMenuW(g_hMenu, MF_STRING, ID_MENU_RESET_COUNT, g_lang->menu_reset_count);
    AppendMenuW(g_hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(g_hMenu, MF_STRING, 8, g_lang->menu_exit);
    UpdateMenuCheckmarks();
    g_menu_initialized = TRUE;
}

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

void SetLanguage(LANG *newLang) {
    g_lang = newLang;
    RefreshMenuText();
}

void SaveLanguageSelectionToRegistry(void) {
    HKEY hKey;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\PomodoroTimer", 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        DWORD langId = GetLanguageIdFromLang(g_lang);
        RegSetValueExW(hKey, L"Language", 0, REG_DWORD, (const BYTE *)&langId, sizeof(DWORD));
        RegCloseKey(hKey);
    }
}

BOOL LoadLanguageSelectionFromRegistry(void) {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\PomodoroTimer", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD langId, dataSize = sizeof(DWORD);
        if (RegQueryValueExW(hKey, L"Language", NULL, NULL, (LPBYTE)&langId, &dataSize) == ERROR_SUCCESS) {
            g_lang = GetLangFromLanguageId(langId);
            RegCloseKey(hKey);
            return TRUE;
        }
        RegCloseKey(hKey);
    }
    return FALSE;
}

void RefreshMenuText(void) {
    if (!g_hMenu) return;
    ModifyMenuW(g_hMenu, 1, MF_BYCOMMAND | MF_STRING, 1, g_lang->menu_start_pomodoro);
    ModifyMenuW(g_hMenu, 2, MF_BYCOMMAND | MF_STRING, 2, g_lang->menu_start_break);
    ModifyMenuW(g_hMenu, 3, MF_BYCOMMAND | MF_STRING, 3, g_lang->menu_start_long_break);
    ModifyMenuW(g_hMenu, 4, MF_BYCOMMAND | MF_STRING, 4, g_lang->menu_clock_sound);
    ModifyMenuW(g_hMenu, 5, MF_BYCOMMAND | MF_STRING, 5, g_lang->menu_autostart);
    ModifyMenuW(g_hMenu, 9, MF_BYCOMMAND | MF_STRING, 9, g_lang->menu_show_dialog);
    ModifyMenuW(g_hMenu, 6, MF_BYCOMMAND | MF_STRING, 6, g_lang->menu_settings);
    ModifyMenuW(g_hMenu, 7, MF_BYCOMMAND | MF_STRING, 7, g_lang->menu_about);
    ModifyMenuW(g_hMenu, ID_MENU_RESET_COUNT, MF_BYCOMMAND | MF_STRING, ID_MENU_RESET_COUNT, g_lang->menu_reset_count);
    ModifyMenuW(g_hMenu, 8, MF_BYCOMMAND | MF_STRING, 8, g_lang->menu_exit);
    ModifyMenuW(g_hMenu, (UINT_PTR)g_hLangMenu, MF_BYCOMMAND | MF_POPUP, (UINT_PTR)g_hLangMenu, g_lang->menu_language);
    UpdateMenuCheckmarks();
}

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
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hdc, hBitmap);

    HBRUSH bgBrush = CreateSolidBrush(RGB(139, 0, 0));
    RECT rect = {0, 0, 32, 32};
    FillRect(hdc, &rect, bgBrush);
    DeleteObject(bgBrush);

    HFONT hFont = CreateFontW(27, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    SIZE textSize;
    GetTextExtentPoint32W(hdc, text, (int)wcslen(text), &textSize);
    int textX = (32 - textSize.cx) / 2;
    int textY = (32 - textSize.cy) / 2 - 2;
    if (wcscmp(text, L"\u25BA") == 0) {
        textX += 2;
    }
    TextOutW(hdc, textX, textY, text, (int)wcslen(text));

    HBRUSH dotBrush = CreateSolidBrush(RGB(144, 238, 144));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, dotBrush);
    for (int i = 0; i < dots; i++) {
        int dotX = 4 + i * 8;
        int dotY = 30;
        Ellipse(hdc, dotX - 3, dotY - 3, dotX + 3, dotY + 3);
    }
    SelectObject(hdc, oldBrush);
    DeleteObject(dotBrush);

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);

    HBITMAP hMask = CreateCompatibleBitmap(hdc, 32, 32);
    HDC hdcMask = CreateCompatibleDC(hdc);
    HBITMAP hOldMaskBmp = (HBITMAP)SelectObject(hdcMask, hMask);
    SetBkColor(hdc, RGB(139, 0, 0));
    BitBlt(hdcMask, 0, 0, 32, 32, hdc, 0, 0, SRCCOPY);
    SelectObject(hdcMask, hOldMaskBmp);

    ICONINFO iconInfo = {0};
    iconInfo.fIcon = TRUE;
    iconInfo.hbmColor = hBitmap;
    iconInfo.hbmMask = hMask;
    HICON hIcon = CreateIconIndirect(&iconInfo);

    SelectObject(hdc, hOldBmp);
    DeleteDC(hdcMask);
    DeleteDC(hdc);
    DeleteObject(hBitmap);
    DeleteObject(hMask);

    last_icon = hIcon;
    wcsncpy(last_text, text, sizeof(last_text)/sizeof(last_text[0]) - 1);
    last_text[sizeof(last_text)/sizeof(last_text[0]) - 1] = L'\0';
    last_dots = dots;
    return hIcon;
}

void update_tray_icon(HWND hwnd, const wchar_t* text, int dots, int seconds) {
    (void)hwnd;
    wchar_t tooltip[128];
    if (seconds > 0) {
        int min = seconds / 60, sec = seconds % 60;
        swprintf(tooltip, sizeof(tooltip)/sizeof(tooltip[0]), L"%02d:%02d", min, sec);
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
    nid.hIcon = create_tray_icon(text, dots);
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

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

/* Kibontja a CLOCK_WAV-ot egy temp fájlba, amit az MCI waveaudio meg tud nyitni. */
static BOOL ensure_clock_temp_file(void) {
    if (g_clock_temp_path[0] && GetFileAttributesA(g_clock_temp_path) != INVALID_FILE_ATTRIBUTES) {
        return TRUE;
    }
    char temp_dir[MAX_PATH];
    if (!GetTempPathA(MAX_PATH, temp_dir)) return FALSE;
    if (strlen(temp_dir) + 32 >= MAX_PATH) return FALSE;
    snprintf(g_clock_temp_path, sizeof(g_clock_temp_path), "%spomodoro_clock.wav", temp_dir);

    HRSRC hRes = FindResourceA(NULL, "CLOCK_WAV", "WAV");
    if (!hRes) { g_clock_temp_path[0] = 0; return FALSE; }
    HGLOBAL hData = LoadResource(NULL, hRes);
    if (!hData) { g_clock_temp_path[0] = 0; return FALSE; }
    DWORD size = SizeofResource(NULL, hRes);
    LPVOID pData = LockResource(hData);
    if (!pData) { g_clock_temp_path[0] = 0; return FALSE; }

    FILE* f = fopen(g_clock_temp_path, "wb");
    if (!f) { g_clock_temp_path[0] = 0; return FALSE; }
    fwrite(pData, 1, size, f);
    fclose(f);
    return TRUE;
}

/* MCI-vel indítja az óra-hangot. A waveaudio eszköz nem támogatja a "repeat"
   kapcsolót, ezért "notify"-jal kérünk értesítést a lejátszás végén
   (MM_MCINOTIFY), és ott indítjuk újra (lásd WndProc). */
static void start_clock_sound(void) {
    if (!settings.enable_clock_sound) return;
    if (g_clock_mci_open) return;
    if (!ensure_clock_temp_file()) return;

    char cmd[512];
    snprintf(cmd, sizeof(cmd), "open \"%s\" type waveaudio alias pclock", g_clock_temp_path);
    if (mciSendStringA(cmd, NULL, 0, NULL) != 0) return;

    if (mciSendStringA("play pclock from 0 notify", NULL, 0, g_main_hwnd) != 0) {
        mciSendStringA("close pclock", NULL, 0, NULL);
        return;
    }
    g_clock_mci_open = TRUE;
}

/* MCI-vel leállítja az óra-hangot. A close megszakítja a lejátszást. */
static void stop_clock_sound(void) {
    if (g_clock_mci_open) {
        g_clock_mci_open = FALSE; /* előbb a flag, hogy a notify ne indítsa újra */
        mciSendStringA("close pclock", NULL, 0, NULL);
    }
    /* Biztonsági háló: ha korábbi verzió PlaySound-dal indította, az is álljon le. */
    PlaySoundA(NULL, NULL, 0);
}

static void stop_timer(HWND hwnd) {
    is_running = 0;
    KillTimer(hwnd, TIMER_ID);
    timer_deadline = 0;
    remaining_seconds = 0;
    stop_clock_sound();
}

void start_timer(HWND hwnd, int duration_minutes, int is_long_break) {
    if (g_hToastWnd) {
        HWND toast = g_hToastWnd;
        g_hToastWnd = NULL;
        DestroyWindow(toast);
    }
    if (is_running) {
        stop_timer(hwnd);
    }
    remaining_seconds = duration_minutes * 60;
    timer_deadline = GetTickCount64() + (ULONGLONG)remaining_seconds * 1000;
    last_beeped_second = -1;
    is_running = 1;
    is_in_long_break = is_long_break;
    if (!SetTimer(hwnd, TIMER_ID, TIMER_INTERVAL_MS, NULL)) {
        stop_timer(hwnd);
        MessageBoxW(hwnd, g_lang->error_start_timer, g_lang->error_title, MB_OK | MB_ICONERROR);
        update_tray_icon(hwnd, L"\u25BA", pomodoro_count, 0);
        return;
    }
    start_clock_sound();
    wchar_t display_text[16];
    format_display_text(remaining_seconds, display_text, sizeof(display_text) / sizeof(display_text[0]));
    update_tray_icon(hwnd, display_text, pomodoro_count, remaining_seconds);
}

static void handle_timer_tick(HWND hwnd) {
    if (!is_running) return;
    ULONGLONG now = GetTickCount64();
    if (now >= timer_deadline) {
        int was_pomodoro = is_in_pomodoro;
        int is_long_break = is_in_long_break;
        stop_timer(hwnd);
        is_in_long_break = 0;
        if (was_pomodoro) {
            if (pomodoro_count < 4) pomodoro_count++;
            is_long_break = pomodoro_count == 4;
            is_in_long_break = is_long_break;
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

void set_autostart(int enable) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        if (enable) {
            char path[MAX_PATH];
            GetModuleFileNameA(NULL, path, MAX_PATH);
            RegSetValueExA(hKey, "PomodoroTimer", 0, REG_SZ, (BYTE*)path, (DWORD)(strlen(path) + 1));
        } else {
            RegDeleteValueA(hKey, "PomodoroTimer");
        }
        RegCloseKey(hKey);
    }
}

void ShowSettingsDialog(HWND hwndParent) {
    DialogBoxW(GetModuleHandle(NULL), MAKEINTRESOURCEW(IDD_SETTINGS), hwndParent, SettingsDlgProc);
}

INT_PTR CALLBACK SettingsDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    (void)lParam;
    switch (uMsg) {
        case WM_INITDIALOG: {
            SetWindowTextW(hwndDlg, g_lang->settings_title);
            SetDlgItemTextW(hwndDlg, 201, g_lang->settings_pomodoro);
            SetDlgItemTextW(hwndDlg, 202, g_lang->settings_short_break);
            SetDlgItemTextW(hwndDlg, 203, g_lang->settings_long_break);
            SetDlgItemTextW(hwndDlg, IDC_CLOCK_SOUND, g_lang->settings_enable_sound);
            SetDlgItemTextW(hwndDlg, IDC_AUTOSTART, g_lang->settings_autostart);
            char buf[16];
            snprintf(buf, sizeof(buf), "%d", settings.pomodoro_duration);
            SetDlgItemTextA(hwndDlg, IDC_POMODORO, buf);
            snprintf(buf, sizeof(buf), "%d", settings.short_break_duration);
            SetDlgItemTextA(hwndDlg, IDC_SHORT_BREAK, buf);
            snprintf(buf, sizeof(buf), "%d", settings.long_break_duration);
            SetDlgItemTextA(hwndDlg, IDC_LONG_BREAK, buf);
            SendDlgItemMessageA(hwndDlg, IDC_CLOCK_SOUND, BM_SETCHECK, settings.enable_clock_sound ? BST_CHECKED : BST_UNCHECKED, 0);
            SendDlgItemMessageA(hwndDlg, IDC_AUTOSTART, BM_SETCHECK, autostart_enabled ? BST_CHECKED : BST_UNCHECKED, 0);
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
                    if (pomodoro > 0 && pomodoro <= 120 &&
                        short_break > 0 && short_break <= 60 &&
                        long_break > 0 && long_break <= 120) {

                        int old_clock_sound = settings.enable_clock_sound;

                        settings.pomodoro_duration = pomodoro;
                        settings.short_break_duration = short_break;
                        settings.long_break_duration = long_break;
                        settings.enable_clock_sound = SendDlgItemMessageA(hwndDlg, IDC_CLOCK_SOUND, BM_GETCHECK, 0, 0) == BST_CHECKED;
                        autostart_enabled = SendDlgItemMessageA(hwndDlg, IDC_AUTOSTART, BM_GETCHECK, 0, 0) == BST_CHECKED;
                        set_autostart(autostart_enabled);
                        save_settings();

                        if (old_clock_sound != settings.enable_clock_sound) {
                            if (!settings.enable_clock_sound) {
                                stop_clock_sound();
                            } else if (is_running) {
                                start_clock_sound();
                            }
                        }

                        EndDialog(hwndDlg, IDOK);
                    } else {
                        MessageBoxW(hwndDlg, g_lang->error_invalid_time, g_lang->error_title, MB_ICONERROR);
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

void ShowAboutDialog(HWND hwndParent) {
    DialogBoxW(GetModuleHandle(NULL), MAKEINTRESOURCEW(IDD_ABOUT), hwndParent, AboutDlgProc);
}

INT_PTR CALLBACK AboutDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HFONT hLinkFont = NULL;
    static HBRUSH hBrush = NULL;
    switch (uMsg) {
        case WM_INITDIALOG: {
            HFONT hFont = (HFONT)SendDlgItemMessageW(hwndDlg, IDC_WEBSITE, WM_GETFONT, 0, 0);
            LOGFONTW lf;
            GetObjectW(hFont, sizeof(LOGFONTW), &lf);
            lf.lfUnderline = TRUE;
            hLinkFont = CreateFontIndirectW(&lf);
            SendDlgItemMessageW(hwndDlg, IDC_WEBSITE, WM_SETFONT, (WPARAM)hLinkFont, TRUE);
            SendDlgItemMessageW(hwndDlg, IDC_COFFEE, WM_SETFONT, (WPARAM)hLinkFont, TRUE);
            SetDlgItemTextW(hwndDlg, IDC_WEBSITE, L"Visit our website");
            hBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
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
                return (INT_PTR)hBrush;
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
                if (hLinkFont) { DeleteObject(hLinkFont); hLinkFont = NULL; }
                if (hBrush) { DeleteObject(hBrush); hBrush = NULL; }
                EndDialog(hwndDlg, LOWORD(wParam));
                return TRUE;
            }
            break;
        case WM_DESTROY:
        case WM_CLOSE:
            if (hLinkFont) { DeleteObject(hLinkFont); hLinkFont = NULL; }
            if (hBrush) { DeleteObject(hBrush); hBrush = NULL; }
            if (uMsg == WM_CLOSE)
                EndDialog(hwndDlg, IDCANCEL);
            return TRUE;
    }
    return FALSE;
}

LRESULT CALLBACK ToastWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            return 0;
        case WM_COMMAND:
            if (LOWORD(wParam) == ID_TOAST_ACTION && HIWORD(wParam) == BN_CLICKED) {
                int saved_pomodoro = toast_is_pomodoro;
                int saved_long_break = toast_is_long_break;
                DestroyWindow(hwnd);
                if (saved_pomodoro) {
                    is_in_pomodoro = 0;
                    if (saved_long_break || pomodoro_count >= 4) {
                        start_timer(g_main_hwnd, settings.long_break_duration, 1);
                    } else {
                        start_timer(g_main_hwnd, settings.short_break_duration, 0);
                    }
                } else {
                    is_in_pomodoro = 1;
                    if (pomodoro_count >= 4) pomodoro_count = 0;
                    start_timer(g_main_hwnd, settings.pomodoro_duration, 0);
                }
            } else if (LOWORD(wParam) == ID_TOAST_CLOSE && HIWORD(wParam) == BN_CLICKED) {
                DestroyWindow(hwnd);
            } else if (LOWORD(wParam) == ID_TOAST_RESET && HIWORD(wParam) == BN_CLICKED) {
                pomodoro_count = 0;
                InvalidateRect(hwnd, NULL, TRUE);
                if (is_running) {
                    wchar_t display_text[16];
                    format_display_text(remaining_seconds, display_text, sizeof(display_text)/sizeof(display_text[0]));
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
            RECT rect;
            GetClientRect(hwnd, &rect);
            HBRUSH hBrush = CreateSolidBrush(RGB(139, 0, 0));
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(100, 0, 0));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, 0, 0, rect.right, rect.bottom);
            SelectObject(hdc, hOldBrush);
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
            int totalDots = 4;
            int dotR = 6;
            int spacing = 12;
            int dotDiameter = dotR * 2;
            int totalWidth = totalDots * dotDiameter + (totalDots - 1) * spacing;
            int startX = (rect.right - totalWidth) / 2;
            int dotsY = rect.bottom - 40 - 15 - 16;
            int greenCount = pomodoro_count;
            if (greenCount < 0) greenCount = 0;
            if (greenCount > 4) greenCount = greenCount % 4;
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
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));
            HFONT hFont = CreateFontW(20, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                                     DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY, DEFAULT_PITCH, L"Segoe UI");
            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
            RECT textRect = {15, 15, rect.right - 15, rect.bottom - 80};
            const wchar_t* message = (const wchar_t*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
            if (message) {
                DrawTextW(hdc, message, -1, &textRect, DT_LEFT | DT_WORDBREAK);
            }
            SelectObject(hdc, hOldFont);
            DeleteObject(hFont);
            EndPaint(hwnd, &ps);
            return 0;
        }
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

void RegisterToastWindowClass(void) {
    static BOOL registered = FALSE;
    if (registered) return;
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = ToastWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = TOAST_WINDOW_CLASS;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    if (RegisterClassW(&wc)) registered = TRUE;
}

void ShowCompletionNotification(HWND hwnd, int is_pomodoro_complete, int is_long_break) {
    (void)hwnd;
    const wchar_t* message;
    if (is_pomodoro_complete) {
        message = g_lang->notify_pomodoro_complete;
    } else if (is_long_break) {
        message = g_lang->notify_long_break_complete;
    } else {
        message = g_lang->notify_break_complete;
    }
    if (g_hToastWnd) {
        HWND toast = g_hToastWnd;
        g_hToastWnd = NULL;
        DestroyWindow(toast);
    }
    RegisterToastWindowClass();
    RECT taskbarRect = {0};
    HWND taskbarWnd = FindWindowW(L"Shell_TrayWnd", NULL);
    if (taskbarWnd) {
        GetWindowRect(taskbarWnd, &taskbarRect);
    }
    int toastWidth = 300;
    int toastHeight = 150;
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    int xPos = sw - toastWidth - 20;
    int yPos = taskbarRect.top - toastHeight - 20;
    if (yPos < 0) {
        yPos = sh - toastHeight - 100;
    }
    static wchar_t toastMessage[256] = {0};
    int msgLen = (int)wcslen(message);
    int maxLen = sizeof(toastMessage)/sizeof(toastMessage[0]) - 1;
    if (msgLen > maxLen) msgLen = maxLen;
    wcsncpy(toastMessage, message, msgLen);
    toastMessage[msgLen] = L'\0';
    g_hToastWnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW,
        TOAST_WINDOW_CLASS, L"Pomodoro Timer", WS_POPUP,
        xPos, yPos, toastWidth, toastHeight,
        NULL, NULL, GetModuleHandle(NULL), NULL);
    if (g_hToastWnd) {
        toast_is_pomodoro = is_pomodoro_complete;
        toast_is_long_break = is_long_break;
        SetWindowLongPtrW(g_hToastWnd, GWLP_USERDATA, (LONG_PTR)toastMessage);
        int btnW = 200, btnH = 40;
        int btnX = (toastWidth - btnW) / 2;
        int btnY = toastHeight - btnH - 15;
        const wchar_t* btnText;
        if (is_pomodoro_complete) {
            btnText = is_long_break ? g_lang->menu_start_long_break : g_lang->menu_start_break;
        } else {
            btnText = g_lang->menu_start_pomodoro;
        }
        g_hToastButton = CreateWindowW(L"BUTTON", btnText,
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            btnX, btnY, btnW, btnH,
            g_hToastWnd, (HMENU)ID_TOAST_ACTION, GetModuleHandle(NULL), NULL);
        static HFONT hBtnFont = NULL;
        if (!hBtnFont) {
            hBtnFont = CreateFontW(17, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                     DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY, DEFAULT_PITCH, L"Segoe UI");
        }
        SendMessageW(g_hToastButton, WM_SETFONT, (WPARAM)hBtnFont, TRUE);
        SetWindowTextW(g_hToastButton, btnText);
        InvalidateRect(g_hToastWnd, NULL, TRUE);
        UpdateWindow(g_hToastWnd);

        static HFONT hSymFont = NULL;
        if (!hSymFont) {
            hSymFont = CreateFontW(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH, L"Segoe UI Symbol");
        }

        int closeW = 22, closeH = 22;
        int closeX = toastWidth - closeW - 8;
        int closeY = 8;
        g_hToastCloseButton = CreateWindowW(L"BUTTON", L"\u2715",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER,
            closeX, closeY, closeW, closeH,
            g_hToastWnd, (HMENU)ID_TOAST_CLOSE, GetModuleHandle(NULL), NULL);
        SendMessageW(g_hToastCloseButton, WM_SETFONT, (WPARAM)hSymFont, TRUE);

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
        HWND hResetBtn = CreateWindowW(L"BUTTON", L"\u21BA",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER,
            resetX, resetY, resetW, resetH,
            g_hToastWnd, (HMENU)ID_TOAST_RESET, GetModuleHandle(NULL), NULL);
        SendMessageW(hResetBtn, WM_SETFONT, (WPARAM)hSymFont, TRUE);

        ShowWindow(g_hToastWnd, SW_SHOW);
        UpdateWindow(g_hToastWnd);
        SetWindowPos(g_hToastWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        AllowSetForegroundWindow(ASFW_ANY);
        SetForegroundWindow(g_hToastWnd);
    } else {
        OutputDebugStringW(L"ShowCompletionNotification: failed to create toast window\n");
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_USER + 1:
            if (LOWORD(lParam) == WM_LBUTTONUP) {
                if (is_running) {
                    stop_timer(hwnd);
                    update_tray_icon(hwnd, L"\u25BA", pomodoro_count, 0);
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
                InitializeContextMenu();
                UpdateMenuCheckmarks();
                POINT pt;
                GetCursorPos(&pt);
                SetForegroundWindow(hwnd);
                int cmd = TrackPopupMenu(g_hMenu, TPM_RETURNCMD, pt.x, pt.y, 0, hwnd, NULL);
                switch (cmd) {
                    case 1:
                        is_in_pomodoro = 1;
                        if (pomodoro_count >= 4) pomodoro_count = 0;
                        start_timer(hwnd, settings.pomodoro_duration, 0);
                        break;
                    case 2:
                        is_in_pomodoro = 0;
                        start_timer(hwnd, settings.short_break_duration, 0);
                        break;
                    case 3:
                        is_in_pomodoro = 0;
                        start_timer(hwnd, settings.long_break_duration, 1);
                        break;
                    case 4: /* Toggle Clock Sound */
                        settings.enable_clock_sound = !settings.enable_clock_sound;
                        save_settings();
                        if (!settings.enable_clock_sound) {
                            stop_clock_sound();
                        } else if (is_running) {
                            start_clock_sound();
                        }
                        UpdateMenuCheckmarks();
                        break;
                    case 5:
                        autostart_enabled = !autostart_enabled;
                        set_autostart(autostart_enabled);
                        UpdateMenuCheckmarks();
                        break;
                    case 9:
                        settings.show_completion_dialog = !settings.show_completion_dialog;
                        save_settings();
                        UpdateMenuCheckmarks();
                        break;
                    case 6:
                        ShowSettingsDialog(hwnd);
                        break;
                    case 7:
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
                    case 8:
                        DestroyWindow(hwnd);
                        break;
                    case ID_MENU_LANG_EN: SetLanguage(&lang_en); SaveLanguageSelectionToRegistry(); break;
                    case ID_MENU_LANG_HU: SetLanguage(&lang_hu); SaveLanguageSelectionToRegistry(); break;
                    case ID_MENU_LANG_DE: SetLanguage(&lang_de); SaveLanguageSelectionToRegistry(); break;
                    case ID_MENU_LANG_IT: SetLanguage(&lang_it); SaveLanguageSelectionToRegistry(); break;
                    case ID_MENU_LANG_ES: SetLanguage(&lang_es); SaveLanguageSelectionToRegistry(); break;
                    case ID_MENU_LANG_FR: SetLanguage(&lang_fr); SaveLanguageSelectionToRegistry(); break;
                    case ID_MENU_LANG_RU: SetLanguage(&lang_ru); SaveLanguageSelectionToRegistry(); break;
                }
            }
            break;
        case MM_MCINOTIFY:
            /* A waveaudio nem tud repeat-et: a hang végén újraindítjuk.
               Megszakításnál (close) a wParam MCI_NOTIFY_ABORTED, azt kiszűrjük. */
            if (wParam == MCI_NOTIFY_SUCCESSFUL && g_clock_mci_open &&
                is_running && settings.enable_clock_sound) {
                mciSendStringA("play pclock from 0 notify", NULL, 0, hwnd);
            }
            return 0;
        case WM_DESTROY:
            stop_clock_sound();
            if (g_clock_temp_path[0]) {
                DeleteFileA(g_clock_temp_path);
                g_clock_temp_path[0] = 0;
            }
            if (g_hToastWnd) {
                HWND toast = g_hToastWnd;
                g_hToastWnd = NULL;
                DestroyWindow(toast);
            }
            stop_timer(hwnd);
            if (g_hMenu) {
                DestroyMenu(g_hMenu);
                g_hMenu = NULL;
                g_hLangMenu = NULL;
            }
            Shell_NotifyIcon(NIM_DELETE, &nid);
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

static int parse_json_int(const char* json, const char* key, int default_value) {
    char search_pattern[64];
    snprintf(search_pattern, sizeof(search_pattern), "\"%s\"", key);
    const char* key_pos = strstr(json, search_pattern);
    if (!key_pos) return default_value;
    const char* value_start = key_pos + strlen(search_pattern);
    while (*value_start == ' ' || *value_start == '\t' || *value_start == '\n' || *value_start == '\r') {
        value_start++;
    }
    if (*value_start++ != ':') return default_value;
    while (*value_start == ' ' || *value_start == '\t' || *value_start == '\n' || *value_start == '\r') {
        value_start++;
    }
    errno = 0;
    char* value_end;
    long value = strtol(value_start, &value_end, 10);
    if (value_start == value_end || errno == ERANGE || value < INT_MIN || value > INT_MAX) {
        return default_value;
    }
    while (*value_end == ' ' || *value_end == '\t' || *value_end == '\n' || *value_end == '\r') {
        value_end++;
    }
    if (*value_end != ',' && *value_end != '}') return default_value;
    return (int)value;
}

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

void save_settings() {
    FILE* fp = fopen("pomodoro_settings.json", "w");
    if (fp) {
        fprintf(fp, "{\"pomodoro_duration\":%d,\"short_break_duration\":%d,\"long_break_duration\":%d,\"enable_clock_sound\":%d,\"show_completion_dialog\":%d}",
                settings.pomodoro_duration, settings.short_break_duration, settings.long_break_duration, settings.enable_clock_sound, settings.show_completion_dialog);
        fclose(fp);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    init_system_metrics();
    load_settings();

    if (!LoadLanguageSelectionFromRegistry()) {
        LANGID langId = GetUserDefaultUILanguage();
        switch (langId) {
            case 0x0409: g_lang = &lang_en; break;
            case 0x040e: g_lang = &lang_hu; break;
            case 0x0407: g_lang = &lang_de; break;
            case 0x0410: g_lang = &lang_it; break;
            case 0x0c0a: g_lang = &lang_es; break;
            case 0x040c: g_lang = &lang_fr; break;
            case 0x0419: g_lang = &lang_ru; break;
            default: g_lang = &lang_en; break;
        }
        SaveLanguageSelectionToRegistry();
    }
    autostart_enabled = is_autostart_enabled();

    HANDLE hEvent = CreateEventW(NULL, TRUE, FALSE, L"PomodoroTimerEvent");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBoxW(NULL, g_lang->error_already_running, g_lang->warn_title, MB_OK | MB_ICONWARNING);
        if (hEvent) CloseHandle(hEvent);
        return 0;
    }

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Pomodoro";
    if (!RegisterClassW(&wc)) {
        MessageBoxW(NULL, g_lang->error_system, g_lang->error_title, MB_OK | MB_ICONERROR);
        if (hEvent) CloseHandle(hEvent);
        return 1;
    }
    HWND hwnd = CreateWindowW(L"Pomodoro", L"Pomodoro", 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
    if (!hwnd) {
        MessageBoxW(NULL, g_lang->error_system, g_lang->error_title, MB_OK | MB_ICONERROR);
        if (hEvent) CloseHandle(hEvent);
        return 1;
    }
    g_main_hwnd = hwnd;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_USER + 1;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcsncpy(nid.szTip, g_lang->tooltip_pomodoro, sizeof(nid.szTip)/sizeof(nid.szTip[0]) - 1);
    nid.szTip[sizeof(nid.szTip)/sizeof(nid.szTip[0]) - 1] = L'\0';
    if (!Shell_NotifyIcon(NIM_ADD, &nid)) {
        MessageBoxW(NULL, g_lang->error_system, g_lang->error_title, MB_OK | MB_ICONERROR);
        DestroyWindow(hwnd);
        if (hEvent) CloseHandle(hEvent);
        return 1;
    }
    update_tray_icon(hwnd, L"\u25BA", pomodoro_count, 0);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    if (last_icon) {
        DestroyIcon(last_icon);
        last_icon = NULL;
    }
    if (hEvent) CloseHandle(hEvent);
    return 0;
}