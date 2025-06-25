/* nuklear - 1.32.0 - public domain */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 600

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_GDI_IMPLEMENTATION

struct userC* g_user = NULL;
struct dataEntryC* g_entries = NULL;
size_t g_entry_count = 0;


#include <nuklear/nuklear.h>
#include <nuklear/nuklear_gdi.h>
#include "draw.h"

#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)

static LRESULT CALLBACK
WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    if (nk_gdi_handle_event(wnd, msg, wparam, lparam))
        return 0;

    return DefWindowProcW(wnd, msg, wparam, lparam);
}

int ui_main(const struct userC* user, const struct dataEntryC* entries, size_t count)
{
    g_user = (struct userC*)user;
    g_entries = (struct dataEntryC*)entries;
    g_entry_count = count;

    // Print user info
    printf("User Info:\n");
    printf("  username: %s\n", user->username ? user->username : "(null)");
    printf("  avatar: %s\n\n", user->avatar ? user->avatar : "(null)");

    // Print each data entry
    for (size_t i = 0; i < count; ++i)
    {
        printf("Entry %zu:\n", i);
        printf("  key: %s\n", entries[i].key ? entries[i].key : "(null)");
        printf("  name: %s\n", entries[i].name ? entries[i].name : "(null)");
        printf("  sort: %d\n", entries[i].sort);
        printf("  init: %s\n", entries[i].init ? entries[i].init : "(null)");
        printf("  current: %s\n", entries[i].current ? entries[i].current : "(null)");
        printf("  change: %s\n", entries[i].change ? entries[i].change : "(null)");
        printf("  dataType: %d\n", entries[i].dataType);
        printf("  formatType: %d\n", entries[i].formatType);
        printf("  noDiff: %s\n", entries[i].noDiff ? "true" : "false");
        printf("  display: %s\n", entries[i].display ? "true" : "false");
        printf("  banchoSupport: %s\n", entries[i].banchoSupport ? "true" : "false");
        printf("  titanicSupport: %s\n", entries[i].titanicSupport ? "true" : "false");
        printf("\n");
    }

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

    fontHeader = nk_gdifont_create("Arial", 22);
    font = nk_gdifont_create("Arial", 16);
    ctx = nk_gdi_init(font, dc, WINDOW_WIDTH, WINDOW_HEIGHT);

    while (running)
    {
        // Print user info
        printf("User Info:\n");
        printf("  username: %s\n", user->username ? user->username : "(null)");
        printf("  avatar: %s\n\n", user->avatar ? user->avatar : "(null)");

        /* Input */
        MSG msg;
        nk_input_begin(ctx);
        if (needs_refresh == 0) {
            if (GetMessageW(&msg, NULL, 0, 0) <= 0)
                running = 0;
            else {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            needs_refresh = 1;
        }
        else needs_refresh = 0;

        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                running = 0;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            needs_refresh = 1;
        }
        nk_input_end(ctx);

        /* GUI */
        RECT rectWindow;
        GetWindowRect(wnd, &rectWindow);
        drawContent(ctx, font, fontHeader, &rectWindow);


        /* Draw */
        nk_gdi_render(nk_rgb(0, 0, 0));
    }

    nk_gdifont_del(font);
    ReleaseDC(wnd, dc);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
    return 0;
}
