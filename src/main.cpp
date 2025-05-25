#include <raylib.h>
#include "game.h"
#include "colors.h"
#include "toggleButton.h"
#include <iostream>
#include <cstdlib>
#include <ctime> 
#include <vector>

struct Key {
    Rectangle rect;
    bool isActive;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void render(Game &game, Font font)
{
    if(EventTriggered(0.2)) // move block down only after 0.2s
    {
        game.MoveBlockDown();
    }
    //remember everything is first calculated then drawn. Nothing is calculated while drawing.
    BeginDrawing();

    ClearBackground(darkBlue);
                    
    DrawTextEx(font,"Score",{340,60},24,2,WHITE);

    DrawRectangleRounded({320,95,170,60},0.3,6,lightBlue);

    char score[10];
    sprintf(score,"%d",game.score);
    Vector2 textSize = MeasureTextEx(font,score,24,2);
    DrawTextEx(font,score,{320+(170-textSize.x)/2,115},24,2,WHITE);

    DrawTextEx(font,"Next",{355,220},24,2,WHITE);
    DrawRectangleRounded({320,255,170,180},0.3,6,lightBlue);
    if(game.gameOver) DrawTextEx(font,"Game Over",{320,490},18,2,WHITE);
    game.Draw();
    EndDrawing();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    InitWindow(500,620, "Setris");
    SetTargetFPS(60);

    srand(time(0));
    
    Font font = LoadFontEx("D:/tetris/font/font.ttf",64,0,0);

    Game game = Game();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Rectangle standardBtn = { 150, 300, 200, 40 };
    Rectangle customBtn = { 150, 370, 200, 40 };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<Key> keys;
    std::vector<Position> activePositions;
    std::vector<std::vector<Position>> custom_positions;
    int keySize = 80;
    int spacing = 10;
    int startX = 20;
    int startY = 20;
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
    Rectangle randomBtn = { 290, 60, 200, 40 };
    Rectangle submitBtn = { 290, 120, 200, 40 };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ToggleButton left_Toggle(350, 300, 120, 30);
    ToggleButton right_Toggle(350, 340, 120, 30);
    ToggleButton up_Toggle(350, 380, 120, 30);
    ToggleButton down_Toggle(350, 420, 120, 30);
    ToggleButton left_rotate_Toggle(350, 460, 120, 30);
    ToggleButton right_rotate_Toggle(350, 500, 120, 30);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string inputText = "";
    bool isInputActive = false;
    Rectangle inputBox = { 350, 540, 120, 30 };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Rectangle startBtn = { 140, 575, 220, 40 };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
            render(game,font);
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
                        for (int i = 0; i < keys.size(); ++i) {//iterates over all 9 buttons
                            if (keys[i].isActive) {
                                int row = i / 3;
                                int col = i % 3;
                                activePositions.push_back(Position(row, col));
                                keys[i].isActive = false;
                            }
                        }
                        if (!activePositions.empty()) {
                            game.totalBlocks++;
                            custom_positions.push_back(activePositions);
                        }
                        // For demo: print positions to console
                        for (const auto &pos : activePositions) {
                            std::cout << "(" << pos.row << ", " << pos.col << ")\n";
                        }
                    }

                    isInputActive = CheckCollisionPointRec(mouse, inputBox);

                    if (CheckCollisionPointRec(mouse, startBtn) && game.totalBlocks>0) {
                        game.customSetup = false;
                        game.initialized = false;
                    }

                    if (CheckCollisionPointRec(mouse, randomBtn)) {
                        
                    }
                }

                if (isInputActive) {
                    int key = GetCharPressed();
                    while (key > 0) {
                        if (key >= '0' && key <= '9') {
                            inputText += (char)key;
                        }
                        key = GetCharPressed();
                    }

                    // Backspace
                    if (IsKeyPressed(KEY_BACKSPACE) && !inputText.empty()) {
                        inputText.pop_back();
                    }
                }
                left_Toggle.Update(mouse);
                right_Toggle.Update(mouse);
                up_Toggle.Update(mouse);
                down_Toggle.Update(mouse);
                left_rotate_Toggle.Update(mouse);
                right_rotate_Toggle.Update(mouse);

                // Drawing
                BeginDrawing();
                ClearBackground(darkBlue);

                for (const auto &key : keys) {
                    DrawRectangleRec(key.rect, key.isActive ? GREEN : RED);
                    DrawRectangleLinesEx(key.rect, 2, BLACK);
                }

                DrawRectangleRec(randomBtn, DARKGRAY);
                DrawRectangleLinesEx(randomBtn, 2, BLACK);
                DrawTextEx(font,"Random", {randomBtn.x + 45, randomBtn.y + 10}, 18,0, WHITE);

                DrawRectangleRec(submitBtn, DARKGRAY);
                DrawRectangleLinesEx(submitBtn, 2, BLACK);
                DrawTextEx(font,"Add Block", {submitBtn.x + 20, submitBtn.y + 10}, 18,0, WHITE);

                std::string blockCountStr = std::to_string(game.totalBlocks);
                DrawTextEx(font, "Total Blocks ", {290, 180}, 16, 0, WHITE);
                DrawTextEx(font, blockCountStr.c_str(), {380, 220}, 20, 0, WHITE);

                DrawTextEx(font, "Move left ", {20, 305}, 20, 0, WHITE);
                left_Toggle.Draw();
                DrawTextEx(font, "Move right ", {20, 345}, 20, 0, WHITE);
                right_Toggle.Draw();
                DrawTextEx(font, "Move up ", {20, 385}, 20, 0, WHITE);
                up_Toggle.Draw();
                DrawTextEx(font, "Move down ", {20, 425}, 20, 0, WHITE);
                down_Toggle.Draw();
                DrawTextEx(font, "Left rotation ", {20, 465}, 20, 0, WHITE);
                left_rotate_Toggle.Draw();
                DrawTextEx(font, "Right rotation ", {20, 505}, 20, 0, WHITE);
                right_rotate_Toggle.Draw();

                DrawTextEx(font,"Speed (seconds)", {20,545}, 18,0, WHITE);
                DrawRectangleRec(inputBox, isInputActive ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(inputBox, 2, DARKGRAY);
                DrawTextEx(font, inputText.c_str(), {inputBox.x + 7, inputBox.y + 7}, 18, 0, BLACK);

                DrawRectangleRec(startBtn, BLUE);
                DrawRectangleLinesEx(startBtn, 2, BLACK);
                DrawTextEx(font,"Start", {startBtn.x + 60, startBtn.y + 10}, 20,0, WHITE);

                EndDrawing();
            }
            else
            {
                if(!game.initialized)
                {
                    // for(auto activePositions:custom_positions)
                    // {
                    //     for (const auto &pos : activePositions) {
                    //         std::cout << "(" << pos.row << ", " << pos.col << ")";
                    //     }
                    //     std::cout<<std::endl;
                    // }

                    game.initialized = true;
                    game.InitializeCustom(custom_positions);
                    custom_positions.clear();
                }
                else
                {
                    render(game,font);
                }
            }
            
        }
        else 
        {
            BeginDrawing();
            ClearBackground(darkBlue);

            DrawTextEx(font,"SETRIS",{90, 120}, 56,0, WHITE);

            DrawTextEx(font,"Choose Mode",{90, 240}, 30,0, WHITE);

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