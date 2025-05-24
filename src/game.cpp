#include "game.h"
#include <cstdlib> 

Game::Game()
{
    grid = Grid();
    blocks = GetAllBlocks();
    cur_block = GetRandomBlock();
    next_block = GetRandomBlock();
    gameOver = false;
    score = 0;
}

Block Game::GetRandomBlock()
{
    if(blocks.empty()) blocks = GetAllBlocks();

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
            next_block.Draw(255,290);
            break;

        case 4://O block
            next_block.Draw(285,270);
            break;

        default:
            next_block.Draw(270,270);
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
    }

    switch (key)
    {
    case KEY_LEFT:
        MoveBlockLeft();
        break;
    
    case KEY_A:
        MoveBlockLeft();
        break;
    
    case KEY_RIGHT:
        MoveBlockRight();
        break;
    
    case KEY_D:
        MoveBlockRight();
        break;

    case KEY_DOWN:
        MoveBlockDown();
        UpdateScore(0,1);
        break;
    
    case KEY_S:
        MoveBlockDown();
        UpdateScore(0,1);
        break;
    
    case KEY_UP:
        RotateBlock();
        break;

    case KEY_W:
        RotateBlock();
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

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = cur_block.GetCellPositions();
    for(Position it:tiles)
    {
        if(grid.IsCellOutside(it.row,it.col)) return true;
    }
    return false;
}

void Game::RotateBlock()
{
    if(!gameOver)
    {
        cur_block.Rotate();
        if(IsBlockOutside()) cur_block.UndoRotate();
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
    blocks = GetAllBlocks();
    cur_block = GetRandomBlock();
    next_block = GetRandomBlock();
    score = 0;
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