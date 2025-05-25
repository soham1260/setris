#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>
#include <cstdlib>
#include <ctime> 
#include <vector>
using namespace std;

struct Key {
    Rectangle rect;
    bool isActive;
};

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

    Rectangle standardBtn = { 150, 180, 200, 40 };
    Rectangle customBtn = { 150, 250, 200, 40 };

    std::vector<Key> keys;
    std::vector<Position> activePositions;
    std::vector<vector<Position>> custom_positions;
    int keySize = 80;
    int spacing = 10;
    int startX = 60;
    int startY = 40;
    int count = 1;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            Rectangle r = {
                static_cast<float>(startX + col * (keySize + spacing)),
                static_cast<float>(startY + row * (keySize + spacing)),
                static_cast<float>(keySize),
                static_cast<float>(keySize)
            };
            keys.push_back({r, false});
        }
    }

    Rectangle submitBtn = { 150, 330, 100, 40 };
    Rectangle startBtn = { 270, 300, 100, 40 };

    while (!WindowShouldClose())
    {
        game.handleInput();

        if(game.mode == START)
        {
            if(!game.initialized)
            {
                game.initialized = true;
                game.InitializeStandard();
            }
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
        else if (game.mode == CUSTOM) 
        {
            if(game.customSetup)
            {
                Vector2 mouse = GetMousePosition();
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    for (auto &key : keys) {
                        if (CheckCollisionPointRec(mouse, key.rect)) {
                            key.isActive = !key.isActive;
                        }
                    }

                    if (CheckCollisionPointRec(mouse, submitBtn)) {
                        activePositions.clear();  // Reset previous results
                        for (int i = 0; i < keys.size(); ++i) {
                            if (keys[i].isActive) {
                                int row = i / 3;
                                int col = i % 3;
                                activePositions.push_back(Position(row, col));
                                keys[i].isActive = false;
                            }
                        }
                        if (!activePositions.empty()) {
                            custom_positions.push_back(activePositions);
                        }
                        // For demo: print positions to console
                        for (const auto &pos : activePositions) {
                            std::cout << "(" << pos.row << ", " << pos.col << ")\n";
                        }
                    }

                    if (CheckCollisionPointRec(mouse, startBtn)) {
                        game.customSetup = false;
                        game.initialized = false;
                    }
                }

                // Drawing
                BeginDrawing();
                ClearBackground(RAYWHITE);

                for (const auto &key : keys) {
                    DrawRectangleRec(key.rect, key.isActive ? GREEN : RED);
                    DrawRectangleLinesEx(key.rect, 2, BLACK);
                }

                DrawRectangleRec(submitBtn, DARKGRAY);
                DrawRectangleLinesEx(submitBtn, 2, BLACK);
                DrawText("Submit", submitBtn.x + 15, submitBtn.y + 10, 20, WHITE);

                DrawRectangleRec(startBtn, BLUE);
                DrawRectangleLinesEx(startBtn, 2, BLACK);
                DrawText("Start", startBtn.x + 20, startBtn.y + 10, 20, WHITE);

                EndDrawing();
            }
            else
            {
                if(!game.initialized)
                {
                    for(auto activePositions:custom_positions)
                    {
                        for (const auto &pos : activePositions) {
                            std::cout << "(" << pos.row << ", " << pos.col << ")";
                        }
                        cout<<std::endl;
                    }
                }
                break;
            }
            
        }
        else 
        {
            BeginDrawing();
            ClearBackground(darkBlue);

            DrawTextEx(font,"Choose Mode",{80, 120}, 30,0, WHITE);

            // Draw buttons
            DrawRectangleRec(standardBtn, LIGHTGRAY);
            DrawTextEx(font,"Standard", {standardBtn.x + 20, standardBtn.y + 10}, 20,0, WHITE);

            DrawRectangleRec(customBtn, LIGHTGRAY);
            DrawTextEx(font,"Custom", {customBtn.x + 40, customBtn.y + 10}, 20,0, WHITE);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, standardBtn)) {
                    game.mode = START;
                } else if (CheckCollisionPointRec(mouse, customBtn)) {
                    game.mode = CUSTOM;
                    game.customSetup = true;
                }
            }
            EndDrawing();
        }

    }
    
    CloseWindow();
}