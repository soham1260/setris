#include "grid.h"
#include "blocks.cpp"
#include <vector>

class Game {
    private:
        std::vector<Block> blocks;
        Block cur_block;
        Block next_block;
        bool IsBlockOutside();
        void RotateBlock();
        void LockBlock();
        bool BlockFits();
        void ResetGame();
        void UpdateScore(int lines,int down);
        Block GetRandomBlock();
        std::vector<Block> GetAllBlocks();
        void MoveBlockLeft();
        void MoveBlockRight();
        Grid grid;

    public:
        Game();
        void Draw();
        void handleInput();
        void MoveBlockDown();
        bool gameOver;
        int score;
};