#include "types.h"
#include "print.h"
#include "cmostime.h"

static ui32 spin = 0;
static char spin_char[] = "|/-\\";

static char block_char[] = "\xB0\xB1\xB2";

void show_color_spin() {
    nanosleep(10000);
    spin++;
    for (ui32 i = 0; i < NUM_COLS; i += 2) {
        pprint_char(i, 24, spin_char[(spin / ((i*i) % 11 + 4)) % 4], (spin / 15 + i) % 15 + 1, PRINT_COLOR_BLACK);
        pprint_char(i+1, 24, spin_char[3 - (spin / ((i*i) % 11 + 4)) % 4], (spin / 15 + i + 1) % 15 + 1, PRINT_COLOR_BLACK);
    }

    ui32 column = rand() % NUM_COLS;
    ui32 color = rand() % 15 + 1;
    pprint_char(column, 22, block_char[rand() % 3], color, PRINT_COLOR_BLACK);
}

#define LEFT_ARROW '\x11'
#define RIGHT_ARROW '\x10'
#define UP_ARROW '\x1E'
#define DOWN_ARROW '\x1F'

struct Fire {
    bool active;
    ui8 direction;
    ui8 column;
    ui8 row;
    ui8 color;
    ui8 speed;
    struct ColorChar history;
};

#define MAX_FIRES 200
struct Fire fire[MAX_FIRES];

static ui8 ship_column = 40;
static ui8 ship_row = 19;
static ui8 ship_color = PRINT_COLOR_WHITE;
static struct ColorChar ship_history = (struct ColorChar) {
    character: ' ',
    color: PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4
};

void new_fire(char ship_char) {
    for (int i = 0; i < MAX_FIRES; ++i) {
        // Find free fire
        if (!fire[i].active) {
            fire[i].active = true;
            fire[i].color = ship_color;
            fire[i].speed = rand() % 12 + 1;
            fire[i].row = ship_row;
            fire[i].column = ship_column;
            fire[i].direction = ship_char;
            // Move fire
            if (fire[i].direction == LEFT_ARROW) {
                fire[i].column--;
            }
            if (fire[i].direction == RIGHT_ARROW) {
                fire[i].column++;
            }
            if (fire[i].direction == UP_ARROW) {
                fire[i].row--;
            }
            if (fire[i].direction == DOWN_ARROW) {
                fire[i].row++;
            }
            fire[i].history = get_screen_color_char(fire[i].column, fire[i].row);
            break;
        }
    }
}

void hide_fires() {
    for (int i = 0; i < MAX_FIRES; ++i) {
        if (fire[i].active) {
            // Restore symbol that was hidden by fire if not color spin
            if (fire[i].row != 22) {
                // Do not remove backspaced symbols
                if (get_screen_color_char(fire[i].column, fire[i].row).character != ' ') {
                    pprint_char(fire[i].column, fire[i].row, fire[i].history.character, fire[i].history.color, PRINT_COLOR_BLACK);
                }
            } else {
                pprint_char(fire[i].column, fire[i].row, '\x09', fire[i].history.color, PRINT_COLOR_BLACK);
            }
        }
    }
}

void move_fires() {
    for (int i = 0; i < MAX_FIRES; ++i) {
        if (fire[i].active) {
            // Move fire
            if (spin % fire[i].speed == 0) {
                if (fire[i].direction == LEFT_ARROW) {
                    if (fire[i].column == 0) {
                        fire[i].active = false;
                        continue;
                    }
                    fire[i].column--;
                }
                if (fire[i].direction == RIGHT_ARROW) {
                    fire[i].column++;
                    if (fire[i].column >= NUM_COLS) {
                        fire[i].active = false;
                        continue;
                    }
                }
                if (fire[i].direction == UP_ARROW) {
                    if (fire[i].row == 0) {
                        fire[i].active = false;
                        continue;
                    }
                    fire[i].row--;
                }
                if (fire[i].direction == DOWN_ARROW) {
                    fire[i].row++;
                    if (fire[i].row >= NUM_COLS) {
                        fire[i].active = false;
                        continue;
                    }
                }
            }
            fire[i].history = get_screen_color_char(fire[i].column, fire[i].row);
        }
    }
}

void show_fires() {
    for (int i = 0; i < MAX_FIRES; ++i) {
        if (fire[i].active) {
            char fire_char = '\x07';
            if (fire[i].column == 0 || fire[i].column == NUM_COLS - 1 || fire[i].row == 0 || fire[i].row == NUM_ROWS - 1) {
                fire_char = '\x0F';
                fire[i].speed = 200;
            }
            // Draw fire
            pprint_char(fire[i].column, fire[i].row, fire_char, fire[i].color, PRINT_COLOR_BLACK);
        }
    }
}

void show_random_ship() {
    // Init
    if (spin == 1) {
        for (int i = 0; i < MAX_FIRES; ++i) {
            fire[i].active = false;
        }
    }
    char ship_char = '*';
    if (spin % 15 == 0) {
        // Restore symbol that was hidden by ship
        pprint_char(ship_column, ship_row, ship_history.character, ship_history.color & 0xF, ship_history.color >> 4 & 0xF);
        // Color ship with contrasting color if not empty space or black
        if (ship_history.character != ' ' && ship_history.character != '\0' && ship_history.color > 0) {
            ship_color = ((ship_history.color & 0xF) + 2) % 16;
            // Do not use dark colors
            if (ship_color < 2) {
                ship_color += 3;
            }
        }
        bool ship_moved = false;
        // Randomly choose movement or orientation of ship
        if (rand() % 2 && ship_column) {
            // Randomly choose if we move ship or just orient it
            if (rand() % 2) {
                ship_column--;
                ship_moved = true;
            }
            ship_char = LEFT_ARROW;
        } else if (ship_column + 1 < NUM_COLS) {
            if (rand() % 2) {
                ship_column++;
                ship_moved = true;
            }
            ship_char = RIGHT_ARROW;
        }
        if (rand() % 2 && ship_row) {
            if (rand() % 2) {
                ship_row--;
                ship_moved = true;
            }
            if (rand() % 2) {
                ship_char = UP_ARROW;
            }
            ship_moved = true;
        } else if (ship_row + 1 < NUM_ROWS) {
            if (rand() % 2) {
                ship_row++;
                ship_moved = true;
            }
            if (rand() % 2) {
                ship_char = DOWN_ARROW;
            }
        }
        // Sometimes randomly change ship color
        if (rand() % 100 == 0) {
            ship_color = rand() % 15 + 1;
        }
        // Save character and color before overwriting with ship
        if (ship_moved) {
            ship_history = get_screen_color_char(ship_column, ship_row);
        } else {
            ship_char = '*';
        }
    }
    hide_fires();
    if (spin % 15 == 0) {
        if (ship_char != '*' && rand() % 1 == 0 && ship_column > 0 && ship_column < NUM_COLS - 1 && ship_row > 0 && ship_row < NUM_ROWS - 1) {
            new_fire(ship_char);
        }
    }
    move_fires();
    show_fires();
    if (spin % 15 == 0) {
        // Draw ship
        pprint_char(ship_column, ship_row, ship_char, ship_color, PRINT_COLOR_BLACK);
    }
}
