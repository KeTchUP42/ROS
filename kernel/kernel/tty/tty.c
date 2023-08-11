#include <kernel/tty/tty.h>

#include <kernel/memory/kheap.h>

#include <libc/stdlib.h>
#include <libc/math.h>
#include <libc/string.h>

#include <stdbool.h>

typedef struct tty_node
{
    tty_entry_type *tty;
    struct tty_node *next;
} tty_node_type;

typedef struct screen_node
{
    screen_zone_type *scrn;
    tty_node_type *tty_root;
    tty_entry_type *tty_active;
    struct screen_node *next;
} screen_node_type;

/**
 * @brief Root node of system screen zones list. Each screen zones node contains its own list of tty.
 */
static screen_node_type *screen_node_root = NULL;

/**
 * @brief Additional help function which trails down from the list root and stops if found screen zone param.
 */
static screen_node_type *trail_screen_nodes(screen_node_type *node, screen_zone_type *scrn, cmptor_type cmp)
{
    while ((!cmp(node->scrn, scrn)) && (node->next != NULL))
        node = node->next;
    return node;
}

/**
 * @brief Additional help function which trails down from the list root and stops if found TTY param.
 */
static tty_node_type *trail_tty_nodes(tty_node_type *node, tty_entry_type *tty, cmptor_type cmp)
{
    while ((!cmp(node->tty, tty)) && (node->next != NULL))
        node = node->next;
    return node;
}

/**
 * @brief Factory function for SCREEN ZONE creation. Use ONLY this to SCREEN ZONE entry creation for tty system.
 */
screen_zone_type *tty_screen_zone_create(struct screen_zone *stack_sample)
{
    bool scrn_list_is_present = (screen_node_root != NULL);
    bool scrn_is_present;
    screen_node_type *node;

    if (scrn_list_is_present)
    {
        node = trail_screen_nodes(screen_node_root, stack_sample, &screen_zone_cmp);
        scrn_is_present = (node->next != NULL) ? true : screen_zone_cmp(node->scrn, stack_sample);
    }
    else
    {
        scrn_is_present = false;
    }

    if (scrn_is_present)
    {
        return node->scrn;
    }
    else
    {
        screen_zone_type *result = kmalloc(sizeof(screen_zone_type));
        result->scrn_x = stack_sample->scrn_x;
        result->scrn_y = stack_sample->scrn_y;
        result->scrn_width = stack_sample->scrn_width;
        result->scrn_height = stack_sample->scrn_height;
        return result;
    }
}

/**
 * @brief Factory function for TTY entry creation.
 */
tty_entry_type *tty_create(size_t w, size_t h, TTY_COLOR_TYPE clr, struct screen_zone *scrn)
{
    tty_entry_type *tty = (tty_entry_type *)kmalloc(sizeof(tty_entry_type));
    tty->tty_width = w;
    tty->tty_height = h;
    /* */
    tty->color = clr;
    tty->mem_buffer = (TTY_CELL_TYPE *)kmalloc(w * h * sizeof(TTY_CELL_TYPE));
    tty->scrn = scrn;

    /**
     * It sets to zero tty row and column:
     */
    tty_clean(tty);
    return tty;
}

static tty_node_type *tty_node_create(tty_entry_type *tty)
{
    tty_node_type *tty_node = (tty_node_type *)kmalloc(sizeof(tty_node_type));
    tty_node->tty = tty;
    tty_node->next = NULL;
    return tty_node;
}

static screen_node_type *scrn_node_create(tty_entry_type *tty)
{
    screen_node_type *scrn_node = (screen_node_type *)kmalloc(sizeof(screen_node_type));
    scrn_node->scrn = tty->scrn;

    scrn_node->tty_root = tty_node_create(tty);

    /* Auto sets current active tty: */
    scrn_node->tty_active = tty;

    scrn_node->next = NULL;
    return scrn_node;
}

/**
 * @brief Adds new just created tty entry to the tty system.
 */
