#if !defined(__APPLE__)
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

  this->line = gpiod_chip_get_line(chip, BUTTON_PIN);

  int err = gpiod_line_request_rising_edge_events(this->line, "rising edge example");
  if(err) perror("gpiod_line_request_bulk_rising_edge_events");

  printf("waiting for rising edge event\n");

  timeout.tv_sec = 60;
  timeout.tv_nsec = 0;

}

IoControllerGpiod::~IoControllerGpiod(){
  gpiod_line_release(this->line);
  gpiod_chip_close(this->chip);
}

void IoControllerGpiod::waitForButtonPress(){
  gpiod_line_event event;
  int err = gpiod_line_event_read(this->line, &event);
  if(err == -1) perror("gpiod_line_event_wait_bulk");
  else if(err == 0) fprintf(stderr, "wait timed out\n");

  int value = gpiod_line_get_value(this->line);
}

#endif //!defined(__APPLE)
