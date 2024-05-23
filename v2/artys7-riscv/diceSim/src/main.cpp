#include <iostream>

#include "hal/inc/uart.h"
#include "common/inc/matrixScreen.hpp"
#include "matrixDriver.hpp"

int main() {
    uart_init(115200);
    MatrixScreen screen(2, 1);
    MatrixDriver driver(screen.getMatrixCountHeight() * screen.getMatrixCountWidth());
    screen(7, 0) = PixelColor::on;
    screen(8, 0) = PixelColor::on;
    driver.setScreen(screen);
    printf("Hello from printf\r\n");
    return 0;
}
