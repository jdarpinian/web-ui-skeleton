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

#import "../pal.h"
#import <Cocoa/Cocoa.h>
#import <mach-o/dyld.h>

static AbsoluteTime start_time = { .hi = 0, .lo = 0 };
int64_t get_nanoseconds() {
  // TODO: Apple deprecated UpTime(), so switch to mach_absolute_time.
  if (UnsignedWideToUInt64(start_time) == 0) {
    start_time = UpTime();
    return 0;
  }
  return UnsignedWideToUInt64(AbsoluteDeltaToNanoseconds(UpTime(), start_time));
}

void debug_log(const char *message, ...) {
#ifdef DEBUG
  va_list list;
  va_start(list, message);
  vprintf(message, list);
  va_end(list);
  putchar('\n');
#endif
}

bool open_browser(const char *url) {
    return [[NSWorkspace sharedWorkspace] openURL:
        [NSURL URLWithString:[NSString stringWithUTF8String:url]]];
}
