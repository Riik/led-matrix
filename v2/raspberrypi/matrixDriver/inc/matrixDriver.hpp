#pragma once

#include "matrixScreen.hpp"

class MatrixDriver {
    public:
        virtual void setScreen(MatrixScreen screen) = 0;

        virtual ~MatrixDriver() {};
};
