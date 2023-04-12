#include <kernel/tty/ktty.h>

static tty_entry_type *tty = NULL;

tty_entry_type *kernel_default_tty(void)
{
    if (tty == NULL)
    {
        VGA_TEXT_COLOR_TYPE color = tvga_entry_color(TVGA_COLOR_LIGHT_GREY, TVGA_COLOR_BLACK);
        screen_zone_type scrn =
        {
            .scrn_x = 0,
            .scrn_y = 0,
            .scrn_width = VGA_TEXT_WIDTH,
            .scrn_height = VGA_TEXT_HEIGHT
        };
        tty = tty_create(VGA_TEXT_WIDTH, VGA_TEXT_HEIGHT, color, tty_screen_zone_create(&scrn));
    }

    return tty;
}
