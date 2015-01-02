/*
 * Copyright 2013 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../pal.h"
#include <stddef.h>
#include <sys/time.h>   // gettimeofday
#include <string.h>     // memset
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>


static bool start_time_initialized = false;
static struct timeval start_time;
static const int64_t nanoseconds_per_microsecond = 1000;
int64_t get_nanoseconds() {
  if (!start_time_initialized) {
    gettimeofday(&start_time, NULL);
    start_time_initialized = true;
  }
  struct timeval current_time, difference;
  gettimeofday(&current_time, NULL);
  timersub(&current_time, &start_time, &difference);
  return ((int64_t)difference.tv_sec) * nanoseconds_per_second +
      ((int64_t)difference.tv_usec) * nanoseconds_per_microsecond;
}


void debug_log(const char *message, ...) {
#ifndef NDEBUG
  va_list list;
  va_start(list, message);
  vprintf(message, list);
  va_end(list);
  putchar('\n');
  fflush(stdout);
#endif
}

bool open_browser(const char *url) {
  size_t buffer_size = strlen(url) + 100;
  char buffer[buffer_size];
  sprintf(buffer, "xdg-open %s", url);
  system(buffer);
  return true;
}
