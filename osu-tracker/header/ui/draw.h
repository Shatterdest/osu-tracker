#include "extc.h"
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
    int dataType;
    int formatType;
    bool noDiff;
    bool display;
    bool banchoSupport;
    bool titanicSupport;
};

void drawContent(struct nk_context* ctx, GdiFont* font, GdiFont* fontHeader, RECT* rect, struct userC* user, struct dataEntryC* entries, size_t count) {
    /* GUI */
    nk_draw_set_color_inline(ctx, NK_COLOR_INLINE_TAG);
    nk_gdi_set_font(fontHeader);
    ctx->style.window.header.active = nk_style_item_color(nk_rgb(51, 51, 51));
    ext_TextColor(ctx,255, 255, 255);
    ext_BG(ctx, 35, 35, 35);

    if (nk_begin(ctx, "[color=#FFFFFF]Railgun[/color]", nk_rect(0, 0, rect->right, rect->bottom),NK_WINDOW_TITLE| NK_WINDOW_BACKGROUND))
    {
        nk_gdi_set_font(font);
        enum { EASY, HARD };
        static int op = EASY;
        static int property = 20;
        nk_layout_row_static(ctx, 30, 80, 2);
        nk_label(ctx, "username", NK_TEXT_CENTERED | NK_TEXT_ALIGN_TOP);
    }
    nk_end(ctx);
};