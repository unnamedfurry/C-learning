//
// Created by unnamedfurry on 3/29/26.
//
#include <math.h>
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

#define PI M_PI
#define DEG_TO_RAD(deg) ((deg) * PI / 180.0f)
#define RAD_TO_DEG(rad) ((rad) * 180.0f / PI)
typedef struct { float x; float y; float angle; } Ball;
void game_pingpong() {
    Ball ball;
    int player_plate_x = 525;
    int bot_plate_x = 525;
    int direction = 0;
    ball.x = 1200.0f/2;
    ball.y = 900.0f/2;
    ball.angle=90;
    bool gameover = false;
    int score = 0;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleLines(215, 50, 801, 801, WHITE);
        DrawRectangle(bot_plate_x, 70, 160, 10, WHITE);
        DrawRectangle(player_plate_x, 821, 160, 10, WHITE);
        DrawCircle((int)ball.x, (int)ball.y, 6, WHITE);
        DrawText(TextFormat("Score: %d", score), 10, 10, 26, gameover==true ? RED : WHITE);

        switch (direction) {
            case 1:
                if (player_plate_x<854) player_plate_x+=3;
                break;
            case 2:
                if (player_plate_x>215) player_plate_x-=3;
                break;
        }

        float rad_angle = DEG_TO_RAD(ball.angle);
        ball.x += cos(rad_angle) * 2;
        ball.y += sin(rad_angle) * 2;

        if ((int)ball.y >= 820 && (int)ball.y <= 840 && (int)ball.x >= player_plate_x && (int)ball.x <= player_plate_x+160) {
            float center_distance = ball.x - (float)player_plate_x+165.0f;
            float normalized_range = center_distance / (160/2.0f);
            float max_bounce_angle = 60.0f;
            float bounce_angle_deg = normalized_range * max_bounce_angle;

            score+=10;
            ball.angle += bounce_angle_deg;
        }
        if ((int)ball.y >= 70 && (int)ball.y <= 80 && (int)ball.x >= bot_plate_x && (int)ball.x <= bot_plate_x+160) {
            float center_distance = ball.x - (float)bot_plate_x+165.0f;
            float normalized_range = center_distance / (160/2.0f);
            float max_bounce_angle = 60.0f;
            float bounce_angle_deg = normalized_range * max_bounce_angle;

            ball.angle += bounce_angle_deg;
        }

        if (ball.x <= 215 || ball.x >= 1016) ball.angle = ball.angle+90;
        if (ball.y >= 851) gameover=true;

        //bot_plate_x = (int)ball.x + rand()%160-80;
        bot_plate_x = (int)ball.x-80;
        if (bot_plate_x>854) bot_plate_x=854;
        if (bot_plate_x<215) bot_plate_x=215;

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) direction = 1;
        if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) direction = 2;

        if (gameover==true) {
            Font dFont = GetFontDefault();
            Vector2 tMeasure = MeasureTextEx(dFont, "Game over", 60, 2.0f);
            int tX = (1200 - (int)tMeasure.x) / 2;
            int tY = (900 - (int)tMeasure.y) / 2;
            DrawText("Game over", tX, tY, 60, RED);
            direction=0;
            ball.x = 1200.0f/2;
            ball.y = 900.0f/2;
            ball.angle=0;
        }

        EndDrawing();
    }
}

typedef struct { int x; int y; } Point;
#define MAX_LENGTH 400
void game_snake() {
    srand(time(NULL) + clock());
    Point snake[MAX_LENGTH];
    int snake_length = 3;
    int width = 40;
    int height = 40;
    int appleX = 0;
    int appleY = 0;
    bool drawnApple = false;
    bool eatenApple = false;
    int score = 0;
    bool init = false;
    int direction = 0;
    bool gameover = false;
    int stepper = 0;
    while (!WindowShouldClose()) {
        if (init!=true) {
            int x = rand()%19;
            int y = rand()%19;
            int snake_direction = rand()%4;
            snake[0].x = x;
            snake[0].y = y;
            switch (snake_direction) {
                case 0:
                    snake[1].x = x+1;
                    snake[1].y = y;
                    snake[2].x = x+2;
                    snake[2].y = y;
                    init=true;
                    continue;
                case 1:
                    snake[1].x = x-1;
                    snake[1].y = y;
                    snake[2].x = x-2;
                    snake[2].y = y;
                    init=true;
                    continue;
                case 2:
                    snake[1].x = x;
                    snake[1].y = y+1;
                    snake[2].x = x;
                    snake[2].y = y+2;
                    init=true;
                    continue;
                case 3:
                    snake[1].x = x;
                    snake[1].y = y-1;
                    snake[2].x = x;
                    snake[2].y = y-2;
                    init=true;
                    continue;
            }
            init=true;
        }
        if (drawnApple==false||eatenApple==true) {
            appleX = rand()%19;
            appleY = rand()%19;
            drawnApple=true;
            eatenApple=false;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        Vector2 square_position = {235,50};
        for (int y=0; y<=19; y++) {
            for (int x=0; x<=19; x++) {
                DrawRectangle(square_position.x, square_position.y, width, height, x%2==y%2 ? DARKGRAY : GRAY);
                square_position.x+=width;
            }
            square_position.y+=height;
            square_position.x=235;
        }

        DrawText(TextFormat("Score: %d", score), 10, 10, 26, gameover==true ? RED : WHITE);
        DrawRectangleLines(235, 50, 801, 801, WHITE);

        DrawCircle(appleX*width+255, appleY*height+70, 20, RED);

        if (stepper%15==0) {
            int last_pos_X = snake[snake_length-1].x;
            int last_pos_Y = snake[snake_length-1].y;

            if (direction!=0) {
                for (int i=snake_length-1; i>0; i--) {
                    snake[i] = snake[i-1];
                    if (snake[0].x == snake[i+1].x && snake[0].y == snake[i+1].y) {
                        gameover=true;
                    }
                }
            }

            switch (direction) {
                case 1: snake[0].x+=1; break;
                case 2: snake[0].x-=1; break;
                case 3: snake[0].y-=1; break;
                case 4: snake[0].y+=1; break;
                case 0: break;
            }

            if (snake[0].x == appleX && snake[0].y == appleY) eatenApple=true;
            if (eatenApple==true) {
                snake_length++;
                snake[snake_length-1].x = last_pos_X;
                snake[snake_length-1].y = last_pos_Y;
                score+=10;
            }
        }
        stepper++;

        for (int i=0; i<snake_length; i++) {
            if (snake[i].x <= -1 || snake[i].x >= 20 || snake[i].y <= -1 || snake[i].y >= 20) {
                gameover=true;
                score=0;
            }
            DrawRectangle(snake[i].x*width+235, snake[i].y*height+50, width, height, i==0 ? BLUE : DARKBLUE);
        }

        if (gameover==true) {
            direction=0;
            Font dFont = GetFontDefault();
            Vector2 tMeasure = MeasureTextEx(dFont, "Game over", 60, 2.0f);
            int tX = (1200 - (int)tMeasure.x) / 2;
            int tY = (900 - (int)tMeasure.y) / 2;
            DrawText("Game over", tX, tY, 60, RED);
        } else {
            if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && direction != 2) direction = 1;
            if ((IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) && direction != 1) direction = 2;
            if ((IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) && direction != 4) direction = 3;
            if ((IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) && direction != 3) direction = 4;
        }

        EndDrawing();
    }
}

int main(void) {
    int wWidth = 1200;
    int wHeight = 900;
    SetConfigFlags(FLAG_VSYNC_HINT);
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