//
// Created by unnamedfurry on 3/29/26.
//
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "raylib.h"

int main_old(void) {
    int sWidth = 800;
    int sHeight = 400;
    InitWindow(sWidth, sHeight, "Mirai Botnet");
    //Font font = LoadFontEx("/usr/share/fonts/TTF/DejaVuSans.ttf", 32, NULL, 0);
    SetTargetFPS(60);

    char text[] = "Boooo!";
    int tSize = 20;
    int tX = sWidth/2-(int)strlen(text)-tSize;
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

/**
*  ----------------------------------------------------------------------------------------------------------------------
*  |    Bits    |    LoadAudioStream()   |   type in SoundCallback   |   value range            |   float translation   |
*  ----------------------------------------------------------------------------------------------------------------------
*  |    32      |    (44100, 32, 1)      |   float*                  |   -1.0 ... +1.0          | -                     |
*  |    16      |    (44100, 16, 1)      |   short*                  |   -32768 ... +32767      | sample * 32767        |
*  |    8       |    (22050, 8, 1)       |   unsigned char*          |   0 ... 255              | 128 + sample * 127    |
*  ----------------------------------------------------------------------------------------------------------------------
**/

// global parameters
//float phase = 0.0f;          // using phase instead of time
//float frequency = 1700.0f;    // start frequency
//float volume = 0.3f;         // volume 0.0 - 1.0
//float duration = 0.02f;
typedef struct {
    float phase;
    float frequency;
    float volume;
    float duration;
} SoundGenerator;
SoundGenerator gen = {0};
AudioStream stream = {0};
void PlaySfx(SoundGenerator *gen, float freq, float vol, float dur){
    gen->frequency = freq;
    gen->volume    = vol;
    gen->duration  = dur;
    gen->phase     = 0.0f;
}
void SoundCallback32(void *buffer, unsigned int frames){
    float *out = (float*)buffer;
    for (unsigned int i = 0; i < frames; i++){
        if (gen.duration <= 0.0f) {
            out[i] = 0;
            continue;
        }
        out[i] = sinf(2.0f * PI * gen.frequency * gen.phase) * gen.volume;
        gen.phase += 1.0f / 44100.0f;
        gen.duration -= 1.0f / 44100.0f;
    }
}
void SoundCallback16(void *buffer, unsigned int frames){
    short *out = (short *)buffer;
    for (unsigned int i = 0; i < frames; i++){
        if (gen.duration <= 0.0f) {
            out[i] = 0;
            continue;
        }
        float envelope = 1.0f;
        if (gen.duration < 0.03f) {
            envelope = gen.duration / 0.03f;
        }
        else if (gen.phase < 0.02f) {
            envelope = gen.phase / 0.02f;
        }
        float sample = sinf(2.0f * PI * gen.frequency * gen.phase) * gen.volume * envelope;
        out[i] = (short)(sample * 32767.0f);
        gen.phase += 1.0f / 44100.0f;
        gen.duration -= 1.0f / 44100.0f;
    }
}
void SoundCallback8(void *buffer, unsigned int frames) {
    unsigned char *out = (unsigned char*)buffer;
    for (unsigned int i = 0; i < frames; i++){
        if (gen.duration <= 0.0f) {
            out[i] = 0;
            continue;
        }
        float sample = sinf(2.0f * PI * gen.frequency * gen.phase) * gen.volume;
        out[i] = (unsigned char)(128.0f + sample * 127.0f);
        gen.phase += 1.0f / 22050.0f;
        gen.duration -= 1.0f / 22050.0f;
    }
}

bool ColorEquals(Color a, Color b) {
    return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
}
typedef enum {
    TETRO_I,
    TETRO_O,
    TETRO_T,
    TETRO_S,
    TETRO_Z,
    TETRO_J,
    TETRO_L,
    TETRO_COUNT
} TetrominoType;
typedef struct {
    TetrominoType type;
    int rotation;
    int x,y;
    Color color;
} Tetromino;
const int shapes[7][4][4][4] = {
    { //TETRO_I
        {{1,0,0,0}, {1,0,0,0}, {1,0,0,0}, {1,0,0,0}}, //0
        {{1,1,1,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}}, //90
        {{0,0,0,1}, {0,0,0,1}, {0,0,0,1}, {0,0,0,1}}, //180
        {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {1,1,1,1}}  //270
    },
    { //TETRO_O
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, //0
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, //90
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, //180
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}  //270
    },
    { //TETRO_T
        {{1,1,1,0}, {0,1,0,0}, {0,0,0,0}, {0,0,0,0}}, //0
        {{0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}, //90
        {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}, //180
        {{1,0,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}}  //270
    },
    { //TETRO_S
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, //0
        {{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}, //90
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, //180
        {{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}  //270
    },
    { //TETRO_Z
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}, //0
        {{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}}, //90
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}, //180
        {{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}}  //270
    },
    { //TETRO_J
        {{0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0}}, //0
        {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}, //90
        {{1,1,0,0}, {1,0,0,0}, {1,0,0,0}, {0,0,0,0}}, //180
        {{1,1,1,0}, {0,0,1,0}, {0,0,0,0}, {0,0,0,0}}  //270
    },
    { //TETRO_L
        {{1,0,0,0}, {1,0,0,0}, {1,1,0,0}, {0,0,0,0}}, //0
        {{1,1,1,0}, {1,0,0,0}, {0,0,0,0}, {0,0,0,0}}, //90
        {{1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}}, //180
        {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}  //270
    }
};
Color board[20][10] = {BLANK};
bool CanPlace (int testX, int testY, int testRotation, Tetromino current) {
    int (*shape)[4] = shapes[current.type][testRotation];
    for (int py=0; py<4; py++) {
        for (int px=0; px<4; px++) {
            if (shape[py][px] == 0) continue;
            int boardX = testX+px;
            int boardY = testY+py;
            if (boardX<0 || boardX>=10 || boardY>=20) {
                return false;
            }
            if (boardY<0) continue;
            if (!ColorEquals(board[boardY][boardX], BLANK)) {
                return false;
            }
        }
    }
    return true;
}
void game_tetris() {
    srand(time(NULL) + clock());
    bool init = false;
    Tetromino current = {0};
    int stepper = 0;
    int score = 0;
    int hardDrop = 0;
    bool gameover = false;
    bool playedSound = false;
    while (!WindowShouldClose()) {
        if (init==false) {
            current.type = rand()%7;
            current.rotation = rand()%4;
            current.x = 4;
            current.y = 0;
            srand(time(NULL) + clock());
            int randomColor = rand()%6;
            switch (randomColor) {
                case 0: current.color = YELLOW; break;
                case 1: current.color = RED; break;
                case 2: current.color = GREEN; break;
                case 3: current.color = BLUE; break;
                case 4: current.color = VIOLET; break;
                case 5: current.color = MAGENTA; break;
            }
            init=true;
        }
        int (*shape)[4] = shapes[current.type][current.rotation];

        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleLines(399, 50, 401, 801, WHITE);
        DrawText(TextFormat("Score: %d", score), 10, 10, 26, gameover==true ? RED : WHITE);

        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                if (shape[i][j] != 0) DrawRectangle((current.x+j)*40+399+1, (current.y+i)*40+50+1, 40-2, 40-2, current.color);
            }
        }
        for (int y=0; y<20; y++) {
            for (int x=0; x<10; x++) {
                DrawRectangle(x*40+399+1, y*40+50+1, 40-2, 40-2, board[y][x]);
            }
        }

        if (gameover==true) {
            Font dFont = GetFontDefault();
            Vector2 tMeasure = MeasureTextEx(dFont, "Game over", 60, 2.0f);
            int tX = (1200 - (int)tMeasure.x) / 2;
            int tY = (900 - (int)tMeasure.y) / 2;
            DrawText("Game over", tX, tY, 60, WHITE);
            current.x=-40;
            current.y=-40;
            if (playedSound==false){ PlaySfx(&gen, 680, 0.6f, 0.3f); playedSound=true; }
        }

        EndDrawing();

        for (int i=0; i<10; i++) {
            if (!ColorEquals(board[0][i], BLANK)) {gameover=true;}
        }
        if (stepper%30==0) {
            if (CanPlace(current.x, current.y+1, current.rotation, current)) {
                current.y+=1;
            } else {
                //Lock piece
                for (int i=0; i<4; i++) {
                    for (int j=0; j<4; j++) {
                        if (shape[i][j] != 0) {
                            int YY = current.y + i;
                            int XX = current.x + j;
                            if (YY >= 0 && YY < 20 && XX >= 0 && XX < 10) {
                                board[YY][XX] = current.color;
                                PlaySfx(&gen, 289, 1.0f, 0.1f);
                            }
                        }
                    }
                }
                //Clear full lines
                for (int y = 19; y >= 0; y--) {
                    bool full = true;
                    for (int x = 0; x < 10; x++) {
                        if (ColorEquals(board[y][x], BLANK)) {
                            full = false;
                            break;
                        }
                    }
                    if (full) {
                        for (int yy = y; yy > 0; yy--) {
                            for (int x = 0; x < 10; x++) {
                                board[yy][x] = board[yy - 1][x];
                            }
                        }
                        for (int x = 0; x < 10; x++) {
                            board[0][x] = BLANK;
                        }
                        score += 10;
                        y++;
                    }
                }
                //Spawn new figure
                if (gameover!=true) {init=false; score+=5;}
            }
        }
        stepper++;
        if (current.rotation>3) current.rotation=0;
        if (current.rotation<0) current.rotation=3;
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            if (CanPlace(current.x+1, current.y, current.rotation, current)) {
                current.x+=1;
                PlaySfx(&gen, 500, 0.4f, 0.03f);
            }
        }
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            if (CanPlace(current.x-1, current.y, current.rotation, current)) {
                current.x-=1;
                PlaySfx(&gen, 500, 0.4f, 0.03f);
            }
        }
        if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) {
            if (CanPlace(current.x, current.y, current.rotation+1, current)) {
                current.rotation+=1;
                PlaySfx(&gen, 500, 0.4f, 0.03f);
            }
        }
        if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) {
            for (int i=0; i<10; i++) {
                if (CanPlace(current.x, current.y+i, current.rotation, current)) {
                    hardDrop=i;
                    PlaySfx(&gen, 500, 0.4f, 0.03f);
                }
            }
        }
        if (hardDrop!=0) {current.y+=hardDrop; hardDrop=0;}
    }
    memset(board, 0, sizeof(board));
}

