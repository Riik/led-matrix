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



IoControllerGpiod::IoControllerGpiod()
{
  int err;

  this->chip = gpiod_chip_open("/dev/gpiochip0");
  if(!this->chip) perror("gpiod_chip_open");

  // determine which pin to use
  this->offsets[0] = 5;
  this->values[0] = -1;

  err = gpiod_chip_get_lines(chip, offsets, 1, &lines);
  if(err) perror("gpiod_chip_get_lines");

  err = gpiod_line_request_bulk_rising_edge_events(&lines, "rising edge example");
  if(err) perror("gpiod_line_request_bulk_rising_edge_events");

  // Timeout of 60 seconds, pass in NULL to wait forever
  timeout.tv_sec = 60;
  timeout.tv_nsec = 0;

  printf("waiting for rising edge event\n");
}

IoControllerGpiod::~IoControllerGpiod(){
  gpiod_line_release_bulk(&this->lines);
  gpiod_chip_close(this->chip);
}

void IoControllerGpiod::waitForButtonPress(){
  int err;
  err = gpiod_line_event_wait_bulk(&this->lines, &this->timeout, &this->events);
  if(err == -1) perror("gpiod_line_event_wait_bulk");
  else if(err == 0) fprintf(stderr, "wait timed out\n");

  err = gpiod_line_get_value_bulk(&this->events, this->values);
  if(err) perror("gpiod_line_get_value_bulk");

  for(unsigned int i=0; i<gpiod_line_bulk_num_lines(&this->events); i++)
  {
    struct gpiod_line* line;
    line = gpiod_line_bulk_get_line(&this->events, i);
    if(!line)
    {
      fprintf(stderr, "unable to get line %d\n", i);
      continue;
    }
    printf("line %s(%d)\n", gpiod_line_name(line), gpiod_line_offset(line));
  }
}

#endif //!defined(__APPLE)
