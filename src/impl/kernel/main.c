#include "types.h"

#include "print.h"
#include "nanosleep/nanosleep.h"
#include "cmostime.h"

void show_date_time() {
    struct rtcdate r;
    cmostime(&r);
    pprint_int(0, 23, r.year, PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    pprint_char(4, 23, '-', PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    pprint_int_pad0(5, 23, r.month, 2, PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    pprint_char(7, 23, '-', PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    pprint_int_pad0(8, 23, r.day, 2, PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);

    pprint_int_pad0(11, 23, r.hour, 2, PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    pprint_char(13, 23, ':', PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    pprint_int_pad0(14, 23, r.minute, 2, PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    pprint_char(16, 23, ':', PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    pprint_int_pad0(17, 23, r.second, 2, PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
}

void kernel_main() {
    enable_cursor(13, 15);

    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("\nWelcome to simple OS demo\n\n");
    beep();

    char vendor_str[13];
    CPUID_vendor_string(vendor_str);
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("Detected CPU vendor string: ");
    print_str(vendor_str);

    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("\n\nWaiting for exactly 3 seconds: ");
    nanosleep_init();
    for (int i = 0; i < 30; i++) {
        for (int w = 0; w < 10; ++w) {
            show_date_time();
            show_color_spin();
            show_random_ship();
        }
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

    while (true) {
        show_date_time();
        show_color_spin();
        show_random_ship();
    }
}

