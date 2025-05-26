#include "game.h"
#include <cstdlib> 
#include <vector>
#include <iostream>

Game::Game()
{
    grid = Grid();
    gameOver = false;
    initialized = false;
    mode = NONE;
    totalBlocks = 0;
}

void Game::InitializeStandard()
{
    score = 0;
    blocks = GetAllBlocks();
    cur_block = GetRandomBlock();
    next_block = GetRandomBlock();
    totalBlocks = blocks.size();
    allow_left = true;
    allow_right = true;
    allow_up = false;
    allow_down = true;
    allow_left_rotate = false;
    allow_right_rotate = true;
}

void Game::InitializeCustom(std::vector<std::vector<Position>> custom_positions, bool left, bool right, bool up, bool down, bool left_rotate, bool right_rotate)
{
    int i=0;
    score = 0;
    for(auto positions:custom_positions)
    {
        Block block = Block();
        block.id = i+1;
        block.cells[0] = positions;
        
        for (int j = 1; j <= 3; j++) {
            std::vector<Position> rotated;
            for (const auto& pos : block.cells[j - 1]) {
                int nr = 1 + (pos.col - 1);
                int nc = 1 - (pos.row - 1);
                rotated.push_back(Position(nr, nc));
            }
            block.cells[j] = rotated;
        }

        block.Move(0,3);
        all_blocks.push_back(block);
        i++;
    }
    std::cout<<all_blocks.size();
    for(auto it:all_blocks)
    {
        for(auto pos:it.cells[0])
        {
            std::cout << "(" << pos.row << ", " << pos.col << ")";
        }
        std::cout<<std::endl;
    }

    allow_left = left;
    allow_right = right;
    allow_up = up;
    allow_down = down;
    allow_left_rotate = left_rotate;
    allow_right_rotate = right_rotate;
    
    cur_block = GetRandomBlock();
    next_block = GetRandomBlock();
}

Block Game::GetRandomBlock()
{
    if(blocks.empty())
    {
        if(mode == START)
        {
            blocks = GetAllBlocks();
        }
        else
        {
            blocks = all_blocks;
        }
    }

    int block_index = rand() % blocks.size();
    Block block = blocks[block_index];
    blocks.erase(blocks.begin() + block_index);
    
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(),JBlock(),LBlock(),OBlock(),SBlock(),TBlock(),ZBlock()};
}

void Game::Draw()
{
    grid.Draw();
    cur_block.Draw(11,11);
    
    switch (next_block.id)
    {
        case 3://I block
            next_block.Draw(255,330);
            break;

        case 4://O block
            next_block.Draw(285,310);
            break;

        default:
            next_block.Draw(270,310);
            break;
    }
}

void Game::handleInput()
{
    int key = GetKeyPressed();

    if(gameOver && key)
    {
        gameOver = false;
        ResetGame();
        initialized = false;
        mode = NONE;
        totalBlocks = 0;
        blocks.clear();
        all_blocks.clear();
    }

    switch (key)
    {
    case KEY_LEFT:
        if(allow_left) MoveBlockLeft();
        break;
    
    case KEY_A:
        if(allow_left) MoveBlockLeft();
        break;
    
    case KEY_RIGHT:
        if(allow_right) MoveBlockRight();
        break;
    
    case KEY_D:
        if(allow_right) MoveBlockRight();
        break;

    case KEY_DOWN:
        if(allow_down)
        {
            MoveBlockDown();
            UpdateScore(0,1);
        }
        break;
    
    case KEY_S:
        if(allow_down)
        {
            MoveBlockDown();
            UpdateScore(0,1);
        }
        break;
    
    case KEY_UP:
        if(allow_up) MoveBlockUp();
        break;

    case KEY_W:
        if(allow_up) MoveBlockUp();
        break;

    case KEY_E:
        if(allow_right_rotate) RotateBlockRight();
        break;

    case KEY_Q:
        if(allow_left_rotate) RotateBlockLeft();
        break;
    
    default:
        break;
    }
}

void Game::MoveBlockLeft()
{
    if(!gameOver)
    {
        cur_block.Move(0,-1);
        if(IsBlockOutside() || !BlockFits()) cur_block.Move(0,1);
    }
}

void Game::MoveBlockRight()
{
    if(!gameOver)
    {
        cur_block.Move(0,1);
        if(IsBlockOutside() || !BlockFits()) cur_block.Move(0,-1);
    }
}

void Game::MoveBlockDown()
{
    if(!gameOver)
    {
        cur_block.Move(1,0);
        if(IsBlockOutside() || !BlockFits())//block has reached lowest possible level
        {
            cur_block.Move(-1,0);
            LockBlock();
        }
    }
}

void Game::MoveBlockUp()
{
    if(!gameOver)
    {
        cur_block.Move(-1,0);
        if(IsBlockOutside() || !BlockFits())//block has reached lowest possible level
        {
            cur_block.Move(1,0);
        }
    }
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = cur_block.GetCellPositions();
    for(Position it:tiles)
    {
        if(grid.IsCellOutside(it.row,it.col)) return true;
    }
    return false;
}

void Game::RotateBlockRight()
{
    if(!gameOver)
    {
        cur_block.RotateRight();
        if(IsBlockOutside()) cur_block.UndoRotateRight();
    }
}

void Game::RotateBlockLeft()
{
    if(!gameOver)
    {
        cur_block.RotateLeft();
        if(IsBlockOutside()) cur_block.UndoRotateLeft();
    }
}
void Game::LockBlock()
{
    std::vector<Position> tiles = cur_block.GetCellPositions(); 

    for(auto it:tiles)
    {
        grid.grid[it.row][it.col] = cur_block.id;
    }
    cur_block = next_block;

    if(!BlockFits())
    {
        gameOver = true;
    }

    next_block = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    UpdateScore(rowsCleared,0);
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = cur_block.GetCellPositions();
    for(auto it:tiles)
    {
        if (!grid.IsCellEmpty(it.row,it.col))
        {
            return false;
        }
    }
    return true;
}

void Grid::ClearRow(int row)
{
    for(int col=0;col<cols;col++)
    {
        grid[row][col] = 0;
    }
}

void Game::ResetGame()
{
    grid.Initialize();
}

void Game::UpdateScore(int lines,int down)
{
    switch(lines)
    {
        case 1:
            score+=100;
            break;
            
        case 2:
            score+=200;
            break;
        
        case 3:
            score+=500;
            break;
        
        default:
            break;

    }
    score+=down*10;
}