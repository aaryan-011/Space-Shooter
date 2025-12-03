#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20
#define ENEMY_COUNT 6

void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void drawBorders() {
    for (int i = 0; i < WIDTH; i++) {
        gotoxy(i, 0); printf("#");
        gotoxy(i, HEIGHT); printf("#");
    }
    for (int i = 0; i <= HEIGHT; i++) {
        gotoxy(0, i); printf("#");
        gotoxy(WIDTH - 1, i); printf("#");
    }
}

void showInstructions() {
    system("cls");
    printf("\n\n");
    printf("        *** HOW TO PLAY ***\n\n");
    printf("   A = Move Left\n");
    printf("   D = Move Right\n");
    printf("   SPACE = Shoot\n\n");
    printf("   Shoot enemies before they reach you.\n");
    printf("   Touching an enemy = GAME OVER.\n\n");
    printf("   Press any key to return to menu...");
    _getch();
}

void showMenu() {
    system("cls");
    printf("\n\n\n");
    printf("        *** SPACE SHOOTER ***\n\n");
    printf("       1. Start Game\n");
    printf("       2. Instructions\n");
    printf("       3. Quit\n\n");
    printf("       Choose an option: ");
}

void startGame() {

    int shipX = WIDTH / 2;
    int bulletX = -1, bulletY = -1;

    int enemyX[ENEMY_COUNT];
    int enemyY[ENEMY_COUNT];

    int score = 0;

    srand(time(NULL));

    // Spawn enemies
    for (int i = 0; i < ENEMY_COUNT; i++) {
        enemyX[i] = rand() % (WIDTH - 2) + 1;
        enemyY[i] = rand() % 5 + 1;
    }

    system("cls");

    // GAME LOOP
    while (1) {

        drawBorders();

        // Score
        gotoxy(2, HEIGHT + 1);
        printf("Score: %d  ", score);

        // Ship
        gotoxy(shipX, HEIGHT - 1);
        printf("^");

        // Enemies
        for (int i = 0; i < ENEMY_COUNT; i++) {
            gotoxy(enemyX[i], enemyY[i]);
            printf("V");
        }

        // Bullet
        if (bulletY != -1) {
            gotoxy(bulletX, bulletY);
            printf("|");
        }

        // Input
        if (_kbhit()) {
            char c = _getch();
            if (c == 'a' && shipX > 1) shipX--;
            if (c == 'd' && shipX < WIDTH - 2) shipX++;
            if (c == ' ' && bulletY == -1) {
                bulletX = shipX;
                bulletY = HEIGHT - 2;
            }
        }

        // Movement
        if (bulletY != -1) {
            bulletY--;
            if (bulletY <= 0) bulletY = -1;
        }

        // Enemy movement
        for (int i = 0; i < ENEMY_COUNT; i++) {
            enemyY[i]++;
            if (enemyY[i] >= HEIGHT) {
                enemyY[i] = 1;
                enemyX[i] = rand() % (WIDTH - 2) + 1;
            }
        }

        // --- Improved hitbox collision ---
        for (int i = 0; i < ENEMY_COUNT; i++) {

            int hitboxWidth = 1;
            int hitboxHeight = 1;

            if (abs(bulletX - enemyX[i]) <= hitboxWidth &&
                abs(bulletY - enemyY[i]) <= hitboxHeight) {

                bulletY = -1;

                enemyX[i] = rand() % (WIDTH - 2) + 1;
                enemyY[i] = 1;

                score++;
            }
        }

        // Enemy hits player → GAME OVER
        for (int i = 0; i < ENEMY_COUNT; i++) {
            if (enemyX[i] == shipX && enemyY[i] == HEIGHT - 1) {
                system("cls");
                printf("\n\n   GAME OVER!\n   Final Score: %d\n\n", score);
                printf("Press any key to return to menu...");
                _getch();
                return;
            }
        }

        Sleep(60);
        system("cls");
    }
}

int main() {
    while (1) {
        showMenu();
        char choice = _getch();

        switch (choice) {
            case '1': startGame(); break;
            case '2': showInstructions(); break;
            case '3': system("cls"); return 0;
        }
    }
}
