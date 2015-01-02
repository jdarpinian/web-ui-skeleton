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

#import <Cocoa/Cocoa.h>
#import <dirent.h>
#include <mach/mach_time.h>
#include "pal.h"
#include "server.h"


int main(int argc, const char *argv[])
{
  // SIGPIPE will terminate the process if we write to a closed socket, unless
  // we disable it like so. Note that GDB/LLDB will stop on SIGPIPE anyway
  // unless you configure them not to.
  // http://stackoverflow.com/questions/10431579/permanently-configuring-lldb-in-xcode-4-3-2-not-to-stop-on-signals
  // TL;DR set a breakpoint on this line, right-click and choose
  // "Edit Breakpoint", change action to "Debugger Command", paste in:
  // "process handle SIGPIPE -n true -p true -s false" (without quotes), and
  // check the "automatically continue" box.
  //
  // While you're at it, also make sure you've configured the debugger to set
  // the working directory of the process to the top-level directory of the
  // project so the server can find the files in the html/ directory.
  signal(SIGPIPE, SIG_IGN);

  command_line_flags flags;
  parse_flags(argc, argv, &flags);
  run_server(&flags);
  return 0;
}
