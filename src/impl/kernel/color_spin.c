#include "types.h"
#include "print.h"
#include "cmostime.h"

static ui32 spin = 0;
static char spin_char[] = "|/-\\";

void show_spin() {
    nanosleep(10000);
    spin++;
    for (ui32 i = 0; i < 65; i += 2) {
        pprint_char(i, 24, spin_char[(spin / ((i*i) % 11 + 4)) % 4], (spin / 15 + i) % 15 + 1, PRINT_COLOR_BLACK);
        pprint_char(i+1, 24, spin_char[3 - (spin / ((i*i) % 11 + 4)) % 4], (spin / 15 + i + 1) % 15 + 1, PRINT_COLOR_BLACK);
    }

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
    pprint_str(20, 23, "UTC", PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
}
