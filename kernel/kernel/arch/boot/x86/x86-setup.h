#ifndef I386_SETUP_H
#define I386_SETUP_H

#include <stdint.h>
#include <stddef.h>

/* Each define here is for a specific flag in the descriptor.                               */
/* Refer to the intel documentation for a description of what each one does.                */
#define SEG_DESCTYPE(x)  ((x) << 0x04) /* Descriptor type (0 for system, 1 for code/data)   */
#define SEG_PRES(x)      ((x) << 0x07) /* Present                                           */
#define SEG_SAVL(x)      ((x) << 0x0C) /* Available for system use                          */
#define SEG_LONG(x)      ((x) << 0x0D) /* Long mode                                         */
#define SEG_SIZE(x)      ((x) << 0x0E) /* Size (0 for 16-bit, 1 for 32)                     */
#define SEG_GRAN(x)      ((x) << 0x0F) /* Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)     */
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05) /* Set privilege level (0 - 3)              */

#define SEG_DATA_RD        0x00 /* Read-Only                                                */
#define SEG_DATA_RDA       0x01 /* Read-Only, accessed                                      */
#define SEG_DATA_RDWR      0x02 /* Read/Write                                               */
#define SEG_DATA_RDWRA     0x03 /* Read/Write, accessed                                     */
#define SEG_DATA_RDEXPD    0x04 /* Read-Only, expand-down                                   */
#define SEG_DATA_RDEXPDA   0x05 /* Read-Only, expand-down, accessed                         */
#define SEG_DATA_RDWREXPD  0x06 /* Read/Write, expand-down                                  */
#define SEG_DATA_RDWREXPDA 0x07 /* Read/Write, expand-down, accessed                        */
#define SEG_CODE_EX        0x08 /* Execute-Only                                             */
#define SEG_CODE_EXA       0x09 /* Execute-Only, accessed                                   */
#define SEG_CODE_EXRD      0x0A /* Execute/Read                                             */
#define SEG_CODE_EXRDA     0x0B /* Execute/Read, accessed                                   */
#define SEG_CODE_EXC       0x0C /* Execute-Only, conforming                                 */
#define SEG_CODE_EXCA      0x0D /* Execute-Only, conforming, accessed                       */
#define SEG_CODE_EXRDC     0x0E /* Execute/Read, conforming                                 */
#define SEG_CODE_EXRDCA    0x0F /* Execute/Read, conforming, accessed                       */

#define GDT_CODE_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_CODE_EXRD

#define GDT_DATA_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWR

#define GDT_CODE_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_CODE_EXRD

#define GDT_DATA_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_DATA_RDWR

#define GDT_TSS_PL0  SEG_DESCTYPE(0) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(0) | SEG_GRAN(0) | \
                     SEG_PRIV(0)     | SEG_CODE_EXA

#define GDT_OFF_NULL_DESCRIPTOR     (0x0000)
#define GDT_OFF_KERNEL_CODE_SEGMENT (0x0008)
#define GDT_OFF_KERNEL_DATA_SEGMENT (0x0010)
#define GDT_OFF_USER_CODE_SEGMENT   (0x0018)
#define GDT_OFF_USER_DATA_SEGMENT   (0x0020)
#define GDT_OFF_TSS_SEGMENT         (0x0028)

/**
 * @brief This function saves the address to the TSS, so it will be restored from there when an interrupt occurs.
 */
void __save_kernel_stack(void *esp);

#endif // I386_SETUP_H
