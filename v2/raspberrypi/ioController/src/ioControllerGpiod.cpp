#if !defined(__APPLE__)
#include <iostream>
#include <system_error>
#include <functional>
#include <sstream>
#include <cassert>

#include <fcntl.h>
#include <unistd.h>

#include "ioControllerGpiod.hpp"

#include <gpiod.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define BUTTON_PIN 5


IoControllerGpiod::IoControllerGpiod()
{
    this->chip = gpiod_chip_open("/dev/gpiochip0");
    if(!this->chip) perror("gpiod_chip_open");

    errno = 0;
    this->line = gpiod_chip_get_line(chip, BUTTON_PIN);
    int ret = gpiod_line_request_input(line, "");
    assert(ret == 0);
    int wiee = gpiod_line_set_flags(this->line, GPIOD_LINE_REQUEST_FLAG_BIAS_DISABLE);
    std::cout << errno << std::endl;
    assert(wiee == 0);
    gpiod_line_release(this->line);
    this->line = gpiod_chip_get_line(chip, BUTTON_PIN);

    int err = gpiod_line_request_rising_edge_events(this->line, "rising edge example");
    if(err) perror("gpiod_line_request_bulk_rising_edge_events");

    printf("waiting for rising edge event\n");

    this->timeout.tv_sec = 60;
    this->timeout.tv_nsec = 0;

}

IoControllerGpiod::~IoControllerGpiod(){
    gpiod_line_release(this->line);
    gpiod_chip_close(this->chip);
}

void IoControllerGpiod::waitForButtonPress(){
    int ret;
    gpiod_line_event event;
    ret = gpiod_line_event_wait(this->line, &this->timeout);
    printf("Wait complete! Retcode: %d\n",ret);
    ret = gpiod_line_event_read(this->line, &event);
    printf("Read complete! Retcode: %d\n",ret);
    if(ret == -1) perror("gpiod_line_event_read");

    int value = gpiod_line_get_value(this->line);
}

#endif //!defined(__APPLE)
