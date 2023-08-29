#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define ALIVE '*'
#define DEAD '-'

bool is_field_empty(int** field);
void game_of_life(FILE* file);
void draw_field(int** field);
void clear_screen();
void initialize(int*** field, FILE* file);
int check_dead_alive(int** field, int x, int y);
void koord_x(int* koor, int x);
void koord_y(int* koor, int y);
int count_neighbours(int** field, int x, int y);
void field_update(int*** field);

int main(int argc, char* argv[]) {
    FILE* file = NULL;
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    if (argc) {
        file = fopen(argv[1], "r");
    }
    game_of_life(file);
    endwin();
    return 0;
}

// Функция игры
void game_of_life(FILE* file) {
    int** field = NULL;
    bool stopped = false;
    int speed = 100000;
    initialize(&field, file);
    while (!stopped) {
        clear();
        draw_field(field);
        if (is_field_empty(field)) {
            stopped = true;
        } else {
            field_update(&field);
            usleep(speed);  // Задержка перед следующим шагом
        }
        int key = getch();
        switch (key) {
            case 'a':
                speed += 20000;
                break;
            case 'd': {
                if (speed - 20000 >= 50000) speed -= 20000;
                break;
            }
            case 'q': {
                stopped = true;
            }
        }
        refresh();
    }

    if (is_field_empty(field)) {
        printw("The game is over: there are no alive cells\n");
    }
    free(field);
}

// Функция инициализации начального состояния
void initialize(int*** field, FILE* file) {
    char cell;
    *field = malloc(WIDTH * HEIGHT * sizeof(int) + HEIGHT * sizeof(int*));
    int* start = (int*)(*field + HEIGHT);
    for (int i = 0; i < HEIGHT; i++) {
        (*field)[i] = start + i * WIDTH;
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            fscanf(file, "%c", &cell);
            if (cell == ALIVE) {
                (*field)[i][j] = 1;
            } else {
                (*field)[i][j] = 0;
            }
        }
    }
}

// Функция отрисовки поля
void draw_field(int** field) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j]) {
                printw("%c", ALIVE);
            } else {
                printw("%c", DEAD);
            }
        }
        printw("\n");
    }
    fflush(stdout);
}

// Функция обновления поля
void field_update(int*** field) {
    int** new_field = malloc(WIDTH * HEIGHT * sizeof(int) + HEIGHT * sizeof(int*));
    int* start = (int*)(new_field + HEIGHT);
    for (int i = 0; i < HEIGHT; i++) {
        new_field[i] = start + i * WIDTH;
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            new_field[i][j] = check_dead_alive(*field, i, j);
        }
    }
    free(*field);
    *field = new_field;
}

// Функция подсчета соседей
int count_neighbours(int** field, int x, int y) {
    int count;
    int koor_x[8];
    int koor_y[8];
    koord_x(koor_x, x);
    koord_y(koor_y, y);
    count = field[koor_x[0]][koor_y[0]] + field[koor_x[1]][koor_y[1]] + field[koor_x[2]][koor_y[2]] +
            field[koor_x[3]][koor_y[3]] + field[koor_x[4]][koor_y[4]] + field[koor_x[5]][koor_y[5]] +
            field[koor_x[6]][koor_y[6]] + field[koor_x[7]][koor_y[7]];
    return count;
}

// Функция подсчета координат x
void koord_x(int* koor, int x) {
    koor[3] = x;
    koor[4] = x;
    if (x != 0 && x != HEIGHT - 1) {
        koor[0] = x - 1;
        koor[1] = x - 1;
        koor[2] = x - 1;
        koor[5] = x + 1;
        koor[6] = x + 1;
        koor[7] = x + 1;
    } else if (x == 0) {
        koor[0] = HEIGHT - 1;
        koor[1] = HEIGHT - 1;
        koor[2] = HEIGHT - 1;
        koor[5] = 1;
        koor[6] = 1;
        koor[7] = 1;
    } else {
        koor[0] = HEIGHT - 2;
        koor[1] = HEIGHT - 2;
        koor[2] = HEIGHT - 2;
        koor[5] = 0;
        koor[6] = 0;
        koor[7] = 0;
    }
}

// Функция подсчета координат y
void koord_y(int* koor, int y) {
    koor[1] = y;
    koor[6] = y;
    if (y != 0 && y != WIDTH - 1) {
        koor[0] = y - 1;
        koor[2] = y + 1;
        koor[3] = y - 1;
        koor[4] = y + 1;
        koor[5] = y - 1;
        koor[7] = y + 1;
    } else if (y == 0) {
        koor[0] = WIDTH - 1;
        koor[2] = 1;
        koor[3] = WIDTH - 1;
        koor[4] = 1;
        koor[5] = WIDTH - 1;
        koor[7] = 1;
    } else {
        koor[0] = WIDTH - 2;
        koor[2] = 0;
        koor[3] = WIDTH - 2;
        koor[4] = 0;
        koor[5] = WIDTH - 2;
        koor[7] = 0;
    }
}

// Проверка жизни/смерти клетки
int check_dead_alive(int** field, int x, int y) {
    int count = count_neighbours(field, x, y);
    int now = field[x][y];
    int state;
    if ((now == 0 && count == 3) || (now == 1 && (count == 2 || count == 3))) {
        state = 1;
    } else {
        state = 0;
    }
    return state;
}

bool is_field_empty(int** field) {
    bool ret = true;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j]) {
                ret = false;
                break;
            }
        }
        if (!ret) {
            break;
        }
    }
    return ret;
}
