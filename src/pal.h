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

// Defines a platform abstraction layer interface for any platform-specific
// functionality needed.

#pragma once

#include <stdint.h>
#include <math.h>

#if __STDC_VERSION__ >= 199901L  // C99
#include <stdbool.h>
#endif

#ifdef _WINDOWS
// MSVC doesn't support C99, so we compile all the C code as C++ instead.
#include <limits>
#ifndef INFINITY
#define INFINITY (std::numeric_limits<double>::infinity())
#endif
// From unistd.h, but unistd.h is not available on Windows, so redeclare it
// here.
int usleep(unsigned int microseconds);
#include <intrin.h>
#define __sync_fetch_and_add _InterlockedExchangeAdd
// Ugh, MSVC doesn't have a sensible snprintf. sprintf_s is close, as long as
// you don't care about the return value.
#define snprintf sprintf_s
// getopt.h is also not available on Windows.
char optopt;
int getopt(int argc, char **argv, char *options);
#else // !_WINDOWS
#include <unistd.h>
#include <getopt.h>
#endif // _WINDOWS

// Returns the number of nanoseconds elapsed relative to some fixed point in the
// past. The point to which this duration is relative does not change during the
// lifetime of the process, but can change between different processes.
int64_t get_nanoseconds();
static const int64_t nanoseconds_per_millisecond = 1000000;
static const int64_t nanoseconds_per_second =
    nanoseconds_per_millisecond * 1000;

// Sends a message to the debug console (which printf doesn't do on Windows...).
// Accepts printf format strings. Always writes a newline at the end of the
// message.
void debug_log(const char *message, ...);

// Opens a new window/tab in the system's default browser.
// Returns true on success, false on failure.
bool open_browser(const char *url);
