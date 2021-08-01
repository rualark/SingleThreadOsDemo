#include "types.h"
#include "print.h"
#include "cmostime.h"

static ui32 spin = 0;
static char spin_char[] = "|/-\\";

static char block_char[] = "\xB0\xB1\xB2";

void show_color_spin() {
    nanosleep(10000);
    spin++;
    for (ui32 i = 0; i < 80; i += 2) {
        pprint_char(i, 24, spin_char[(spin / ((i*i) % 11 + 4)) % 4], (spin / 15 + i) % 15 + 1, PRINT_COLOR_BLACK);
        pprint_char(i+1, 24, spin_char[3 - (spin / ((i*i) % 11 + 4)) % 4], (spin / 15 + i + 1) % 15 + 1, PRINT_COLOR_BLACK);
    }

    ui32 column = rand() % 80;
    ui32 color = rand() % 15 + 1;
    pprint_char(column, 22, block_char[rand() % 3], color, PRINT_COLOR_BLACK);
}

static ui8 beetle_column = 40;
static ui8 beetle_row = 19;
static ui8 beetle_color = PRINT_COLOR_WHITE;
static struct ColorChar beetle_history = (struct ColorChar) {
    character: ' ',
    color: PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4
};

void show_random_beetle() {
    if (spin % 15 == 0) {
        pprint_char(beetle_column, beetle_row, beetle_history.character, beetle_history.color & 0xF, beetle_history.color >> 4 & 0xF);
        if (beetle_history.character != ' ' && beetle_history.character != '\0') {
            beetle_color = beetle_history.color;
        }
        bool beetle_moved = false;
        char beetle_char = '*';
        if (rand() % 2 && beetle_column) {
            if (rand() % 2) {
                beetle_column--;
                beetle_moved = true;
            }
            beetle_char = '\x11';
        } else if (beetle_column < 79) {
            if (rand() % 2) {
                beetle_column++;
                beetle_moved = true;
            }
            beetle_char = '\x10';
        }
        if (rand() % 2 && beetle_row) {
            if (rand() % 2) {
                beetle_row--;
                beetle_moved = true;
            }
            if (rand() % 2) {
                beetle_char = '\x1E';
            }
            beetle_moved = true;
        } else if (beetle_row < 24) {
            if (rand() % 2) {
                beetle_row++;
                beetle_moved = true;
            }
            if (rand() % 2) {
                beetle_char = '\x1F';
            }
        }
        if (rand() % 100 == 0) {
            beetle_color = rand() % 15 + 1;
        }
        if (beetle_moved) {
            beetle_history = get_screen_color_char(beetle_column, beetle_row);
        } else {
            beetle_char = '*';
        }
        pprint_char(beetle_column, beetle_row, beetle_char, beetle_color, PRINT_COLOR_BLACK);
    }
}
