
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 27
#define WIDTH 82

void print_grid(int grid[][WIDTH], int move_area[][WIDTH]);
int count_neighbors(int grid[][WIDTH], int y, int x);
void copy_board(int grid[][WIDTH]);
void first_grid(int grid[][WIDTH]);
int sleeping_time();

int main() {
    int grid_move[HEIGHT][WIDTH] = {0};
    first_grid(grid_move);

    initscr();
    nodelay(stdscr, true);
    noecho();

    FILE *file = freopen("/dev/tty", "r", stdin);
    if (file != NULL) {
        int grid[HEIGHT][WIDTH] = {0};
        while (1) {
            print_grid(grid, grid_move);
        }
    }
    return 0;
}

void print_grid(int grid[][WIDTH], int move_area[][WIDTH]) {
    napms(sleeping_time());
    clear();

    for (int i = 1; i < HEIGHT - 1; i++) {
        for (int j = 1; j < WIDTH - 1; j++) {
            if (grid[i][j] == 1)
                printw("*");
            else
                printw(" ");
        }
        printw("\n");
    }
    copy_board(grid);
    for (int i = 1; i < HEIGHT - 1; i++) {
        for (int j = 1; j < WIDTH - 1; j++) {
            if (grid[i][j] == 1) {
                if (count_neighbors(grid, i, j) == 2 || count_neighbors(grid, i, j) == 3)
                    move_area[i][j] = grid[i][j];
                if (count_neighbors(grid, i, j) > 3 || count_neighbors(grid, i, j) < 2) move_area[i][j] = 0;
            } else {
                if (count_neighbors(grid, i, j) == 3) move_area[i][j] = 1;
            }
        }
    }

    for (int i = 1; i < HEIGHT - 1; i++) {
        for (int j = 1; j < WIDTH - 1; j++) {
            grid[i][j] = move_area[i][j];
        }
    }

    refresh();
}

int count_neighbors(int grid[][WIDTH], int y, int x) {
    int number = 0;

    number = grid[y - 1][x - 1] + grid[y - 1][x] + grid[y - 1][x + 1] + grid[y][x + 1] + grid[y + 1][x + 1] +
             grid[y + 1][x] + grid[y + 1][x - 1] + grid[y][x - 1];

    return number;
}

void copy_board(int grid[][WIDTH]) {
    grid[0][0] = grid[25][80];
    grid[26][0] = grid[1][80];
    grid[26][81] = grid[1][1];
    grid[0][81] = grid[25][1];

    for (int i = 1; i < WIDTH - 1; i++) {
        grid[0][i] = grid[25][i];
        grid[26][i] = grid[1][i];
    }

    for (int j = 1; j < HEIGHT - 1; j++) {
        grid[j][0] = grid[j][80];
        grid[j][81] = grid[j][1];
    }
}

void first_grid(int grid[][WIDTH]) {
    char symbol;
    int i = 0, j = 0;
    while ((symbol = fgetc(stdin)) != EOF) {
        if (symbol == '\n') {
            j = 0;
            i++;
        } else {
            if (symbol == '1') {
                grid[i][j] = 1;
            } else
                grid[i][j] = 0;
        }
        j++;
    }
}

int sleeping_time() {
    static int sleep = 500;
    char btn = getch();
    if (btn == 'Q' || btn == 'q') {
        exit(1);
    } else if (btn == '+') {
        sleep = sleep - 100;
    } else if (btn == '-') {
        sleep = sleep + 100;
    }
    return sleep;
}