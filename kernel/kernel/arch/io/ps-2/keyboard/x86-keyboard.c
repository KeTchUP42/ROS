#include "x86-keyboard.h"

#include <kernel/tty/sys-ksc.h>

static sys_kb_scn_cds_type sys_scan_codes_map_short[256] =
{
    [0x0E] = SYS_KSC_BACKSPACE,
    [0x0F] = SYS_KSC_TAB,
    [0x1C] = SYS_KSC_ENTER,
    [0x01] = SYS_KSC_ESCAPE,
    [0x39] = SYS_KSC_SPACE,

    [0x28] = SYS_KSC_QUOTATION_MARK_SINGLE,
    [0x37] = SYS_KSC_ASTERISK_NUMPAD,

#if 0
    [0x4E] = SYS_KSC_NUMPAD_PLUS,
#endif

    [0x33] = SYS_KSC_COMMA,
    [0x0C] = SYS_KSC_MINUS,
    [0x34] = SYS_KSC_DOT,
    [0x35] = SYS_KSC_SLASH_FOWARD,

    [0x0B] = SYS_KSC_NUMBER_0,
    [0x02] = SYS_KSC_NUMBER_1,
    [0x03] = SYS_KSC_NUMBER_2,
    [0x04] = SYS_KSC_NUMBER_3,
    [0x05] = SYS_KSC_NUMBER_4,
    [0x06] = SYS_KSC_NUMBER_5,
    [0x07] = SYS_KSC_NUMBER_6,
    [0x08] = SYS_KSC_NUMBER_7,
    [0x09] = SYS_KSC_NUMBER_8,
    [0x0A] = SYS_KSC_NUMBER_9,

    [0x27] = SYS_KSC_COLON_SEMI,
    [0x0D] = SYS_KSC_EQUAL_TO,
    [0x1A] = SYS_KSC_BRACKET_OPEN,
    [0x2B] = SYS_KSC_SLASH_BACKWARD,
    [0x1B] = SYS_KSC_BRACKET_CLOSE,
    [0x29] = SYS_KSC_GRAVE_ACCENT,

    [0x1E] = SYS_KSC_A,
    [0x30] = SYS_KSC_B,
    [0x2E] = SYS_KSC_C,
    [0x20] = SYS_KSC_D,
    [0x12] = SYS_KSC_E,
    [0x21] = SYS_KSC_F,
    [0x22] = SYS_KSC_G,
    [0x23] = SYS_KSC_h,
    [0x17] = SYS_KSC_I,
    [0x24] = SYS_KSC_J,
    [0x25] = SYS_KSC_K,
    [0x26] = SYS_KSC_L,
    [0x32] = SYS_KSC_M,
    [0x31] = SYS_KSC_N,
    [0x18] = SYS_KSC_O,
    [0x19] = SYS_KSC_P,
    [0x10] = SYS_KSC_Q,
    [0x13] = SYS_KSC_R,
    [0x1F] = SYS_KSC_S,
    [0x14] = SYS_KSC_T,
    [0x16] = SYS_KSC_U,
    [0x2F] = SYS_KSC_V,
    [0x11] = SYS_KSC_W,
    [0x2D] = SYS_KSC_X,
    [0x15] = SYS_KSC_Y,
    [0x2C] = SYS_KSC_Z,

    [0x1D] = SYS_KSC_LEFT_CTRL,
    [0x2A] = SYS_KSC_LEFT_SHIFT,
    [0x36] = SYS_KSC_RIGHT_SHIFT,
    [0x38] = SYS_KSC_LEFT_ALT,
    [0x3A] = SYS_KSC_CAPSLOCK,

    /* Release codes: */
    [0x9D] = SYS_KSC_LEFT_CTRL_R,
    [0xAA] = SYS_KSC_LEFT_SHIFT_R,
    [0xB6] = SYS_KSC_RIGHT_SHIFT_R,
    [0xBB] = SYS_KSC_LEFT_ALT_R,

    [0x3B] = SYS_KSC_F1,
    [0x3C] = SYS_KSC_F2,
    [0x3D] = SYS_KSC_F3,
    [0x3E] = SYS_KSC_F4,
    [0x3F] = SYS_KSC_F5,
    [0x40] = SYS_KSC_F6,
    [0x41] = SYS_KSC_F7,
    [0x42] = SYS_KSC_F8,
    [0x43] = SYS_KSC_F9,
    [0x44] = SYS_KSC_F10,
    [0x57] = SYS_KSC_F11,
    [0x58] = SYS_KSC_F12,

#if 0
    [0x45] = SYS_KSC_NUMLOCK,
    [0x52] = SYS_KSC_NUMPAD_NUMBER_0,
    [0x4F] = SYS_KSC_NUMPAD_NUMBER_1,
    [0x50] = SYS_KSC_NUMPAD_NUMBER_2,
    [0x51] = SYS_KSC_NUMPAD_NUMBER_3,
    [0x4B] = SYS_KSC_NUMPAD_NUMBER_4,
    [0x4C] = SYS_KSC_NUMPAD_NUMBER_5,
    [0x4D] = SYS_KSC_NUMPAD_NUMBER_6,
    [0x47] = SYS_KSC_NUMPAD_NUMBER_7,
    [0x48] = SYS_KSC_NUMPAD_NUMBER_8,
    [0x49] = SYS_KSC_NUMPAD_NUMBER_9,

    [0x4A] = SYS_KSC_NUMPAD_MINUS,
    [0x53] = SYS_KSC_NUMPAD_DOT,
#endif

};

