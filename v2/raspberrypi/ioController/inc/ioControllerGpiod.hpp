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

      struct timespec timeout;
      struct gpiod_chip *chip;
      struct gpiod_line *line;
};
#endif //!defined(__APPLE)
