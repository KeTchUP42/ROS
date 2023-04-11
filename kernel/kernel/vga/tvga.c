#include <kernel/vga/tvga.h>

static const vga_entry_type system_vga =
{
    .vga_width = VGA_TEXT_WIDTH, .vga_height = VGA_TEXT_HEIGHT, .cell_sz = sizeof(uint16_t), .vga_buffer = VGA_TEXT
};

const vga_entry_type *system_text_vga(void)
{
    return &system_vga;
}
