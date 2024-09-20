#include <stdio.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

double weight1_1, weight2_1;
double weight1_2, weight2_2;

int main() {
    int factor = 100;
    InitWindow(16 * factor, 9 * factor, "Neural Network");

    float value = 0.5;

    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();
        // GuiGroupBox((Rectangle){ 66, 24, 276, 312 }, "STANDARD");
        GuiSlider((Rectangle){ 96, 48, 216, 32 }, TextFormat("%0.2f", value), NULL, &value, 0.0f, 1.0f);
        // value = GuiVerticalSlider((Rectangle){ 120, 120, 24, 192 }, TextFormat("%0.2f", value), NULL, value, 0.0f, 1.0f);
        // value = GuiVerticalSliderBar((Rectangle){ 264, 120, 24, 192 }, TextFormat("%0.2f", value), NULL, value, 0.0f, 1.0f);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}