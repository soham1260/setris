#include <vector>
#include <map>
#include "position.h"
#include "colors.h"

class Block {
    private:
        int cellSize;
        int r_state;
        int r_offset;
        int c_offset;
        std::vector<Color> colors;

    public:
        int id;
        Block();
        std::map<int,std::vector<Position>> cells; //all different positions a block can take
        void Draw(int x_offset,int y_offset);
        void Move(int rows,int cols);
        std::vector<Position> GetCellPositions();
        void Rotate();
        void UndoRotate();
};