#if !defined(__APPLE__)
#include <iostream>
#include <system_error>
#include <cassert>

#include "gpiodInputEventHandler.hpp"

#include <sys/time.h>

#define BUTTON_PIN 5


GpiodInputEventHandler::GpiodInputEventHandler()
{
    this->chip = gpiod_chip_open("/dev/gpiochip0");
    if(!this->chip) perror("gpiod_chip_open");

    errno = 0;
    this->line = gpiod_chip_get_line(chip, BUTTON_PIN);
    int ret = gpiod_line_request_input(line, "");
    assert(ret == 0);
    int wiee = gpiod_line_set_flags(this->line, GPIOD_LINE_REQUEST_FLAG_BIAS_DISABLE);
    assert(wiee == 0);
    gpiod_line_release(this->line);
    this->line = gpiod_chip_get_line(chip, BUTTON_PIN);

    int err = gpiod_line_request_rising_edge_events(this->line, "rising edge example");
    if(err) perror("gpiod_line_request_bulk_rising_edge_events");
}

GpiodInputEventHandler::~GpiodInputEventHandler(){
    gpiod_line_release(this->line);
    gpiod_chip_close(this->chip);
}

void GpiodInputEventHandler::flushWaitingEvents() {
    gpiod_line_event event;
    while(this->eventWaiting()) {
        gpiod_line_event_read(this->line, &event);
    }
}

bool GpiodInputEventHandler::eventWaiting() {
    static struct timespec timeout = {0};
    int ret = gpiod_line_event_wait(this->line, &timeout);
    if (ret == -1) perror("gpiod_line_event_wait");
    return ret == 1;
}

#endif //!defined(__APPLE)
