#ifndef KTTY_H
#define KTTY_H

#include <kernel/tty/tty.h>

/**
 * @brief Lazy constructs kernel_default_tty and returns it.
 */
tty_entry_type *kernel_default_tty(void);

/**
 * @brief Sets kernel_default_tty with new value and if it was in the tty system replaces it.
 */
void kernel_default_tty_reset(struct tty_entry *tty);

#endif // KTTY_H
