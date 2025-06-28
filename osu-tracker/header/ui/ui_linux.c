#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_XLIB_IMPLEMENTATION
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_xlib.h>

#include "draw.h"

#define DTIME           20
#define WINDOW_WIDTH    400
#define WINDOW_HEIGHT   600
#define MAX_ENTRIES     100

    enum gameMode {
        osu = 0, taiko = 1, fruits = 2, mania = 3
    };

    enum server {
        bancho = 0, titanic = 1
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

    static struct appC _app = { 123456, osu, bancho };
    static struct userC _user = { "Neko", "avatar.png" };
    static struct dataEntryC _entries[MAX_ENTRIES];
    static size_t _entry_count = 0;

    bool show_debug_layout = false;
    bool data_debug_layout = false;

    typedef struct XWindow {
        Display* dpy;
        Window root;
        Visual* vis;
        Colormap cmap;
        XWindowAttributes attr;
        XSetWindowAttributes swa;
        Window win;
        int screen;
        XFont* font;
        XFont* fontHeader;
        XFont* fontSmall;
        unsigned int width;
        unsigned int height;
        Atom wm_delete_window;
    } XWindow;

    static void die(const char* fmt, ...)
    {
        va_list ap;
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        va_end(ap);
        fputs("\n", stderr);
        exit(EXIT_FAILURE);
    }

    static long timestamp(void)
    {
        struct timeval tv;
        if (gettimeofday(&tv, NULL) < 0) return 0;
        return (long)((long)tv.tv_sec * 1000 + (long)tv.tv_usec / 1000);
    }

    static void sleep_for(long t)
    {
        struct timespec req;
        const time_t sec = (int)(t / 1000);
        const long ms = t - (sec * 1000);
        req.tv_sec = sec;
        req.tv_nsec = ms * 1000000L;
        while (-1 == nanosleep(&req, &req));
    }

    int main(void)
    {
        long dt;
        long started;
        int running = 1;
        XWindow xw;
        struct nk_context* ctx;

        memset(&xw, 0, sizeof xw);
        xw.dpy = XOpenDisplay(NULL);
        if (!xw.dpy) die("Could not open display; $DISPLAY not set?");
        xw.root = DefaultRootWindow(xw.dpy);
        xw.screen = DefaultScreen(xw.dpy);
        xw.vis = DefaultVisual(xw.dpy, xw.screen);
        xw.cmap = XCreateColormap(xw.dpy, xw.root, xw.vis, AllocNone);

        xw.swa.colormap = xw.cmap;
        xw.swa.event_mask =
            ExposureMask | KeyPressMask | KeyReleaseMask |
            ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
            PointerMotionMask | KeymapStateMask;

        xw.win = XCreateWindow(
            xw.dpy, xw.root, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
            DefaultDepth(xw.dpy, xw.screen), InputOutput, xw.vis,
            CWEventMask | CWColormap, &xw.swa
        );

        XStoreName(xw.dpy, xw.win, "osu! Tracker");
        XMapWindow(xw.dpy, xw.win);
        xw.wm_delete_window = XInternAtom(xw.dpy, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(xw.dpy, xw.win, &xw.wm_delete_window, 1);
        XGetWindowAttributes(xw.dpy, xw.win, &xw.attr);
        xw.width = (unsigned int)xw.attr.width;
        xw.height = (unsigned int)xw.attr.height;

        // Fonts (match Windows sizes)
        xw.font = nk_xfont_create(xw.dpy, "Sans-16");
        xw.fontHeader = nk_xfont_create(xw.dpy, "Sans-22");
        xw.fontSmall = nk_xfont_create(xw.dpy, "Sans-14");

        ctx = nk_xlib_init(xw.font, xw.dpy, xw.screen, xw.win, xw.width, xw.height);

        while (running)
        {
            XEvent evt;
            started = timestamp();
            nk_input_begin(ctx);
            while (XPending(xw.dpy)) {
                XNextEvent(xw.dpy, &evt);
                if (evt.type == ClientMessage && (Atom)evt.xclient.data.l[0] == xw.wm_delete_window)
                    goto cleanup;
                if (XFilterEvent(&evt, xw.win)) continue;
                nk_xlib_handle_event(xw.dpy, xw.screen, xw.win, &evt);

                // Toggle debug layouts with F10/F11
                if (evt.type == KeyPress) {
                    KeySym sym = XLookupKeysym(&evt.xkey, 0);
                    if (sym == XK_F11) show_debug_layout = !show_debug_layout;
                    if (sym == XK_F10) data_debug_layout = !data_debug_layout;
                }
            }
            nk_input_end(ctx);

            nk_style_default(ctx);

            // GUI rendering
            drawContent(ctx, xw.font, xw.fontSmall, xw.fontHeader, xw.width, xw.height,
                _app, _user, _entries, _entry_count, show_debug_layout, data_debug_layout);

            XClearWindow(xw.dpy, xw.win);
            nk_xlib_render(xw.win, nk_rgb(0, 0, 0));
            XFlush(xw.dpy);

            dt = timestamp() - started;
            if (dt < DTIME) sleep_for(DTIME - dt);
        }

    cleanup:
        nk_xfont_del(xw.dpy, xw.font);
        nk_xfont_del(xw.dpy, xw.fontHeader);
        nk_xfont_del(xw.dpy, xw.fontSmall);
        nk_xlib_shutdown();
        XUnmapWindow(xw.dpy, xw.win);
        XFreeColormap(xw.dpy, xw.cmap);
        XDestroyWindow(xw.dpy, xw.win);
        XCloseDisplay(xw.dpy);
        return 0;
    }

#ifdef __cplusplus
}
#endif
