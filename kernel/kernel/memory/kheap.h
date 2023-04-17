#ifndef KHEAP_H
#define KHEAP_H

#include <stddef.h>

/**
 * @brief Kernel low-level heap info constants.
 *
 * To get value write:
 * (unsigned char *)__kernel_heap_start;
 */
extern unsigned char __kernel_heap_start[];
extern unsigned char __kernel_heap_end[];

/**
 * @brief Kernel low-level heap-size info.
 *
 * To get value write:
 * (size_t)__kernel_heap_size;
 */
extern const size_t __kernel_heap_size[];

/**
 * @brief Align of kmalloc() return ptr.
 */
#define KERNEL_HEAP_ALIGN (0x10)

/**
 * @brief Kernel user-friendly heap size.
 */
#define KERNEL_HEAP_SIZE ((size_t)__kernel_heap_size)

/**
 * @brief Allocates size_t bytes of uninitialized storage.
 *
 * @return Address of allocated data with HEAP_ALIGN align or NULL on failure.
 */
void *kmalloc(size_t sz);

/**
 * @brief Deallocates the space previously allocated by kmalloc().
 *
 * If ptr is a null pointer, the function does nothing.
 *
 * Kernel will call die() if the value of input ptr not equal a value returned earlier by kmalloc().
 * Kernel will call die() if the memory area referred to by ptr has already been deallocated.
 */
void kfree(void *ptr);

/**
 * @brief Available kernel heap memory size in bytes.
 */
size_t kheap_left(void);

/**
 * @brief Allocated kernel heap memory size in bytes.
 */
size_t kheap_allocated(void);

/**
 * @brief Allocated kernel heap nodes count.
 */
size_t kheap_nodes_count(void);

#endif // KHEAP_H
