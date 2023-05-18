#pragma once

#include "matrixScreen.hpp"

class SubProgram {
    public:
        virtual MatrixScreen getScreen() = 0;
        
        virtual ~SubProgram() {}
};