static sys_kb_scn_cds_type sys_scan_codes_map_long[128] =
{
#if 0
    [0x35] = SYS_KSC_NUMPAD_SLASH_FOWARD,
    [0x1C] = SYS_KSC_NUMPAD_ENTER,
#endif

    [0x47] = SYS_KSC_HOME,

    [0x49] = SYS_KSC_PAGE_UP,
    [0x51] = SYS_KSC_PAGE_DOWN,

    [0x48] = SYS_KSC_ARROW_UP,
    [0x4B] = SYS_KSC_ARROW_LEFT,
    [0x4D] = SYS_KSC_ARROW_RIGHT,
    [0x50] = SYS_KSC_ARROW_DOWN,

    [0x4F] = SYS_KSC_END,
    [0x52] = SYS_KSC_INSERT,
    [0x53] = SYS_KSC_DELETE,
};

#include <stdint.h>

#include <kernel/arch/io/ports/x86-ports.h>

#define KEYBOARD_STATUS_PORT         0x64
#define KEYBOARD_DATA_PORT           0x60
#define KEYBOARD_LONG_CODE_PREFIX    0xE0

static uint8_t sys_left_ctrl_flag   = 0x0;
static uint8_t sys_left_shift_flag  = 0x0;
static uint8_t sys_right_shift_flag = 0x0;
static uint8_t sys_left_alt_flag    = 0x0;

static void set_key_flags(sys_kb_scn_cds_type code)
{
    switch (code)
    {
        case SYS_KSC_LEFT_CTRL:
            sys_left_ctrl_flag = 0x1;
            break;

        case SYS_KSC_LEFT_CTRL_R:
            sys_left_ctrl_flag = 0x0;
            break;

        case SYS_KSC_LEFT_SHIFT:
            sys_left_shift_flag = 0x1;
            break;

        case SYS_KSC_LEFT_SHIFT_R:
            sys_left_shift_flag = 0x0;
            break;

        case SYS_KSC_RIGHT_SHIFT:
            sys_right_shift_flag = 0x1;
            break;

        case SYS_KSC_RIGHT_SHIFT_R:
            sys_right_shift_flag = 0x0;
            break;

        case SYS_KSC_LEFT_ALT:
            sys_left_alt_flag = 0x1;
            break;

        case SYS_KSC_LEFT_ALT_R:
            sys_left_alt_flag = 0x0;
            break;
        default:
            break;
    }
}

sys_kb_scn_cds_type __ps2_x86_kb_read_ksc(void)
{
    uint8_t status;
    uint8_t keycode;

    status = inb(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01)
    {
        sys_kb_scn_cds_type code;

        keycode = inb(KEYBOARD_DATA_PORT);
        if (keycode == KEYBOARD_LONG_CODE_PREFIX)
        {
            keycode = inb(KEYBOARD_DATA_PORT);
            code = sys_scan_codes_map_long[keycode];
        }
        else
        {
            code = sys_scan_codes_map_short[keycode];
        }
        set_key_flags(code);

        return code;
    }
    return SYS_KSC_NULL;
}

int __ps2_x86_kb_is_key_prssd(enum sys_kb_scn_cds code)
{
    switch (code)
    {
        case SYS_KSC_LEFT_CTRL:
            return sys_left_ctrl_flag;
            break;

        case SYS_KSC_LEFT_SHIFT:
            return sys_left_shift_flag;
            break;

        case SYS_KSC_RIGHT_SHIFT:
            return sys_right_shift_flag;
            break;

        case SYS_KSC_LEFT_ALT:
            return sys_left_alt_flag;
            break;
        default:
            break;
    }
    return -1;
}

#include <stdbool.h>

char __ps2_x86_kb_read_ch(void)
{
    sys_kb_scn_cds_type code = __ps2_x86_kb_read_ksc();
    if (code >= 128)
        return 0;
    char ch = (char)code;

    /* TODO: */
    bool shift_key_status =  (__ps2_x86_kb_is_key_prssd(SYS_KSC_LEFT_SHIFT) == 1) ||
                             ( __ps2_x86_kb_is_key_prssd(SYS_KSC_RIGHT_SHIFT) == 1);
    if ((ch >= 97) && (ch <= 122) && shift_key_status)
        ch = ch - 32;
    return ch;
}
