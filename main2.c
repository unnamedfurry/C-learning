//
// Created by unnamedfurry on 3/29/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>
#include "raylib.h"

int main_old(void) {
    int sWidth = 800;
    int sHeight = 400;
    InitWindow(sWidth, sHeight, "Mirai Botnet");
    //Font font = LoadFontEx("/usr/share/fonts/TTF/DejaVuSans.ttf", 32, NULL, 0);
    SetTargetFPS(60);

    char text[] = "Boooo!";
    int tSize = 20;
    int tX = sWidth/2-strlen(text)-tSize;
    int tY = sHeight/2-tSize/2;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) tX+=2;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) tX-=2;
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) tY-=2;
        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) tY+=2;
        DrawText(text, tX, tY, tSize, WHITE);
        //DrawTextEx(font, text, (Vector2){tX, tY}, tSize, 2, WHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void game_pingpong() {

}

void game_snake() {
    int width = 40;
    int height = 40;
    int appleX = 0;
    int appleY = 0;
    bool drawnApple = false;
    bool eatenApple = false;
    int score = 0;
    bool init = false;
    //               y   x
    int snake_coords[20][20];
    int sX = 0;
    int sY = 0;
    int direction = 0;
    while (!WindowShouldClose()) {
        Vector2 square_position = {235,50};
        if (init!=true) {
            for (int y=0; y<=19; y++) {
                for (int x=0; x<=19; x++) {
                    snake_coords[y][x] = 0;
                }
            }
            srand(time(NULL) + clock());
            int i = rand()%19;
            srand(time(NULL) + clock());
            int j = rand()%19;
            snake_coords[i][j] = i*j%2==0?1:0 , j*i%2==0?1:0;
            init=true;
        }
        if (drawnApple==false||eatenApple==true) {
            srand(time(NULL) + clock());
            appleX = rand()%20 * width + 255;
            srand(time(NULL) + clock());
            appleY = rand()%20 * height + 70;
            drawnApple=true;
            eatenApple=false;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText(TextFormat("Score: %d", score), 10, 10, 26, WHITE);
        DrawRectangleLines(235, 50, 801, 801, WHITE);
        for (int y=0; y<=19; y++) {
            for (int x=0; x<=19; x++) {
                DrawRectangle(square_position.x, square_position.y, width, height, x%2==y%2 ? DARKGRAY : GRAY);
                square_position.x+=width;
            }
            square_position.y+=height;
            square_position.x=235;
        }

        for (int y=0; y<=19; y++) {
            for (int x=0; x<=19; x++) {
                if (snake_coords[y][x] == 0) {} else {
                    sX = width*x+235;
                    sY = height*y+50;
                    DrawRectangle(sX, sY, 40, 40, BLUE);
                    if (appleX-255 == sX-235 && appleY-70 == sY-50) {
                        eatenApple=true;
                        score+=10;
                    }
                }
            }
        }
        DrawCircle(appleX, appleY, 20, RED);

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) direction=1;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) direction=2;
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) direction=3;
        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) direction=4;
        switch (direction) {
            case 1:
                sY+=40;
                continue;
            case 2:
                sY-=40;
                continue;
            case 3:
                sX-=40;
                continue;
            case 4:
                sX+=40;
                continue;
        }

        EndDrawing();
    }
}

int main(void) {
    int wWidth = 1200;
    int wHeight = 900;
    InitWindow(wWidth, wHeight, "Games 1.0");
    SetTargetFPS(60);
    Font dFont = GetFontDefault();

    char topText[] = "Choose a game:";
    int ttSize = 70;
    char g1Text[] = "Ping-Pong";
    int g1tSize = 30;
    char g2Text[] = "Snake";
    int g2tSize = 30;
    char g3Text[] = "Soon...";
    int g3tSize = 30;
    int selector = 1;
    bool drawSoon = false;

    while (!WindowShouldClose()) {
        Vector2 ttMeasure = MeasureTextEx(dFont, topText, (float)ttSize, 2.0f);
        int ttX = (wWidth - (int)ttMeasure.x) / 2.15;
        int ttY = wHeight/3 - (int)ttMeasure.y/2;

        Vector2 g1tMeasure = MeasureTextEx(dFont, g1Text, (float)g1tSize, 2.0f);
        int g1tX = wWidth/6 - (int)g1tMeasure.x/2;
        int g1tY = wHeight*2/3 - (int)g1tMeasure.y/2;

        Vector2 g2tMeasure = MeasureTextEx(dFont, g2Text, (float)g2tSize, 2.0f);
        int g2tX = wWidth/2 - (int)g2tMeasure.x/2;
        int g2tY = wHeight*2/3 - (int)g2tMeasure.y/2;

        Vector2 g3tMeasure = MeasureTextEx(dFont, g3Text, (float)g3tSize, 2.0f);
        int g3tX = wWidth*5/6 - (int)g3tMeasure.x/2;
        int g3tY = wHeight*2/3 - (int)g3tMeasure.y/2;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(topText, ttX, ttY, ttSize, WHITE);
        DrawText(g1Text, g1tX, g1tY, g1tSize, WHITE);
        DrawText(g2Text, g2tX, g2tY, g2tSize, WHITE);
        DrawText(g3Text, g3tX, g3tY, g3tSize, GRAY);
        DrawRectangleLines(g1tX-15, g1tY-15, 180, 60, selector==1 ? GREEN : WHITE);
        DrawRectangleLines(g2tX-45, g2tY-15, 180, 60, selector==2 ? GREEN : WHITE);
        DrawRectangleLines(g3tX-45, g3tY-15, 180, 60, selector==3 ? RED : GRAY);

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            selector+=1;
            if (selector>3) selector=1;
            drawSoon=false;
        }
        if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) {
            selector-=1;
            if (selector<1) selector=3;
            drawSoon=false;
        }
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            switch (selector) {
                case 1:
                    game_pingpong();
                    break;
                case 2:
                    game_snake();
                    break;
                case 3:
                    drawSoon = true;
                    break;
            }
        }
        if (drawSoon==true) {
            char soon[] = "The game will be ready soon";
            int sSize = 20;
            Vector2 sMeasure = MeasureTextEx(dFont, soon, sSize, 2.0f);
            int sX = (wWidth - (int)sMeasure.x) /2;
            int sY = wHeight - sMeasure.y;
            DrawText(soon, sX, sY, sSize, WHITE);
        }

        DrawFPS(10,10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}