#pragma once
#include <poll.h>
#include <cstdio>

class StdinPoller {
    public:

        StdinPoller() {
            this->fds.fd = 0;
            this->fds.events = POLLIN;
        }

        bool eventOccured() {
            if (!this->dataInStdin()) {
                return false;
            }
            do {
                getc(stdin);
            } while(this->dataInStdin());
            return true;
        }
    private:
        bool dataInStdin() {
            int ret = poll(&this->fds, 1, 0);
            return ret == 1;
        }
        struct pollfd fds;
};