void tty_add(struct tty_entry *tty)
{
    /* SCREEN ZONES list basic initialization: */
    if (screen_node_root == NULL)
    {
        screen_node_root = scrn_node_create(tty);
        tty_flush(screen_node_root->tty_active);
        return;
    }

    screen_node_type *scrn_node = trail_screen_nodes(screen_node_root, tty->scrn, &ptr_cmp);
    bool is_scrn_node_not_found = (scrn_node->next == NULL) && (scrn_node->scrn != tty->scrn);

    if (is_scrn_node_not_found)
    {
        scrn_node->next = scrn_node_create(tty);
        tty_flush(scrn_node->next->tty_active);
    }
    else
    {
        tty_node_type *tty_node = trail_tty_nodes(scrn_node->tty_root, tty, &ptr_cmp);
        /* If tty exists don't do anything: */
        if (tty_node->tty == tty)
            return;
        tty_node->next = tty_node_create(tty);
#if 1
        /* If you want change active tty every time you adds the new one: */
        scrn_node->tty_active = tty;
        tty_flush(scrn_node->tty_active);
#endif
    }
}

/**
 * @brief Additional help function which finds and returns previous node from node with SCREEN equal to scrn param.
 *
 * Or pointer to the first node if its SCREEN equal to scrn param.
 * Or NULL if there is no such node at all.
 */
static screen_node_type *trail_screen_nodes_prev(screen_node_type *node, screen_zone_type *scrn)
{
    if (node == NULL)
        return NULL;

    screen_node_type *prev = node;
    while (node->scrn != scrn)
    {
        if (node->next == NULL)
            return NULL;

        prev = node;
        node = node->next;
    }
    return prev;
}

/**
 * @brief Additional help function which finds and returns previous node from node with TTY equal to tty param.
 *
 * Or pointer to the first node if its TTY equal to tty param.
 * Or NULL if there is no such node at all.
 */
static tty_node_type *trail_tty_nodes_prev(tty_node_type *node, tty_entry_type *tty)
{
    if (node == NULL)
        return NULL;

    tty_node_type *prev = node;
    while (node->tty != tty)
    {
        if (node->next == NULL)
            return NULL;

        prev = node;
        node = node->next;
    }
    return prev;
}

/**
 * @brief Removes and deallocates tty entry from the system.
 *
 *  If tty entry was the last one with such SCREEN param it removed and deallocates its screen node too.
 */
void tty_remove(struct tty_entry *tty)
{
    screen_node_type *scrn_node_prev = trail_screen_nodes_prev(screen_node_root, tty->scrn);
    if (scrn_node_prev == NULL)
        return;
    bool is_scrn_root = (scrn_node_prev == screen_node_root) && (scrn_node_prev->scrn == tty->scrn);
    screen_node_type *scrn_node = is_scrn_root ? scrn_node_prev : scrn_node_prev->next;

    tty_node_type *tty_node_prev = trail_tty_nodes_prev(scrn_node->tty_root, tty);
    if (tty_node_prev == NULL)
        return;
    bool is_tty_root = (tty_node_prev == scrn_node->tty_root) && (tty_node_prev->tty == tty);
    tty_node_type *tty_node = is_tty_root ? tty_node_prev : tty_node_prev->next;

    if (is_tty_root)
        scrn_node->tty_root = tty_node->next;
    else
        tty_node_prev->next = tty_node->next;

    kfree(tty_node->tty->mem_buffer);
    kfree(tty_node->tty);
    kfree(tty_node);

    if (scrn_node->tty_root == NULL)
    {
        if (is_scrn_root)
            screen_node_root = scrn_node->next;
        else
            scrn_node_prev->next = scrn_node->next;

        kfree(scrn_node->scrn);
        kfree(scrn_node);
    }
    else
    {
        scrn_node->tty_active = is_tty_root ? scrn_node->tty_root->tty : tty_node_prev->tty;
        tty_flush(scrn_node->tty_active);
    }
}

