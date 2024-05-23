#pragma once

#include <vector>
#include <cstdint>
#include "matrixScreen.hpp"

class MatrixDriver {
    public:
        MatrixDriver(size_t matrixCount);

        void setScreen(MatrixScreen screen);
    private:
        void turnScreenOff();
        void turnScreenOn();
        void transmitSetOfCols(const std::vector<uint8_t>& buf, size_t colIndex); 
        std::size_t matrixCount;
};
