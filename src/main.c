#include <stdio.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

float weight1_1, weight2_1;
float weight1_2, weight2_2;

int DrawWeightControls() {
    Vector2 anchor01 = { 56, 48 };
    GuiGroupBox((Rectangle){ anchor01.x + 0, anchor01.y + 0, 256, 200 }, "Weights");
    GuiSlider((Rectangle){ anchor01.x + 64, anchor01.y + 32, 136, 24 }, "W1,1", NULL, &weight1_1, 0, 100);
    GuiSlider((Rectangle){ anchor01.x + 64, anchor01.y + 72, 136, 24 }, "W1,2", NULL, &weight1_2, 0, 100);
    GuiSlider((Rectangle){ anchor01.x + 64, anchor01.y + 112, 136, 24 }, "W2,1", NULL, &weight2_1, 0, 100);
    GuiSlider((Rectangle){ anchor01.x + 64, anchor01.y + 152, 136, 24 }, "W2,2", NULL, &weight2_2, 0, 100);
}

int main() {
    int factor = 100;
    char* styles_path = "./src/external/raygui-4.0/styles/cyber/style_cyber.rgs";
    InitWindow(16 * factor, 9 * factor, "Neural Network");
    GuiLoadStyle(styles_path);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    float value = 0.0;

    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();
        DrawWeightControls();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}