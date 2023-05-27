#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>

#define WIDTH 10
#define HEIGHT 20

int score = 0;
int board[HEIGHT][WIDTH] = {0};

struct Tetromino {
    int x, y;
    int shape[4][4];
};

const struct Tetromino tetrominos[7] = {
    {0, 0, {{0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}}},
    {0, 0, {{1, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}}},
    {0, 0, {{1, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}}},
    {0, 0, {{0, 0, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}}},
    {0, 0, {{0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}}},
    {0, 0, {{1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}}},
    {0, 0, {{0, 1, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}}}
};

void drawBoard() {
    system("cls");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == 0) {
                printf(" ");
            } else {
                printf("#");
            }
        }
        printf("\n");
    }

    printf("Score: %d\n", score);
}

bool checkCollision(const struct Tetromino *tetromino) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetromino->shape[i][j] != 0) {
                int x = tetromino->x + j;
                int y = tetromino->y + i;

                if (x < 0 || x >= WIDTH || y >= HEIGHT || board[y][x] != 0) {
                    return true;
                }
            }
        }
    }

    return false;
}

void mergeTetromino(const struct Tetromino *tetromino) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetromino->shape[i][j] != 0) {
                int x = tetromino->x + j;
                int y = tetromino->y + i;

                board[y][x] = 1;
            }
        }
    }
}

void clearLines() {
    for (int i = HEIGHT - 1; i >= 0; i--) {
        bool lineComplete = true;

        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == 0) {
                lineComplete = false;
                break;
            }
        }

        if (lineComplete) {
            score++;
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < WIDTH; j++) {
                    board[k][j] = board[k - 1][j];
                }
            }
            for (int j = 0; j < WIDTH; j++) {
                board[0][j] = 0;
            }
            i++;
        }
    }
}

int main() {
    bool gameOver = false;
    struct Tetromino currentTetromino = tetrominos[rand() % 7];

    while (!gameOver) {
        drawBoard();

        struct Tetromino nextTetromino = tetrominos[rand() % 7];

        currentTetromino.x = WIDTH / 2 - 2;
        currentTetromino.y = 0;

        while (true) {
            currentTetromino.y++;

            if (checkCollision(&currentTetromino)) {
                currentTetromino.y--;
                mergeTetromino(&currentTetromino);
                clearLines();
                break;
            }

            if (kbhit()) {
                char key = getch();

                if (key == 'a' && !checkCollision(&(struct Tetromino) {currentTetromino.x - 1, currentTetromino.y, currentTetromino.shape})) {
                    currentTetromino.x--;
                } else if (key == 'd' && !checkCollision(&(struct Tetromino) {currentTetromino.x + 1, currentTetromino.y, currentTetromino.shape})) {
                    currentTetromino.x++;
                } else if (key == 's') {
                    while (!checkCollision(&currentTetromino)) {
                        currentTetromino.y++;
                    }
                    currentTetromino.y--;
                    mergeTetromino(&currentTetromino);
                    clearLines();
                    break;
                } else if (key == 'q') {
                    int oldX = currentTetromino.x;
                    currentTetromino.x = currentTetromino.y;
                    currentTetromino.y = WIDTH - oldX - 4;
                } else if (key == 'e') {
                    int oldX = currentTetromino.x;
                    currentTetromino.x = HEIGHT - currentTetromino.y - 1;
                    currentTetromino.y = oldX;
                }
            }

            Sleep(100);
        }

        if (checkCollision(&nextTetromino)) {
            gameOver = true;
        } else {
            currentTetromino = nextTetromino;
        }
    }

    drawBoard();
    printf("Game Over!\n");

    return 0;
}