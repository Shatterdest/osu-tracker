#include "extc.h"
void* drawContent(struct nk_context* ctx) {
    /* GUI */
    nk_draw_set_color_inline(ctx, NK_COLOR_INLINE_TAG);

    if (nk_begin(ctx, custom_strcat(1,"[color=#ff0000]test[/color]"), nk_rect(50, 50, 200, 200),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
        NK_WINDOW_CLOSABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
    {
        enum { EASY, HARD };
        static int op = EASY;
        static int property = 20;
        nk_layout_row_static(ctx, 30, 80, 1);
        nk_label(ctx, "username", NK_TEXT_CENTERED | NK_TEXT_ALIGN_TOP);

        if (nk_button_label(ctx, "button"))
            fprintf(stdout, "button pressed\n");
        nk_layout_row_dynamic(ctx, 30, 2);
        if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
        if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
    }
    nk_end(ctx);
};