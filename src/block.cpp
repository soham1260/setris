#include "block.h"

Block::Block()
{
    cellSize = 30;
    r_state = 0;
    colors = GetCellColors();
    r_offset = 0;
    c_offset = 0;
}

void Block::Draw(int x_offset,int y_offset)
{
    std::vector<Position> tiles = GetCellPositions(); //contains all positions that should be colored for current r_state after applying offset

    for(auto it:tiles)
    {
        DrawRectangle(it.col*cellSize+x_offset,it.row*cellSize+y_offset,cellSize-1,cellSize-1,colors[id]);
    }
}

void Block::Move(int rows,int cols)
{
    r_offset += rows;
    c_offset += cols;
}

std::vector<Position> Block::GetCellPositions()
{
    std::vector<Position> tiles = cells[r_state]; //contains all positions that should be colored for current r_state
    std::vector<Position> movedTiles;

    for(auto it:tiles)
    {
        Position newPosition = Position(it.row + r_offset,it.col + c_offset);
        movedTiles.push_back(newPosition);
    }

    return movedTiles;
}

void Block::RotateRight()
{
    r_state++;
    r_state%=4;
}

void Block::UndoRotateRight()
{
    r_state--;
    if(r_state == -1) r_state = 3;
}

void Block::RotateLeft()
{
    r_state--;
    if(r_state == -1) r_state = 3;
}

void Block::UndoRotateLeft()
{
    r_state++;
    r_state%=4;
}