#pragma once
#include <poll.h>
#include <unistd.h>

class StdinPoller {
    public:

        StdinPoller() {
            this->fds.fd = 0;
            this->fds.events = POLLIN;
        }

        bool eventOccured() {
            int ret = poll(&this->fds, 1, 0);
            if (ret == 1) {
                lseek(0, 0, SEEK_END);
                return true;
            }else {
                return false;
            }
        }
    private:
        struct pollfd fds;
};