int tty_flush(struct tty_entry *tty)
{
#if 1
    if (!tty_is_present(tty))
        return EXIT_FAILURE;
#endif
    size_t tty_width = tty->tty_width;
    size_t tty_height = tty->tty_height;

    size_t scrn_width = tty->scrn->scrn_width;
    size_t scrn_height = tty->scrn->scrn_height;

    TTY_CELL_TYPE *vga_cell_ptr;

    for (size_t y = 0; (y < tty_height) && (y < scrn_height); ++y)
    {
        const size_t h_index = y * tty_width;

        for (size_t x = 0; (x < tty_width) && (x < scrn_width); ++x)
        {
            vga_cell_ptr = (TTY_CELL_TYPE *)vga_cell(system_text_vga(), tty->scrn, x, y);
            if (vga_cell_ptr == NULL)
                break;

            const size_t index = h_index + x;
            *vga_cell_ptr = tty->mem_buffer[index];
        }
    }
    return EXIT_SUCCESS;
}

void tty_clean(struct tty_entry *tty)
{
    size_t tty_width = tty->tty_width;
    size_t tty_height = tty->tty_height;

    for (size_t y = 0; y < tty_height; ++y)
    {
        const size_t h_index = y * tty_width;

        for (size_t x = 0; x < tty_width; ++x)
        {
            const size_t index = h_index + x;
            tty->mem_buffer[index] = vga_cell_txt(' ', tty->color);
        }
    }
    tty->cursor_row = 0;
    tty->cursor_column = 0;
}

void tty_dealloc_all(void)
{
    screen_node_type *scrn_node = screen_node_root;

    while (scrn_node != NULL)
    {
        tty_node_type *tty_node = scrn_node->tty_root;

        while (tty_node != NULL)
        {
            kfree(tty_node->tty->mem_buffer);
            kfree(tty_node->tty);
            {
                tty_node_type *tmp = tty_node;
                tty_node = tty_node->next;
                kfree(tmp);
            }
        }
        kfree(scrn_node->scrn);
        {
            screen_node_type *tmp = scrn_node;
            scrn_node = scrn_node->next;
            kfree(tmp);
        }
    }

    screen_node_root = NULL;
}

static screen_node_type *scrn_node_with_scrn(screen_node_type *scrn_root, screen_zone_type *scrn)
{
    if (scrn_root == NULL)
        return NULL;

    screen_node_type *scrn_node = trail_screen_nodes(scrn_root, scrn, &ptr_cmp);

    bool is_scrn_node_not_found = (scrn_node->next == NULL) && (!ptr_cmp(scrn_node->scrn, scrn));
    if (is_scrn_node_not_found)
        return NULL;
    return scrn_node;
}

static tty_node_type *tty_node_with_tty(tty_node_type *tty_root, tty_entry_type *tty)
{
    if (tty_root == NULL)
        return NULL;

    tty_node_type *tty_node = trail_tty_nodes(tty_root, tty, &ptr_cmp);

    bool is_tty_node_not_found = (tty_node->next == NULL) && (!ptr_cmp(tty_node->tty, tty));
    if (is_tty_node_not_found)
        return NULL;
    return tty_node;
}

int tty_set_active(struct tty_entry *tty)
{
    screen_node_type *scrn_node = scrn_node_with_scrn(screen_node_root, tty->scrn);
    if (scrn_node == NULL)
        return EXIT_FAILURE;

    if (scrn_node->tty_active == tty)
        return EXIT_SUCCESS;

    tty_node_type *tty_node = tty_node_with_tty(scrn_node->tty_root, tty);
    if (tty_node == NULL)
        return EXIT_FAILURE;

    scrn_node->tty_active = tty;
    tty_flush(scrn_node->tty_active);
    return EXIT_SUCCESS;
}

tty_entry_type *screen_active_tty(struct screen_zone *scrn)
{
    screen_node_type *scrn_node = scrn_node_with_scrn(screen_node_root, scrn);
    return (scrn_node != NULL) ? scrn_node->tty_active : NULL;
}

