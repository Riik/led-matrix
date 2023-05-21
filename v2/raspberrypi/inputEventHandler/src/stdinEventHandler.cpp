#include <cstdio>

#include "stdinEventHandler.hpp"

StdinEventHandler::StdinEventHandler() {
    this->fds.fd = 0;
    this->fds.events = POLLIN;
}

void StdinEventHandler::flushWaitingEvents() {
    while(this->eventWaiting()) {
        getc(stdin);
    }
}

bool StdinEventHandler::eventWaiting() {
    int ret = poll(&this->fds, 1, 0);
    if (ret == -1) perror("poll");
    return ret == 1;
}
