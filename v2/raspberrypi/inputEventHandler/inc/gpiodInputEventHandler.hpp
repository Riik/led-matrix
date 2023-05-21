#pragma once
#if !defined(__APPLE__)

#include <gpiod.h>
#include "inputEventHandler.hpp"

class GpiodInputEventHandler : public InputEventHandler {
    public:

        bool eventWaiting() final;
        void flushWaitingEvents() final;
        GpiodInputEventHandler();
        ~GpiodInputEventHandler() override;

    private:

      struct gpiod_chip *chip;
      struct gpiod_line *line;
};
#endif //!defined(__APPLE)
