#include <raylib.h>
#include <vector>

class Grid {
    private:
        int rows;
        int cols;
        int cellSize;
        std::vector<Color> colors;
        bool IsRowFull(int row);
        void ClearRow(int row);
        void MoveRowDown(int row,int num_rows);
        
    public:
        Grid();
        std::vector<std::vector<int>> grid; //2d vector each element is a number representing a color from "colors" array
        void Initialize();
        void Print();
        void Draw();
        bool IsCellOutside(int row,int col);
        bool IsCellEmpty(int row,int col);
        int ClearFullRows();
};