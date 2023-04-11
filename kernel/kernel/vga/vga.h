#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct vga_entry
{
    /* VGA width and height in used code points: */
    size_t vga_width;
    size_t vga_height;
    size_t cell_sz;
    void *vga_buffer;
} vga_entry_type;

typedef struct screen_zone
{
    size_t scrn_x;
    size_t scrn_y;
    size_t scrn_width;
    size_t scrn_height;
} screen_zone_type;


static inline void *vga_cell(const struct vga_entry *vga, const struct screen_zone *scrn, size_t x, size_t y)
{
    bool x_value_in_range = ((scrn->scrn_x + x) < vga->vga_width);
    bool y_value_in_range = ((scrn->scrn_y + y) < vga->vga_height);

    if (x_value_in_range && y_value_in_range)
    {
        void *scrn_zone_addr = vga->vga_buffer + ((vga->vga_width * vga->cell_sz) * scrn->scrn_y) + (scrn->scrn_x * vga->cell_sz);
        void *scrn_cell_addr = scrn_zone_addr + ((vga->vga_width * vga->cell_sz) * y + (x * vga->cell_sz));
        return scrn_cell_addr;
    }
    return NULL;
}
/**
 * @brief Plain screen zone compare function.
 */
static inline bool screen_zone_cmp(const void *lhs, const void *rhs)
{
    screen_zone_type *lscrn = (screen_zone_type *)lhs;
    screen_zone_type *rscrn = (screen_zone_type *)rhs;

    return (lscrn->scrn_x == rscrn->scrn_x) &&
           (lscrn->scrn_y == rscrn->scrn_y) &&
           (lscrn->scrn_width == rscrn->scrn_width) &&
           (lscrn->scrn_height == rscrn->scrn_height);
}

#endif // VGA_H
