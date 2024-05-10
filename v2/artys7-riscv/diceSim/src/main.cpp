#include <iostream>

#include "hal/inc/uart.h"

int main() {
    uart_init(115200);
    printf("Hello from printf\r\n");
    return 0;
}
