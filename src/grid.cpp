#include "grid.h"
#include "colors.h"
#include <iostream>
Grid::Grid()
{
    rows = 20;
    cols = 10;
    cellSize = 30;
    
    grid.resize(rows);
    for (auto &row : grid) 
    {
        row.resize(cols);
    }

    Initialize();

    colors = GetCellColors();
}

void Grid::Initialize()
{
    for(auto &row : grid)
    {
        for(auto &element : row)
        {
            element = 0;
        }
    }
}

void Grid::Print()
{
    for(auto row : grid)
    {
        for(auto element : row)
        {
            std::cout<<element<<" ";
        }
        std::cout<<std::endl;
    }
}

void Grid::Draw()
{
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            int cellValue = grid[i][j]; //get current color/value
            DrawRectangle(j*cellSize+11,i*cellSize+11,cellSize-1,cellSize-1,colors[cellValue]); //draw the cell with that color
        }
    }
}

bool Grid::IsCellOutside(int row,int col)
{
    if(row >= 0 && row < rows && col >=0 && col < cols) return false;

    return true;
}

bool Grid::IsCellEmpty(int row,int col)
{
    if(grid[row][col] == 0)
    {
        return true;
    }
    return false;
}

bool Grid::IsRowFull(int row)
{
    for(int col=0;col<cols;col++)
    {
        if(grid[row][col] == 0) return false;
    }
    return true;
}

void Grid::MoveRowDown(int row,int num_rows)
{
    for(int col=0;col<cols;col++)
    {
        grid[row+num_rows][col] = grid[row][col];
        grid[row][col] = 0;
    }
}

int Grid::ClearFullRows()
{
    int completed = 0;
    for(int row = rows-1;row >=0;row--)
    {
        if(IsRowFull(row))
        {
            ClearRow(row);
            completed++;
        }
        else if(completed > 0)
        {
            MoveRowDown(row,completed);
        }
    }
    return completed;
}