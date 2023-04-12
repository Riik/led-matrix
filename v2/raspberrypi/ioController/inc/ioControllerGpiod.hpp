#pragma once
#if !defined(__APPLE__)

#include <thread>
#include <string>

#include <gpiod.h>

#include "ioControllerGpiod.hpp"
#include "ioController.hpp"

class IoControllerGpiod : public IoController {
    public:

        void waitForButtonPress() final;
        IoControllerGpiod();
        ~IoControllerGpiod() override;

    private:
      unsigned int offsets[1];

      int values[1];
      struct timespec timeout;

      struct gpiod_chip *chip;
      struct gpiod_line_bulk lines;
      struct gpiod_line_bulk events;
};
#endif //!defined(__APPLE)
