#include "grid.h"
#include "blocks.cpp"
#include "position.h"
#include <vector>

enum GameMode { NONE, START, CUSTOM };

class Game {
    private:
        std::vector<Block> blocks;
        std::vector<Block> all_blocks;
        Block cur_block;
        Block next_block;
        bool IsBlockOutside();
        void RotateBlockRight();
        void RotateBlockLeft();
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
        void MoveBlockUp();
        bool gameOver;
        int score;
        GameMode mode;
        bool initialized;
        void InitializeStandard();
        void InitializeCustom(std::vector<std::vector<Position>> custom_positions, bool left, bool right, bool up, bool down, bool left_rotate, bool right_rotate);
        bool customSetup;
        int totalBlocks;
        bool allow_left;
        bool allow_right;
        bool allow_up;
        bool allow_down;
        bool allow_left_rotate;
        bool allow_right_rotate;
};