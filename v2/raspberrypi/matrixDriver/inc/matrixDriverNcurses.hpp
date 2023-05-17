#pragma once

#include "matrixScreen.hpp"
#include "matrixDriver.hpp"

class MatrixDriverNcurses : public MatrixDriver {
    public:
        MatrixDriverNcurses();

        ~MatrixDriverNcurses();
        // It does not make sense to copy this object, both objects would just work on the same ncurses instance.
        MatrixDriverNcurses& operator=(const MatrixDriverNcurses&) = delete;
        MatrixDriverNcurses(const MatrixDriverNcurses&) = delete;

        void setScreen(MatrixScreen screen) final;
};
