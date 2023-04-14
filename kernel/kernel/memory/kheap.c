#include <kernel/memory/kheap.h>

#include <stdbool.h>
#include <stddef.h>

#include <libc/stdlib.h>

/**
 * @brief Kernel low-level info constants with readable names.
 */
#define KERNEL_HEAP_START ((unsigned char *)__kernel_heap_start)
#define KERNEL_HEAP_END ((unsigned char *)__kernel_heap_end)

/**
 * @brief Current available kernel heap size and number of nodes.
 */
static size_t heap_memory_left = KERNEL_HEAP_SIZE;
static size_t heap_memory_allocated = 0;
static size_t heap_nodes_count = 0;

/**
 * @brief In addition to the data, each node has a header with this format.
 */
typedef struct __attribute__ ((aligned (KERNEL_HEAP_ALIGN))) heap_node
{
    size_t gap_sz;
    size_t sz;
    struct heap_node *next;
} heap_node_type;

#define NO_GAP (0)

/**
 * @brief Pointer to the first existing heap node.
 */
static heap_node_type *heap_node_root = NULL;

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
    if ((sz > KERNEL_HEAP_ALIGN) && (sz % KERNEL_HEAP_ALIGN != 0))
    {
        sz = ((sz / KERNEL_HEAP_ALIGN) + 1) * KERNEL_HEAP_ALIGN;
    }
    else if (sz < KERNEL_HEAP_ALIGN)
    {
        sz = KERNEL_HEAP_ALIGN;
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

static heap_node_type *heap_create_tail_node(heap_node_type *node, size_t sz)
{
    node->sz = sz;
    node->gap_sz = NO_GAP;
    node->next = NULL;
    return node;
}

static void heap_update_node_statistics_malloc(size_t sz)
{
    heap_memory_left -= sz;
    heap_memory_allocated += sz;
    ++heap_nodes_count;
}

void *kmalloc(size_t sz)
{
    if (sz == 0) return NULL;
    sz = align_size(sz);

    /* Full node header + data size: */
    size_t node_data_size = full_node_size(sz);

    if (heap_memory_left < node_data_size)
        return NULL;

    /* Heap basic node initialization: */
    if (heap_node_root == NULL)
    {
        heap_node_root = heap_create_tail_node((heap_node_type *)KERNEL_HEAP_START, sz);
        heap_update_node_statistics_malloc(node_data_size);
        /* Jumps to the data address: */
        return (heap_node_root + 1);
    }

    heap_node_type *node;
    ptrdiff_t root_diff = (void *)heap_node_root - (void *)KERNEL_HEAP_START;

    if (root_diff < 0)
        die("kmalloc(): heap underflow!\n");

    if ((size_t)root_diff >= node_data_size)
    {
        node = (heap_node_type *)KERNEL_HEAP_START;

        node->sz = sz;
        node->gap_sz = root_diff - node_data_size;
        node->next = heap_node_root;

        heap_node_root = node;
        heap_update_node_statistics_malloc(node_data_size);
        /* Jumps to the data address: */
        return (node + 1);
    }

    /* Finds free heap node: */
    node = trail_heap_space(heap_node_root, node_data_size);

    heap_node_type *next_node = (heap_node_type *)((void *)(node + 1) + node->sz);

    if (node->next == NULL)
    {
        {
            ptrdiff_t free_heap_space = (ptrdiff_t)((void *)KERNEL_HEAP_END - (void *)next_node);
            if ((free_heap_space < 0) || ((size_t)free_heap_space < node_data_size))
                return NULL;
        }
        node->next = heap_create_tail_node(next_node, sz);
    }
    else
    {
        next_node->sz = sz;
        next_node->gap_sz = node->gap_sz - node_data_size;
        node->gap_sz = NO_GAP;
        next_node->next = node->next;
        node->next = next_node;
    }

    heap_update_node_statistics_malloc(node_data_size);

    /* Jumps to the data address: */
    return (next_node + 1);
}

/**
 * @brief Searches needed node in the heap from given node ptr.
 */
static heap_node_type *trail_heap_node(heap_node_type *node, heap_node_type *needed)
{
    if (node == needed)
        return node;

    while (node->next != needed)
    {
        if (node->next == NULL)
            return NULL;
        node = node->next;
    }
    return node;
}

static void heap_update_node_statistics_free(size_t sz)
{
    heap_memory_left += sz;
    heap_memory_allocated -= sz;
    --heap_nodes_count;
}

void kfree(void *ptr)
{
    if (ptr == NULL)
        return;
    if (heap_nodes_count == 0)
        die("kfree(): no heap nodes!\n");

    if ((ptrdiff_t)ptr % KERNEL_HEAP_ALIGN != 0)
        die("kfree(): invalid ptr align!\n");

    {
        bool is_not_valid_0 = ptr < ((void *)KERNEL_HEAP_START + sizeof(heap_node_type));
        bool is_not_valid_1 = ptr > ((void *)KERNEL_HEAP_END - sizeof(heap_node_type) - KERNEL_HEAP_ALIGN);

        if (is_not_valid_0 || is_not_valid_1)
            die("kfree(): invalid ptr heap range!\n");
    }

    bool is_node_root = false;
    heap_node_type *node;

    {
        heap_node_type *needed_node = (heap_node_type *)(ptr - sizeof(heap_node_type));
        /* It gets needed-node parent or root node: */
        node = trail_heap_node(heap_node_root, needed_node);
        if (node == NULL)
            die("kfree(): no such heap node!\n");

        if (heap_node_root == needed_node)
            is_node_root = true;
    }

    /* Full node header + data size: */
    size_t node_data_size = full_node_size(is_node_root ? node->sz : node->next->sz);

    if (is_node_root)
    {
        heap_node_root = node->next;
        heap_update_node_statistics_free(node_data_size);
        return;
    }

    node->gap_sz = node->gap_sz + node->next->gap_sz + node_data_size;
    node->next = node->next->next;

    if (node->next == NULL)
        node->gap_sz = NO_GAP;

    /* Updating heap nodes statics: */
    heap_update_node_statistics_free(node_data_size);
}

size_t kheap_left(void)
{
    return heap_memory_left;
}

size_t kheap_allocated(void)
{
    return heap_memory_allocated;
}

size_t kheap_nodes_count(void)
{
    return heap_nodes_count;
}
