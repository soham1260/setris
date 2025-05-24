#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>
#include <cstdlib>
#include <ctime> 
using namespace std;

double prev_time = 0;
bool EventTriggered(double interval)
{
    double cur_time = GetTime();
    if(cur_time - prev_time >= interval)
    {
        prev_time = cur_time;
        return true;
    }
    return false;
}

int main()
{
    InitWindow(500,620, "Tetris");
    SetTargetFPS(60);

    srand(time(0));
    
    Font font = LoadFontEx("D:/tetris/font/font.ttf",64,0,0);

    Game game = Game();

    while (!WindowShouldClose())
    {
        game.handleInput();

        if(EventTriggered(0.2)) // move block down only after 0.2s
        {
            game.MoveBlockDown();
        }
        //remember everything is first calculated then drawn. Nothing is calculated while drawing.
        BeginDrawing();

        ClearBackground(darkBlue);
        
        DrawTextEx(font,"Score",{340,20},24,2,WHITE);

        DrawRectangleRounded({320,55,170,60},0.3,6,lightBlue);

        char score[10];
        sprintf(score,"%d",game.score);
        Vector2 textSize = MeasureTextEx(font,score,24,2);
        DrawTextEx(font,score,{320+(170-textSize.x)/2,75},24,2,WHITE);

        DrawTextEx(font,"Next",{355,180},24,2,WHITE);
        DrawRectangleRounded({320,215,170,180},0.3,6,lightBlue);
        if(game.gameOver) DrawTextEx(font,"Game Over",{320,450},18,2,WHITE);
        game.Draw();
        EndDrawing();
    }
    
    CloseWindow();
}