#include <kernel/memory/kheap.h>

#include <stdbool.h>
#include <stddef.h>

#include <libc/stdlib.h>

/**
 * @brief Kernel low-level info constants with readable names.
 */
static unsigned char *const kernel_heap_start = (unsigned char *const)__kernel_heap_start;
static unsigned char *const kernel_heap_end = (unsigned char *const)__kernel_heap_end;

/**
 * @brief Current available kernel heap size and number of nodes.
 */
static size_t heap_size = (size_t)__kernel_heap_size;
static size_t heap_nodes = 0;

/**
 * @brief In addition to the data, each node has a header with this format.
 */
typedef struct __attribute__ ((aligned (HEAP_ALIGN))) heap_node
{
    size_t gap_sz;
    size_t sz;
    struct heap_node *next;
} heap_node_type;

#define NO_GAP (0)

/**
 * @brief Searches available memory with given chunk size in the heap with start from node-ptr.
 */
static heap_node_type *trail_heap_space(heap_node_type *node, size_t sz)
{
    while ((node->gap_sz < sz) && (node->next != NULL))
        node = node->next;
    return node;
}

/**
 * @brief Function aligns kmalloc() given size value to HEAP_ALIGN aligned.
 */
static size_t align_size(size_t sz)
{
    if ((sz > HEAP_ALIGN) && (sz % HEAP_ALIGN != 0))
    {
        sz = ((sz / HEAP_ALIGN) + 1) * HEAP_ALIGN;
    }
    else if (sz < HEAP_ALIGN)
    {
        sz = HEAP_ALIGN;
    }
    return sz;
}

/**
 * @brief Full node header + data size.
 */
static size_t full_node_size(size_t sz)
{
    return sizeof(heap_node_type) + sz;
}

void *kmalloc(size_t sz)
{
    if (sz == 0) return NULL;
    sz = align_size(sz);

    /* Full node header + data size: */
    size_t node_data_size = full_node_size(sz);

    if (heap_size < node_data_size)
        return NULL;

    /* Heap basic node initialization: */
    heap_node_type *node = (heap_node_type *)kernel_heap_start;
    if (heap_nodes == 0)
    {
        node->sz = sz;
        node->gap_sz = NO_GAP;
        node->next = NULL;

        ++heap_nodes;
        heap_size -= node_data_size;
        /* Jumps to the data address: */
        ++node;
        return node;
    }

    /* Finds free heap node: */
    node = trail_heap_space(node, node_data_size);

    heap_node_type *next_node = (heap_node_type *)((void *)(node + 1) + node->sz);

    if (node->next == NULL)
    {
        {
            ptrdiff_t free_heap_space = (ptrdiff_t)((void *)kernel_heap_end - (void *)next_node);
            if ((free_heap_space < 0) || ((size_t)free_heap_space < node_data_size))
                return NULL;
        }

        node->next = next_node;

        next_node->sz = sz;
        next_node->gap_sz = NO_GAP;
        next_node->next = NULL;
    }
    else
    {
        next_node->sz = sz;

        next_node->gap_sz = node->gap_sz - node_data_size;
        node->gap_sz = NO_GAP;

        next_node->next = node->next;
        node->next = next_node;
    }

    ++heap_nodes;
    heap_size -= node_data_size;
    /* Jumps to the data address: */
    ++next_node;
    return next_node;
}

/**
 * @brief Searches needed node in the heap from given node ptr.
 */
static heap_node_type *trail_heap_node(heap_node_type *node, heap_node_type *needed)
{
    while (node->next != needed)
    {
        if (node->next == NULL)
            return NULL;
        node = node->next;
    }
    return node;
}

void kfree(void *ptr)
{
    if (ptr == NULL)
        return;
    if (heap_nodes == 0)
        die("kfree(): no heap nodes!\n");

    if ((ptrdiff_t)ptr % HEAP_ALIGN != 0)
        die("kfree(): invalid ptr align!\n");

    {
        bool is_not_valid_0 = ptr < ((void *)kernel_heap_start + sizeof(heap_node_type));
        bool is_not_valid_1 = ptr > ((void *)kernel_heap_end - sizeof(heap_node_type) - HEAP_ALIGN);

        if (is_not_valid_0 || is_not_valid_1)
            die("kfree(): invalid ptr heap range!\n");
    }

    /* Heap basic node initialization: */
    heap_node_type *node = (heap_node_type *)kernel_heap_start;

    {
        heap_node_type *needed_node = (heap_node_type *)(ptr - sizeof(heap_node_type));
        /* It gets needed-node parent: */
        node = trail_heap_node(node, needed_node);
        if (node == NULL)
            die("kfree(): no such heap node!\n");
    }

    /* Full node header + data size: */
    size_t node_data_size = full_node_size(node->next->sz);

    node->gap_sz = node->gap_sz + node->next->gap_sz + node_data_size;
    node->next = node->next->next;

    if (node->next == NULL)
        node->gap_sz = NO_GAP;
}

size_t kheap_left(void)
{
    return heap_size;
}

size_t kheap_nodes(void)
{
    return heap_nodes;
}
