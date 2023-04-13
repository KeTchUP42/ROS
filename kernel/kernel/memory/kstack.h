#ifndef KSTACK_H
#define KSTACK_H

/**
 * @brief Kernel low-level stack info constants.
 *
 * To get value write:
 * (unsigned char *)__kernel_stack_start;
 */
extern unsigned char __kernel_stack_start[];
extern unsigned char __kernel_stack_end[];

/**
 * @brief Kernel low-level stack-size info.
 *
 * To get value write:
 * (size_t)__kernel_stack_size;
 */
extern const size_t __kernel_stack_size[];

/**
 * @brief Kernel user-friendly stack size.
 */
#define KERNEL_STACK_SIZE ((size_t)__kernel_stack_size)

#endif // KSTACK_H
