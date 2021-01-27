#ifndef MATRIX_DRIVER_H
#define MATRIX_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

/**Start a thread wich will copy the active buffer to the LED matrix
 * @param cols The amount of columns (driven by SPI). Cannot be zero. Must be a multiple of 8.
 * @return true if success, false if error. 
 *
 * @warning Call this function first to prevent initialization issues.
 */
bool startLedDriving(size_t rows);

/**Introduce new data for the matrix driver to display.
 * @param newBuf A buffer filled with the new data.
 *  The buffer is row based and there are rows / 8 elements per row.
 *
 * This function returns after the swap was completed, the time this takes is dependend on multiple factors.
 * @warning Call this function before stopLedDriving and after startLedDriving.*/
void ledDriverSwapBuffer(void);

/**Get a pointer to the buffer you can use to write new data to
 * @warning Call this function before stopLedDriving and after startLedDriving.*/
uint_fast8_t *ledDriverGetInactiveBuffer(void);

/**Stop driving the LED. Includes resource cleanup. Only returns after the thread has died.*/
void stopLedDriving(void);



#endif //MATRIX_DRIVER_H