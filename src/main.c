#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <time.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define WORLD_WIDTH 10000
#define WORLD_HEIGHT 10000
#define GRID_STEP 100
#define H 0.00001

double inputValue;
double targetInputValue;
float learnRate = 0.2;
bool continuousLearning = false;

Vector2 GetGridCoordinates(Vector2 position) {
    return (Vector2){position.x / GRID_STEP, position.y / GRID_STEP};
}

Vector2 GetWorldCoordinates(Vector2 gridPosition) {
    return (Vector2){gridPosition.x * GRID_STEP, gridPosition.y * GRID_STEP};
}

float fx(float x) {
    return 0.2 * powf(x, 4) + 0.1 * powf(x, 3) - powf(x, 2) + 2;
};

void learn() {
    float h = 0.0001;
    float slope = (fx(targetInputValue + h) - fx(targetInputValue)) / h;
    targetInputValue -= slope * learnRate;
}

void GradientDescentControls() {
    Vector2 anchor01 = { 56, 48 };
    GuiGroupBox((Rectangle){ anchor01.x + 0, anchor01.y + 0, 256, 240 }, "Gradient Descent");
    int randomClicked = GuiButton((Rectangle){ anchor01.x + 64, anchor01.y + 32, 136, 24 }, "Random");
    int learnClicked = GuiButton((Rectangle){ anchor01.x + 64, anchor01.y + 72, 136, 24 }, "Learn");
    GuiSlider((Rectangle){ anchor01.x + 64, anchor01.y + 112, 136, 24 }, "Learn", NULL, &learnRate, 0, 1);
    GuiToggle((Rectangle){ anchor01.x + 64, anchor01.y + 152, 136, 24 }, "Continuous", &continuousLearning);

    if (randomClicked) {
        targetInputValue = ((double)rand() / RAND_MAX) * 6 - 3;
        printf("New target input value: %f\n", targetInputValue);
    }

    if (learnClicked || continuousLearning) {
        learn();
    }

    inputValue = Lerp(inputValue, targetInputValue, 0.01f);
}

void DrawInfiniteGrid(Camera2D camera) {
    Vector2 cameraWorldPos = GetScreenToWorld2D((Vector2){0, 0}, camera);
    Vector2 cameraWorldEndPos = GetScreenToWorld2D((Vector2){GetScreenWidth(), GetScreenHeight()}, camera);

    Vector2 startGrid = GetGridCoordinates(cameraWorldPos);
    Vector2 endGrid = GetGridCoordinates(cameraWorldEndPos);

    int startX = ((int)startGrid.x - 1);
    int endX = ((int)endGrid.x + 1);
    int startY = ((int)startGrid.y - 1);
    int endY = ((int)endGrid.y + 1);

    for (int x = startX; x <= endX; x++) {
        Vector2 worldStart = GetWorldCoordinates((Vector2){x, startY});
        Vector2 worldEnd = GetWorldCoordinates((Vector2){x, endY});
        DrawLine(worldStart.x, worldStart.y, worldEnd.x, worldEnd.y, GRAY);
        char numStr[10];
        snprintf(numStr, sizeof(numStr), "%d", x);
        DrawText(numStr, worldStart.x + 5, 5, 20 / camera.zoom, RAYWHITE);
    }

    for (int y = startY; y <= endY; y++) {
        Vector2 worldStart = GetWorldCoordinates((Vector2){startX, y});
        Vector2 worldEnd = GetWorldCoordinates((Vector2){endX, y});
        DrawLine(worldStart.x, worldStart.y, worldEnd.x, worldEnd.y, GRAY);
        char numStr[10];
        snprintf(numStr, sizeof(numStr), "%d", -y);
        DrawText(numStr, 5, worldStart.y + 5, 20 / camera.zoom, RAYWHITE);
    }

    Vector2 originWorld = GetWorldCoordinates((Vector2){0, 0});
    DrawLine(startX * GRID_STEP, originWorld.y, endX * GRID_STEP, originWorld.y, RED);
    DrawLine(originWorld.x, startY * GRID_STEP, originWorld.x, endY * GRID_STEP, GREEN);
}

