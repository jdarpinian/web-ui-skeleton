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

#include "stdafx.h"

static INIT_ONCE start_time_init_once;
static LARGE_INTEGER start_time;
static LARGE_INTEGER frequency;
BOOL CALLBACK init_start_time(PINIT_ONCE ignored, void *ignored2,
    void **ignored3) {
  BOOL r;
  r = QueryPerformanceCounter(&start_time);
  assert(r);
  r = QueryPerformanceFrequency(&frequency);
  assert(r);
  return TRUE;
}


int64_t get_nanoseconds_elapsed(const LARGE_INTEGER &current_time) {
  BOOL r = InitOnceExecuteOnce(&start_time_init_once, &init_start_time, NULL,
      NULL);
  assert(r);
  int64_t elapsed = current_time.QuadPart - start_time.QuadPart;
  if (elapsed < 0) {
    elapsed = 0;
  }
  // We switch to floating point here before the division to avoid overflow
  // and/or loss of precision. We can't do this in 64-bit signed integer math:
  // doing the multiplication first would overflow in a matter of seconds, while
  // doing the division first risks loss of accuracy if the timer resolution is
  // close to the nanosecond range.
  double elapsed_ns =
      elapsed * (nanoseconds_per_second / (double)frequency.QuadPart);
  return (int64_t) elapsed_ns;
}


// Returns the number of nanoseconds elapsed since the first call to
// get_nanoseconds in this process. The first call always returns 0.
int64_t get_nanoseconds() {
  LARGE_INTEGER current_time;
  BOOL r = QueryPerformanceCounter(&current_time);
  assert(r);
  return get_nanoseconds_elapsed(current_time);
}

static const int log_buffer_size = 1000;
void debug_log(const char *message, ...) {
#ifndef NDEBUG
  char buf[log_buffer_size];
  va_list args;
  va_start(args, message);
  vsnprintf_s(buf, _TRUNCATE, message, args);
  va_end(args);
  OutputDebugStringA(buf);
  OutputDebugStringA("\n");
  printf("%s\n", buf);
#endif
}

void always_log(const char *message, ...) {
  char buf[log_buffer_size];
  va_list args;
  va_start(args, message);
  vsnprintf_s(buf, _TRUNCATE, message, args);
  va_end(args);
  OutputDebugStringA(buf);
  OutputDebugStringA("\n");
  printf("%s\n", buf);
}


int usleep(unsigned int microseconds) {
  Sleep(microseconds / 1000);
  return 0;
}

bool open_browser(const char *url) {
  // Recommended by:
  // http://msdn.microsoft.com/en-us/library/windows/desktop/bb762153(v=vs.85).aspx
  HRESULT hr =
      CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
  assert(hr == S_OK);
  return 32 < (int)ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}
