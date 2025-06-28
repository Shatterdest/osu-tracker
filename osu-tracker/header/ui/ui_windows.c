/* nuklear - 1.32.0 - public domain */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>

#define MAX_ENTRIES 100

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 600

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_GDI_IMPLEMENTATION

enum gameMode {
    osu = 0
    , taiko = 1
    , fruits = 2
    , mania = 3
};

enum server {
    bancho = 0
    , titanic = 1
};


struct appC {
    int osuId;
    enum gameMode gameMode;
    enum server server;
};

struct userC {
    const char* username;
    const char* avatar;
};

struct dataEntryC {
    const char* key;
    const char* name;
    int sort;
    const char* init;
    const char* current;
    const char* change;
    bool positive;
    bool display;
    bool single;
    bool banchoSupport;
    bool titanicSupport;
};

// Internal storage
static struct appC _app;
static struct userC _user;
static struct dataEntryC _entries[MAX_ENTRIES];
static size_t _entry_count = 0;

// Track allocated strings for cleanup
static char* _user_str_copies[2] = { NULL, NULL }; // username, avatar
static char* _entries_str_copies[MAX_ENTRIES * 5]; // 5 strings per entry
static size_t _entries_str_copies_count = 0;

static char* strdup_safe(const char* src) {
    if (!src) return NULL;
    size_t len = strlen(src);
    char* dst = malloc(len + 1);
    if (!dst) return NULL;
    memcpy(dst, src, len + 1);
    return dst;
}

static void free_internal_copies() {
    for (size_t i = 0; i < _entries_str_copies_count; i++) {
        free(_entries_str_copies[i]);
    }
    _entries_str_copies_count = 0;

    if (_user_str_copies[0]) { free(_user_str_copies[0]); _user_str_copies[0] = NULL; }
    if (_user_str_copies[1]) { free(_user_str_copies[1]); _user_str_copies[1] = NULL; }
}

static void copy_data_entry_deep(const struct dataEntryC* src, struct dataEntryC* dst) {
    dst->sort = src->sort;
    dst->positive = src->positive;
    dst->display = src->display;
    dst->single = src->single;
    dst->banchoSupport = src->banchoSupport;
    dst->titanicSupport = src->titanicSupport;

    // List of string fields to copy
    const char* src_fields[] = { src->key, src->name, src->init, src->current, src->change };
    const char** dst_fields[] = { &dst->key, &dst->name, &dst->init, &dst->current, &dst->change };

    for (int i = 0; i < 5; i++) {
        char* copy = strdup_safe(src_fields[i]);
        if (!copy) {
            fprintf(stderr, "Out of memory copying string field\n");
            exit(1);
        }
        _entries_str_copies[_entries_str_copies_count++] = copy;
        *dst_fields[i] = copy;
    }
}

void copyArrayData(const struct appC* app, const struct userC* user, const struct dataEntryC* entries, size_t count) {
    if (count > MAX_ENTRIES) count = MAX_ENTRIES;

    free_internal_copies();

    _app = *app;

    _user_str_copies[0] = strdup_safe(user->username);
    _user_str_copies[1] = strdup_safe(user->avatar);
    _user.username = _user_str_copies[0];
    _user.avatar = _user_str_copies[1];

    _entry_count = count;

    for (size_t i = 0; i < count; i++) {
        copy_data_entry_deep(&entries[i], &_entries[i]);
    }
}

#include <nuklear/nuklear.h>
#include <nuklear/nuklear_gdi.h>
#include "draw.h"

#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)

bool show_debug_layout = false;
bool data_debug_layout = false;

static LRESULT CALLBACK
WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (wparam == VK_F11) {
            show_debug_layout = !show_debug_layout;
            InvalidateRect(wnd, NULL, FALSE); // force redraw
        }
        if (wparam == VK_F10) {
            data_debug_layout = !data_debug_layout;
            InvalidateRect(wnd, NULL, FALSE); // force redraw
            return 0;
        }
        break;
    }

    if (nk_gdi_handle_event(wnd, msg, wparam, lparam))
        return 0;

    return DefWindowProcW(wnd, msg, wparam, lparam);
}

int ui_main()
{
    struct nk_context* ctx;

    WNDCLASSW wc;
    ATOM atom;
    RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
    DWORD exstyle = WS_EX_APPWINDOW;
    HWND wnd;
    HDC dc;
    int running = 1;
    int needs_refresh = 1;

    /* Win32 */
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(0);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"osu! Tracker";
    atom = RegisterClassW(&wc);

    AdjustWindowRectEx(&rect, style, FALSE, exstyle);
    wnd = CreateWindowExW(exstyle, wc.lpszClassName, WIDEN(OSU_TRACKER_TITLE),
        style | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, wc.hInstance, NULL);
    dc = GetDC(wnd);
    /* GUI */
    GdiFont* font;
    GdiFont* fontHeader;
    GdiFont* fontSmall;

    fontHeader = nk_gdifont_create("Arial", 22);
    font = nk_gdifont_create("Arial", 16);
    fontSmall = nk_gdifont_create("Arial", 16);

    ctx = nk_gdi_init(font, dc, WINDOW_WIDTH, WINDOW_HEIGHT);
    int w = WINDOW_WIDTH;
    int h = WINDOW_HEIGHT;

    DWORD lastFrameTime = GetTickCount();
    const DWORD targetFrameMS = 1000 / 60; // 60 FPS

    while (running)
    {
        MSG msg;
        bool hasInput = false;

        nk_input_begin(ctx); // Begin input handling

        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = 0;
                break;
            }

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            hasInput = true;
        }

        nk_input_end(ctx); // End input handling

        DWORD now = GetTickCount();
        DWORD delta = now - lastFrameTime;

        if (!hasInput) {
            // Limit to 60 FPS when idle
            if (delta < targetFrameMS) {
                Sleep(targetFrameMS - delta);
                continue;
            }
        }
        lastFrameTime = now;
        // Always draw
        nk_style_default(ctx);
        drawContent(ctx, font, fontSmall, fontHeader, w, h, _app, _user, _entries, _entry_count, show_debug_layout, data_debug_layout);
        nk_gdi_render(nk_rgb(0, 0, 0));
    }

    // Cleanup
    nk_gdifont_del(font);
    ReleaseDC(wnd, dc);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
    return 0;
}