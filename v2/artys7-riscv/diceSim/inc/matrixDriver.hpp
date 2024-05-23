#pragma once

#include <vector>
#include <cstdint>
#include "matrixScreen.hpp"

class MatrixDriver {
    public:
        MatrixDriver(size_t matrixCount);

        void setScreen(MatrixScreen screen) const;
        void setBrightness(uint_fast8_t brightness) const;
    private:
        void turnScreenOff() const;
        void turnScreenOn() const;
        void transmitSetOfCols(const std::vector<uint8_t>& buf, size_t colIndex) const;
        std::size_t matrixCount;
};