#define DEG_TO_RAD(deg) ((deg) * M_PI / 180.0f)
#define RAD_TO_DEG(rad) ((rad) * 180.0f / M_PI)
typedef struct { float x; float y; float angle; } Ball;
void game_pingpong() {
    srand(time(NULL) + clock());
    Ball ball;
    float player_plate_x = 525.0f;
    float bot_plate_x = 525.0f;
    int direction = 0;
    ball.x = 1200.0f/2;
    ball.y = 900.0f/2;
    ball.angle=90;
    bool gameover = false;
    int score = 0;
    bool randomized = false;
    float bot_range = 0.0f;
    float bot_target_x = 0.0f;
    float bot_speed = 1.0f;
    bool playedSound = false;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleLines(215, 50, 801, 801, WHITE);
        DrawRectangle((int)bot_plate_x, 70, 160, 10, GREEN);
        DrawRectangle((int)player_plate_x, 821, 160, 10, BLUE);
        DrawCircle((int)ball.x, (int)ball.y, 6, RED);
        DrawText(TextFormat("Score: %d", score), 10, 10, 26, gameover==true ? RED : WHITE);

        switch (direction) {
            case 1:
                if (player_plate_x<854) player_plate_x+=3;
                else direction=2;
                break;
            case 2:
                if (player_plate_x>215) player_plate_x-=3;
                else direction=1;
                break;
        }

        float rad_angle = DEG_TO_RAD(ball.angle);
        ball.x += (float)cos(rad_angle) * 2.0f;
        ball.y += (float)sin(rad_angle) * 2.0f;

        if ((int)ball.y >= 820 && (int)ball.y <= 840 && ball.x >= player_plate_x && ball.x <= player_plate_x+160) {
            float center_distance = ball.x - player_plate_x+165.0f;
            float normalized_range = center_distance / (160/2.0f);
            float max_bounce_angle = 60.0f;
            float bounce_angle_deg = normalized_range * max_bounce_angle;

            score+=10;
            ball.angle += bounce_angle_deg;
            PlaySfx(&gen, 321, 0.8f, 0.1f);
        }
        if ((int)ball.y >= 70 && (int)ball.y <= 80 && ball.x >= bot_plate_x && ball.x <= bot_plate_x+160) {
            float center_distance = ball.x - bot_plate_x+165.0f;
            float normalized_range = center_distance / (160/2.0f);
            float max_bounce_angle = 60.0f;
            float bounce_angle_deg = normalized_range * max_bounce_angle;

            score+=5;
            ball.angle += bounce_angle_deg;
            PlaySfx(&gen, 321, 0.8f, 0.1f);
        }

        if (ball.x <= 215 || ball.x >= 1016) {
            ball.angle = ball.angle+90;
            PlaySfx(&gen, 321, 0.8f, 0.1f);
        }
        if (ball.angle>360) {
            ball.angle = ball.angle-360;
            PlaySfx(&gen, 321, 0.8f, 0.1f);
        }
        if (ball.y >= 851) gameover=true;
        if (ball.y <= 50) {
            player_plate_x = 525;
            bot_plate_x = 525;
            ball.x = 1200.0f/2;
            ball.y = 900.0f/2;
            ball.angle=90;
            score+=50;
            PlaySfx(&gen, 548, 0.6f, 0.1f);
        }

        if (randomized==false){
            bot_range = (rand()%160-80)*10;
            randomized=true;
        }
        float target = ball.x - 80 + (bot_range / 10.0f);
        bot_target_x = bot_target_x + (target - bot_target_x) * 0.1f;
        if (bot_plate_x < bot_target_x) bot_plate_x+=bot_speed;
        else if (bot_plate_x > bot_target_x) bot_plate_x-=bot_speed;
        if (bot_range<0) bot_range++;
        else if (bot_range>0) bot_range--;
        else if (bot_range==0) randomized=false;
        if (bot_plate_x>854) bot_plate_x=854;
        if (bot_plate_x<215) bot_plate_x=215;
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            direction = 1;
            PlaySfx(&gen, 500, 0.4f, 0.03f);
        }
        if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) {
            direction = 2;
            PlaySfx(&gen, 500, 0.4f, 0.03f);
        }

        if (gameover==true) {
            Font dFont = GetFontDefault();
            Vector2 tMeasure = MeasureTextEx(dFont, "Game over", 60, 2.0f);
            int tX = (1200 - (int)tMeasure.x) / 2;
            int tY = (900 - (int)tMeasure.y) / 2;
            DrawText("Game over", tX, tY, 60, WHITE);
            direction=0;
            ball.x = 1200.0f/2;
            ball.y = 900.0f/2;
            ball.angle=0;
            if (playedSound==false){ PlaySfx(&gen, 680, 0.6f, 0.3f); playedSound=true; }
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
    float width = 40.0f;
    float height = 40.0f;
    int appleX = 0;
    int appleY = 0;
    bool drawnApple = false;
    bool eatenApple = false;
    int score = 0;
    bool init = false;
    int direction = 0;
    bool gameover = false;
    int stepper = 0;
    bool playedSound = false;
    int snake_direction = -1;
    while (!WindowShouldClose()) {
        if (init!=true) {
            int x = rand()%19;
            int y = rand()%19;
            snake_direction = rand()%4;
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

        Vector2 square_position = {215.0f,50.0f};
        for (int y=0; y<=19; y++) {
            for (int x=0; x<=19; x++) {
                DrawRectangle((int)square_position.x, (int)square_position.y, (int)width, (int)height, x%2==y%2 ? DARKGRAY : GRAY);
                square_position.x+=width;
            }
            square_position.y+=height;
            square_position.x=215;
        }

        DrawText(TextFormat("Score: %d", score), 10, 10, 26, gameover==true ? RED : WHITE);
        DrawRectangleLines(215, 50, 801, 801, WHITE);

        DrawCircle(appleX*(int)width+235, appleY*(int)height+70, 20, RED);
        DrawCircle(appleX*(int)width+237, appleY*(int)height+66, 12, (Color){ 245, 55, 69, 255 });
        Vector3 start = {(float)appleX*width+235, (float)appleY*height+56, 1.0f};
        Vector3 end = {(float)appleX*width+240, (float)appleY*height+44, 1.0f};
        DrawCapsule(start, end, 4, 1, 1, GREEN);

        for (int i=0; i<snake_length; i++) {
            if (snake[i].x <= -1 || snake[i].x >= 20 || snake[i].y <= -1 || snake[i].y >= 20) gameover=true;
            DrawRectangle(snake[i].x*(int)width+215, snake[i].y*(int)height+50, (int)width, (int)height, i==0 ? BLUE : DARKBLUE);
            if (i==0) {
                switch (snake_direction+1) {
                case 2:
                    DrawCircle(snake[i].x*(int)width+246, snake[i].y*(int)height+60, 6, WHITE);
                    DrawCircle(snake[i].x*(int)width+246, snake[i].y*(int)height+80, 6, WHITE);
                    DrawCircle(snake[i].x*(int)width+249, snake[i].y*(int)height+60, 3, BLACK);
                    DrawCircle(snake[i].x*(int)width+249, snake[i].y*(int)height+80, 3, BLACK);
                    continue;
                case 1:
                    DrawCircle(snake[i].x*(int)width+226, snake[i].y*(int)height+80, 6, WHITE);
                    DrawCircle(snake[i].x*(int)width+226, snake[i].y*(int)height+60, 6, WHITE);
                    DrawCircle(snake[i].x*(int)width+223, snake[i].y*(int)height+80, 3, BLACK);
                    DrawCircle(snake[i].x*(int)width+223, snake[i].y*(int)height+60, 3, BLACK);
                    continue;
                case 3:
                    DrawCircle(snake[i].x*(int)width+226, snake[i].y*(int)height+60, 6, WHITE);
                    DrawCircle(snake[i].x*(int)width+246, snake[i].y*(int)height+60, 6, WHITE);
                    DrawCircle(snake[i].x*(int)width+226, snake[i].y*(int)height+57, 3, BLACK);
                    DrawCircle(snake[i].x*(int)width+246, snake[i].y*(int)height+57, 3, BLACK);
                    continue;
                case 4:
                    DrawCircle(snake[i].x*(int)width+226, snake[i].y*(int)height+80, 6, WHITE);
                    DrawCircle(snake[i].x*(int)width+246, snake[i].y*(int)height+80, 6, WHITE);
                    DrawCircle(snake[i].x*(int)width+226, snake[i].y*(int)height+83, 3, BLACK);
                    DrawCircle(snake[i].x*(int)width+246, snake[i].y*(int)height+83, 3, BLACK);
                    continue;
                }
            } else {
                DrawRectangleLines(snake[i].x*(int)width+215, snake[i].y*(int)height+50, (int)width, (int)height, CLITERAL(Color){ 0, 70, 160, 255 });
            }
        }

        if (stepper%15==0) {
            int last_pos_X = snake[snake_length-1].x;
            int last_pos_Y = snake[snake_length-1].y;

            if (direction!=0) {
                for (int i=snake_length-1; i>0; i--) {
                    if (snake[i].x == appleX && snake[i].y == appleY) drawnApple=false;
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
                PlaySfx(&gen, 300, 0.6f, 0.03f);
                snake_length++;
                snake[snake_length-1].x = last_pos_X;
                snake[snake_length-1].y = last_pos_Y;
                score+=10;
            }
        }
        stepper++;

        if (gameover==true) {
            direction=0;
            Font dFont = GetFontDefault();
            Vector2 tMeasure = MeasureTextEx(dFont, "Game over", 60, 2.0f);
            int tX = (1200 - (int)tMeasure.x) / 2;
            int tY = (900 - (int)tMeasure.y) / 2;
            DrawText("Game over", tX, tY, 60, WHITE);
            if (playedSound==false){ PlaySfx(&gen, 680, 0.6f, 0.3f); playedSound=true; }
        } else {
            if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && direction != 2) {
                direction = 1;
                snake_direction = 1;
                PlaySfx(&gen, 500, 0.4f, 0.03f);
            }
            if ((IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) && direction != 1) {
                direction = 2;
                snake_direction = 0;
                PlaySfx(&gen, 500, 0.4f, 0.03f);
            }
            if ((IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) && direction != 4) {
                direction = 3;
                snake_direction = 2;
                PlaySfx(&gen, 500, 0.4f, 0.03f);
            }
            if ((IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) && direction != 3) {
                direction = 4;
                snake_direction = 3;
                PlaySfx(&gen, 500, 0.4f, 0.03f);
            }
        }

        EndDrawing();
    }
}

int main(void) {
    int wWidth = 1200;
    int wHeight = 900;
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(wWidth, wHeight, "Games 4.0");
    InitAudioDevice();
    SetTargetFPS(60);
    Font dFont = GetFontDefault();

    char topText[] = "Choose a game:";
    int ttSize = 70;
    char g1Text[] = "Ping-Pong";
    int g1tSize = 30;
    char g2Text[] = "Snake";
    int g2tSize = 30;
    char g3Text[] = "Tetris";
    int g3tSize = 30;
    int selector = 1;
    bool drawSoon = false;

    stream = LoadAudioStream(44100, 16, 1);  // sample rate, bit depth, channels
    SetAudioStreamCallback(stream, SoundCallback16);
    PlayAudioStream(stream);
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
        DrawText(g3Text, g3tX, g3tY, g3tSize, WHITE);
        DrawRectangleLines(g1tX-15, g1tY-15, 180, 60, selector==1 ? GREEN : WHITE);
        DrawRectangleLines(g2tX-45, g2tY-15, 180, 60, selector==2 ? GREEN : WHITE);
        DrawRectangleLines(g3tX-40, g3tY-15, 180, 60, selector==3 ? GREEN : WHITE);

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            PlaySfx(&gen, 1700, 0.2f, 0.03f);
            selector+=1;
            if (selector>3) selector=1;
            drawSoon=false;
        }
        if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) {
            PlaySfx(&gen, 1700, 0.2f, 0.03f);
            selector-=1;
            if (selector<1) selector=3;
            drawSoon=false;
        }
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            PlaySfx(&gen, 1700, 0.2f, 0.03f);
            switch (selector) {
                case 1:
                    game_pingpong();
                    break;
                case 2:
                    game_snake();
                    break;
                case 3:
                    game_tetris();
                    break;
            }
        }
        if (drawSoon==true) {
            char soon[] = "The game will be ready soon";
            int sSize = 20;
            Vector2 sMeasure = MeasureTextEx(dFont, soon, sSize, 2.0f);
            int sX = (wWidth - (int)sMeasure.x) /2;
            int sY = wHeight - (int)sMeasure.y;
            DrawText(soon, sX, sY, sSize, WHITE);
        }

        DrawFPS(10,10);
        EndDrawing();
    }

    StopAudioStream(stream);
    UnloadAudioStream(stream);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}