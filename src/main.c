#include <stdio.h>
#include <raylib.h>
#include "raygui.h"

int main() {
    int factor = 100;
    InitWindow(16 * factor, 9 * factor, "Neural Network");

    while (!WindowShouldClose()) {
        BeginDrawing();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}