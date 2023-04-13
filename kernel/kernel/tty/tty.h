#ifndef TTY_H
#define TTY_H

#include <stddef.h>
#include <stdbool.h>

#include <kernel/vga/vga.h>
#include <kernel/vga/tvga.h>

#define TTY_COLOR_TYPE VGA_TEXT_COLOR_TYPE
#define TTY_CELL_TYPE VGA_TEXT_CELL_TYPE

/**
 * @brief TTY virtual terminal descriptor, used by procs.
 */
typedef struct tty_entry
{
    size_t tty_width;
    size_t tty_height;
    size_t cursor_row;
    size_t cursor_column;
    TTY_COLOR_TYPE color;
    TTY_CELL_TYPE *mem_buffer;
    struct screen_zone *scrn;
} tty_entry_type;

/**
 * @brief Factory function for SCREEN ZONE creation. Use ONLY this to SCREEN ZONE entry creation for tty system.
 */
screen_zone_type *tty_screen_zone_create(struct screen_zone *stack_sample);

/**
 * @brief Factory function for TTY entry creation.
 */
tty_entry_type *tty_create(size_t w, size_t h, TTY_COLOR_TYPE clr, struct screen_zone *scrn);

/**
 * @brief Adds new just created tty entry to the tty system.
 */
void tty_add(struct tty_entry *tty);

/**
 * @brief Removes and deallocates tty entry from the system.
 *
 *  If tty entry was the last one with such SCREEN param it removed and deallocates its screen node too.
 */
void tty_remove(struct tty_entry *tty);

/**
 * @brief Deallocates ALL tty system.
 */
void tty_dealloc_all(void);

/**
 * @brief Writes mem_buffer to the screen.
 *
 * @return EXIT_FAILURE if tty entry is't in the system. Otherwise EXIT_SUCCESS.
 */
int tty_flush(struct tty_entry *tty);

/**
 * @brief Clears tty_entry mem_buffer and sets cursor to the very start.
 */
void tty_clean(struct tty_entry *tty);

/**
 * @brief Gets active tty by the ptr to screen zone or NULL if there is no such screen zone in the system.
 */
tty_entry_type *screen_active_tty(struct screen_zone *scrn);

bool tty_is_active(struct tty_entry *tty);
bool tty_is_present(struct tty_entry *tty);

/**
 * @brief Prints char string to the tty.
 *
 * TTY ESCAPE:
 * '\n'  - moves cursor to the new line.
 * '\b'  - moves cursor backward to 1 code point.
 * '\04' - flushes tty buffer to the vga memory.
 * '\e'{TTY_COLOR_TYPE} - sets tty color, reads color bytes in Big-Endian order.
 */
void tty_print(struct tty_entry *tty, const char *str);
void tty_write(struct tty_entry *tty, const char *data, size_t sz);
void tty_putchar(struct tty_entry *tty, char ch);
void tty_move_cursor(struct tty_entry *tty, int n);

#endif // TTY_H
