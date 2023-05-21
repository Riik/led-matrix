#pragma once
#include <poll.h>

#include "inputEventHandler.hpp"

class StdinEventHandler : public InputEventHandler {
    public:
        StdinEventHandler();
        bool eventWaiting() final;
        void flushWaitingEvents() final;
    private:
        struct pollfd fds;
};
