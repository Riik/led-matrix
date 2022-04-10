#include "matrixScreen.hpp"


class GameOfLife
{
public:
    GameOfLife(MatrixScreen &screen);

    const MatrixScreen &nextframe();
private:

    MatrixScreen curScreen;
    MatrixScreen nextScreen;

    std::size_t countNeighbors(std::size_t x, std::size_t y);
};
