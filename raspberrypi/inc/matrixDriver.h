#ifndef MATRIX_DRIVER_H
#define MATRIX_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

/**The LED driver outputs line by line.
 * There are always 8 rows, current hardware does not allow anything else.
 * The amount of columns is always a multiple of 8 with a minimum of 8.
 * One uint_fast8_t first 8 pixels. This means that there are (amount of columns) / 8 uint_fast8_t per row.
 * An important footnote is that the first uint_fast8_t is transmitted first, meaning that it will end up
 * on the last 8 columns.*/

#define LED_MATRIX_ROW_COUNT 8

/**Start a thread wich will copy the active buffer to the LED matrix
 * @param cols The amount of columns (driven by SPI). Cannot be zero. Must be a multiple of 8.
 * @return true if success, false if error. 
 *
 * @warning Call this function first to prevent initialization issues.
 */
bool startLedDriving(size_t colCount);

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

/**Set the brightness (percentage, 0 - 100)
 * @return false if new brightness is above 100, true otherwise.*/
bool ledDriverSetBrightnessPercentage(unsigned int newBrightnessPercentage);


#endif //MATRIX_DRIVER_H