bool tty_is_active(struct tty_entry *tty)
{
    tty_entry_type *active_tty = screen_active_tty(tty->scrn);
    return (active_tty != NULL) ? (active_tty == tty) : false;
}

bool tty_is_present(struct tty_entry *tty)
{
    screen_node_type *scrn_node = scrn_node_with_scrn(screen_node_root, tty->scrn);
    if (scrn_node == NULL)
        return false;

    tty_node_type *tty_node = tty_node_with_tty(scrn_node->tty_root, tty);
    if (tty_node == NULL)
        return false;
    return true;
}

static void tty_put(tty_entry_type *tty, char ch)
{
    const size_t i = tty->cursor_row * tty->tty_width + tty->cursor_column;
    tty->mem_buffer[i] = vga_cell_txt(ch, tty->color);
}

static void tty_shift_buffer_b(tty_entry_type *tty, size_t n)
{
    size_t sz = tty->tty_width * tty->tty_height;
    for (size_t i = 0; i < sz - n; ++i)
        tty->mem_buffer[i] = tty->mem_buffer[i + n];
#if 0
    for (size_t i = sz - n; i < sz; ++i)
        tty->mem_buffer[i] = tvga_entry(' ', tty->color);
#endif
}

static void tty_cursor_n(struct tty_entry *tty)
{
    tty->cursor_column = 0;
    const size_t row = tty->cursor_row + 1;
    if (row == tty->tty_height)
    {
        tty_shift_buffer_b(tty, tty->tty_width);
        return;
    }
    tty->cursor_row = row;
}

static void tty_move_cursor_forward(tty_entry_type *tty)
{
    if (++tty->cursor_column == tty->tty_width)
        tty_cursor_n(tty);
}

static void tty_move_cursor_backward(tty_entry_type *tty)
{
    if (tty->cursor_column == 0)
    {
        if (tty->cursor_row == 0)
        {
            return;
        }

        tty->cursor_column = tty->tty_width - 1;
        --tty->cursor_row;
        return;
    }
    --tty->cursor_column;
}

void tty_move_cursor(struct tty_entry *tty, int n)
{
    void (*tty_move)(tty_entry_type *) = (n > 0) ? &tty_move_cursor_forward : &tty_move_cursor_backward;
    n = abs(n);

    while (n-- != 0)
    {
        tty_move(tty);
    }
}

static int tty_escape(struct tty_entry *tty, char ch)
{
    static size_t escape_flag = 0;
    static TTY_COLOR_TYPE tmp_clr = 0;

    if (escape_flag != 0)
    {
        tmp_clr |= (uint8_t)ch;
        if (--escape_flag != 0)
        {
            /* For warning disabling, compiler will optimize this: */
            if (sizeof(TTY_COLOR_TYPE) > sizeof(uint8_t))
                tmp_clr <<= (sizeof(uint8_t) * 8);
            return EXIT_SUCCESS;
        }

        tty->color = tmp_clr;
        tmp_clr = 0;
        return EXIT_SUCCESS;
    }
    switch (ch)
    {
        case '\n':
            tty_cursor_n(tty);
            break;
        case '\b':
            tty_move_cursor(tty, -1);
            break;
        /* END OF TRANSMISSION */
        case 0x04:
            tty_flush(tty);
            break;
        /* '\e' */
        case 0x1b:
            escape_flag = sizeof(TTY_COLOR_TYPE);
            break;
        default:
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void tty_putchar(struct tty_entry *tty, char ch)
{
    int code = tty_escape(tty, ch);
    if (code == EXIT_SUCCESS) return;

    tty_put(tty, ch);
    tty_move_cursor(tty, 1);
}

void tty_write(struct tty_entry *tty, const char *data, size_t sz)
{
    for (size_t i = 0; i < sz; i++)
        tty_putchar(tty, data[i]);
}

void tty_print(struct tty_entry *tty, const char *str)
{
    while (*str)
    {
        tty_putchar(tty, *str);
        str++;
    }

#if 0
    tty_write(tty, str, strlen(str));
#endif
}
