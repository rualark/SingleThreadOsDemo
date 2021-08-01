#include "types.h"

#include "print.h"
#include "nanosleep/nanosleep.h"

void kernel_main() {
    enable_cursor(13, 15);

    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to simple OS demo\n\n");
    beep();

    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("Waiting exactly for 3 seconds: ");
    nanosleep_init();
    for (int i = 0; i < 30; i++) {
        nanosleep(100000);
        print_str(".");
    }

    print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    print_str("\n\nPlease enter your name: ");
    int max_len = 50;
    char str[max_len];
    if (input_str(str, max_len)) {
        print_str("\nYour name is: ");
        print_str(str);
    }

    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    print_str("\n\nGood bye!");
}
