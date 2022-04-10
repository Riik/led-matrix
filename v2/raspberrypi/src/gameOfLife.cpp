#include <algorithm>

#include "gameOfLife.hpp"

GameOfLife::GameOfLife(MatrixScreen &screen) : curScreen{screen}, nextScreen{screen}
{
}

std::size_t GameOfLife::countNeighbors(std::size_t x, std::size_t y)
{
    std::size_t count = 0;

    std::size_t iMin = 0;
    if (x > 0) {
        iMin = x - 1;
    }
    std::size_t jMin = 0;
    if (y > 0) {
        jMin = y - 1;
    }

    for (size_t i = iMin; i < std::min(this->curScreen.getPixelCountWidth(), x + 2); ++i){
        for (size_t j = jMin; j < std::min(this->curScreen.getPixelCountHeight(), y + 2); ++j){
            if (this->curScreen(i, j) == MatrixScreen::PixelColor::on) {
                count++;
            }
        }
    }
    if (this->curScreen(x,y) == MatrixScreen::PixelColor::on) {
        count -= 1;
    }
    return count;
}

const MatrixScreen &GameOfLife::nextframe()
{
    for (size_t i = 0; i < this->curScreen.getPixelCountWidth(); ++i) {
        for (size_t j = 0; j < this->curScreen.getPixelCountHeight(); ++j) {
            size_t neighborCount = countNeighbors(i, j);
            if (this->curScreen(i, j) == MatrixScreen::PixelColor::on && (neighborCount < 2 || neighborCount > 3)){
                this->nextScreen(i, j) = MatrixScreen::PixelColor::off;
            }
            if (this->curScreen(i, j) == MatrixScreen::PixelColor::off && neighborCount == 3){
                this->nextScreen(i, j) = MatrixScreen::PixelColor::on;
            }
        }
    }
    this->curScreen = this->nextScreen;
    return nextScreen;
}
