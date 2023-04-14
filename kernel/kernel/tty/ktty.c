#include <kernel/tty/ktty.h>

static tty_entry_type *default_tty = NULL;

tty_entry_type *kernel_default_tty(void)
{
    if (default_tty == NULL)
    {
        VGA_TEXT_COLOR_TYPE color = vga_color_txt(TVGA_COLOR_LIGHT_GREY, TVGA_COLOR_BLACK);
        screen_zone_type scrn =
        {
            .scrn_x = 0,
            .scrn_y = 0,
            .scrn_width = VGA_TEXT_WIDTH,
            .scrn_height = VGA_TEXT_HEIGHT
        };
        default_tty = tty_create(VGA_TEXT_WIDTH, VGA_TEXT_HEIGHT, color, tty_screen_zone_create(&scrn));
    }

    return default_tty;
}

void kernel_default_tty_reset(struct tty_entry *tty)
{
    bool was_present = (default_tty != NULL) ? tty_is_present(default_tty) : false;
    if (was_present)
        tty_remove(default_tty);

    default_tty = tty;

    if ((default_tty != NULL) && was_present)
        tty_add(default_tty);
}
