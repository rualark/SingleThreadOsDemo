#include "types.h"
#include "print.h"
#include "cmostime.h"

static ui32 spin = 0;
static char spin_char[] = "|/-\\";

void show_spin() {
    nanosleep(10000);
    spin++;
    for (ui32 i = 0; i < 80; i += 2) {
        pprint_char(i, 24, spin_char[(spin / ((i*i) % 11 + 4)) % 4], (spin / 15 + i) % 15 + 1, PRINT_COLOR_BLACK);
        pprint_char(i+1, 24, spin_char[3 - (spin / ((i*i) % 11 + 4)) % 4], (spin / 15 + i + 1) % 15 + 1, PRINT_COLOR_BLACK);
    }
}
