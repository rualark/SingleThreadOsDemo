#include <stdbool.h>
#include "keyboard.h"
#include "print.h"
#include "types.h"

static ui8 get_input_keycode() {
    ui8 code = 0;
    while (!code) {
        // This is just colored spin as a demo
        show_spin();
        show_date_time();
        code = inb(KEYBOARD_PORT);
    }
    
    // This outputs key code at the bottom of the screen for debug
    pprint_int_pad(75, 23, code, 4, PRINT_COLOR_RED, PRINT_COLOR_BLACK);

    return code;
}

// Returns true if pressed Enter, or false if pressed Esc
bool input_str(char* str, int max_len) {
    bool capslock = false;
    bool shift = false;
    bool alt = false;
    bool ctrl = false;
    bool numlock = true;
    bool scrolllock = false;

    ui8 last_keycode = 0;
    int pos = 0;
    while (true) {
        ui8 c = get_input_keycode();
        // Skip repeating keys
        if (c == last_keycode) {
            continue;
        }
        last_keycode = c;
        if (c == KEY_LEFT_SHIFT_RELEASE) {
            shift = false;
        }
        if (c == KEY_RIGHT_SHIFT_RELEASE) {
            shift = false;
        }
        // Skip depressing other keys
        if (c > 127) {
            continue;
        }
        if (c == KEY_ENTER) {
            str[pos] = '\0';
            return true;
        } else if (c == KEY_ESC) {
            str[pos] = '\0';
            return false;
        } else if (c == KEY_BACKSPACE) {
            if (pos) {
                pos--;
                clear_col();
            }
        } else if (c == KEY_LEFT_SHIFT_PRESS) {
            shift = true;
        } else if (c == KEY_CAPS_LOCK_PRESS) {
            capslock = !capslock;
        } else if ((c >=2 && c <= 13) || (c >= 16 && c <= 27) || (c >= 30 && c <= 41) || (c >= 43 && c <= 53) || c == 57) {
            if (pos + 1 < max_len) {
                char ch;
                if (capslock || shift) {
                    ch = get_ascii_char(c);
                } else {
                    ch = get_ascii_char_lower(c);
                }
                str[pos] = ch;
                print_char(ch);
                pos++;
            }
        }
    }
}