void DrawSlopeLine(float x) {
    float y = fx(x);
    float slope = (fx(x + H) - fx(x)) / H;

    // The line equation is y = mx + b, where m is slope and b is y-intercept
    // We want to create a line segment centered at (x, y) with length 'lineLength'
    // To do this, we move half the length in both directions along the line
    // 
    // For point1: 
    //   x1 = x - lineLength/2
    //   dy / dx = m
    //   y2 - y1 = m * (x2 - x1)
    //   y1 - y = m * (x1 - x)
    //   y1 = y + m * (x1 - x)
    //   we substitute x1 = x - lineLength/2
    //   
    //   y1 = y + slope * (-lineLength/2)
    //   y1 = y - (lineLength/2) * slope
    //   This moves back along the line by half the length
    // 
    // For point2:
    //   x2 = x + lineLength/2
    //   y2 = y + (lineLength/2) * slope
    //   This moves forward along the line by half the length

    float lineLength = 1.0f;
    Vector2 point1 = GetWorldCoordinates((Vector2){x - lineLength/2, -(y - lineLength/2 * slope)});
    Vector2 point2 = GetWorldCoordinates((Vector2){x + lineLength/2, -(y + lineLength/2 * slope)});
    Vector2 center = GetWorldCoordinates((Vector2){.x = x, .y = -y});
    DrawLineEx(point1, point2, 4.0f, RED);
    DrawCircleV(center, 5, BLUE);
}

void DrawFunction(Camera2D camera) {
    Vector2 cameraWorldPos = GetScreenToWorld2D((Vector2){-1000, -1000}, camera);
    Vector2 cameraWorldEndPos = GetScreenToWorld2D((Vector2){GetScreenWidth() * 2, GetScreenHeight() * 2}, camera);

    Vector2 startGrid = GetGridCoordinates(cameraWorldPos);
    Vector2 endGrid = GetGridCoordinates(cameraWorldEndPos);

    int startX = startGrid.x;
    int endX = endGrid.x;

    int res = 10;
    Vector2 points[(endX - startX + 1) * res];
    int pointCount = 0;

    for (float x = startX; x <= endX; x+=(1.0f/res)) {
        float y = fx(x);
        Vector2 worldPoint = GetWorldCoordinates((Vector2){x, -y});
        points[pointCount] = worldPoint;
        pointCount++;
    }

    DrawLineStrip(points, pointCount, YELLOW);
}

int main() {
    int factor = 100;
    char* styles_path = "./src/external/raygui-4.0/styles/cyber/style_cyber.rgs";
    InitWindow(16 * factor, 9 * factor, "Gradient Descent 2D Visualizer");
    GuiLoadStyle(styles_path);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    srand(time(NULL));
    inputValue = ((double)rand() / RAND_MAX) * 6 - 3;
    targetInputValue = inputValue;

    Vector2 target = {0, 0};
    Vector2 anchor = {0, 0};
    Vector2 mouseDelta = {0, 0};
    int dragging = 0;
    float zoom = 1.0f;
    float targetZoom = 1.0f;

    while (!WindowShouldClose()) {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        mouseDelta = GetMouseDelta();
        
        Camera2D camera = {
            .target = target,
            .offset = (Vector2){screenWidth/2.0f, screenHeight/2.0f},
            .rotation = 0,
            .zoom = zoom,
        };

        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            targetZoom += wheel * 0.05f;
            if (targetZoom < 0.1f) targetZoom = 0.1f;
            if (targetZoom > 10.0f) targetZoom = 10.0f;
            zoom = Lerp(zoom, targetZoom, 0.1f);
            camera.zoom = zoom;
            Vector2 mouseWorldPosNew = GetScreenToWorld2D(GetMousePosition(), camera);
            target = Vector2Add(target, Vector2Subtract(mouseWorldPos, mouseWorldPosNew));
        } else {
            zoom = Lerp(zoom, targetZoom, 0.1f);
            camera.zoom = zoom;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            dragging = 1;
            anchor = GetScreenToWorld2D(GetMousePosition(), camera);
        } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            dragging = 0;
        }

        if (dragging) {
            Vector2 newPosition = GetScreenToWorld2D(GetMousePosition(), camera);
            target = Vector2Subtract(target, Vector2Subtract(newPosition, anchor));
        }

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        BeginDrawing();

            if (dragging) {
                DrawCircleV(GetWorldToScreen2D(anchor, camera), 10, YELLOW);
            }
            BeginMode2D(camera);
                DrawInfiniteGrid(camera);
                DrawFunction(camera);
                DrawSlopeLine(inputValue);
            EndMode2D();

        GradientDescentControls();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}