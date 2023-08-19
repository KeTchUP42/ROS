#ifndef SYS_KSC_H
#define SYS_KSC_H

typedef enum sys_kb_scn_cds
{
    SYS_KSC_NULL = 0,
    SYS_KSC_BACKSPACE = 8,
    SYS_KSC_TAB,

    SYS_KSC_ENTER = 10,                     // \n
    SYS_KSC_ESCAPE = 27,

    SYS_KSC_SPACE = 32,                     // space
    SYS_KSC_QUOTATION_MARK_SINGLE = '\'',   // '
    SYS_KSC_ASTERISK_NUMPAD = '*',          // *

    /* Moved from extended codes for ascii compatibility: */

    SYS_KSC_NUMPAD_PLUS,                    // +

    SYS_KSC_COMMA,                          // ,
    SYS_KSC_MINUS,                          // -
    SYS_KSC_DOT,                            // .
    SYS_KSC_SLASH_FOWARD,                   // /
    SYS_KSC_NUMBER_0,                       // 0
    SYS_KSC_NUMBER_1,                       // 1
    SYS_KSC_NUMBER_2,                       // 2
    SYS_KSC_NUMBER_3,                       // 3
    SYS_KSC_NUMBER_4,                       // 4
    SYS_KSC_NUMBER_5,                       // 5
    SYS_KSC_NUMBER_6,                       // 6
    SYS_KSC_NUMBER_7,                       // 7
    SYS_KSC_NUMBER_8,                       // 8
    SYS_KSC_NUMBER_9,                       // 9
    SYS_KSC_COLON_SEMI = ';',               // ;
    SYS_KSC_EQUAL_TO =  '=',                // =

    SYS_KSC_BRACKET_OPEN = '[',             // [
    SYS_KSC_SLASH_BACKWARD,                 // '\'
    SYS_KSC_BRACKET_CLOSE,                  // ]
    SYS_KSC_GRAVE_ACCENT = '`',             // `

    SYS_KSC_A = 'a',
    SYS_KSC_B,
    SYS_KSC_C,
    SYS_KSC_D,
    SYS_KSC_E,
    SYS_KSC_F,
    SYS_KSC_G,
    /* GCC bug fix: */
    SYS_KSC_h,
    SYS_KSC_I,
    SYS_KSC_J,
    SYS_KSC_K,
    SYS_KSC_L,
    SYS_KSC_M,
    SYS_KSC_N,
    SYS_KSC_O,
    SYS_KSC_P,
    SYS_KSC_Q,
    SYS_KSC_R,
    SYS_KSC_S,
    SYS_KSC_T,
    SYS_KSC_U,
    SYS_KSC_V,
    SYS_KSC_W,
    SYS_KSC_X,
    SYS_KSC_Y,
    SYS_KSC_Z,

    /* Other system codes: */

    SYS_KSC_LEFT_CTRL = 128,
    SYS_KSC_LEFT_SHIFT,
    SYS_KSC_RIGHT_SHIFT,
    SYS_KSC_LEFT_ALT,
    SYS_KSC_CAPSLOCK,

    SYS_KSC_LEFT_CTRL_R,
    SYS_KSC_LEFT_SHIFT_R,
    SYS_KSC_RIGHT_SHIFT_R,
    SYS_KSC_LEFT_ALT_R,

    SYS_KSC_F1,
    SYS_KSC_F2,
    SYS_KSC_F3,
    SYS_KSC_F4,
    SYS_KSC_F5,
    SYS_KSC_F6,
    SYS_KSC_F7,
    SYS_KSC_F8,
    SYS_KSC_F9,
    SYS_KSC_F10,
    SYS_KSC_F11,
    SYS_KSC_F12,

    SYS_KSC_NUMLOCK,
    SYS_KSC_NUMPAD_NUMBER_0,                // 0
    SYS_KSC_NUMPAD_NUMBER_1,                // 1
    SYS_KSC_NUMPAD_NUMBER_2,                // 2
    SYS_KSC_NUMPAD_NUMBER_3,                // 3
    SYS_KSC_NUMPAD_NUMBER_4,                // 4
    SYS_KSC_NUMPAD_NUMBER_5,                // 5
    SYS_KSC_NUMPAD_NUMBER_6,                // 6
    SYS_KSC_NUMPAD_NUMBER_7,                // 7
    SYS_KSC_NUMPAD_NUMBER_8,                // 8
    SYS_KSC_NUMPAD_NUMBER_9,                // 9

    SYS_KSC_NUMPAD_MINUS,                   // -
    SYS_KSC_NUMPAD_SLASH_FOWARD,            // /
    SYS_KSC_NUMPAD_ENTER,
    SYS_KSC_NUMPAD_DOT,                     // .

    SYS_KSC_HOME,
    SYS_KSC_ARROW_UP,
    SYS_KSC_ARROW_LEFT,
    SYS_KSC_ARROW_RIGHT,
    SYS_KSC_ARROW_DOWN,

    SYS_KSC_PAGE_UP,
    SYS_KSC_PAGE_DOWN,

    SYS_KSC_END,
    SYS_KSC_INSERT,
    SYS_KSC_DELETE,

} sys_kb_scn_cds_type;

#endif // SYS_KSC_H
