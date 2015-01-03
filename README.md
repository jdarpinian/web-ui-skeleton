Skeleton for a cross-platform native app with a web-based UI. Supports
Windows, Mac, and Linux, with Visual Studio, XCode, and Makefiles respectively.

## Features

Running the server will automatically open a browser tab showing the UI.
When all UI pages are closed, the server will automatically exit.

The UI can communicate with the server by making HTTP requests. The server can
use custom request handlers to perform any task requested by the UI using native
code.

In debug mode, static files in the html directory are served directly from the
filesystem to support edit-refresh debugging. In release mode the static files
are embedded into the server executable for easy distribution.

## Building

To build, first execute the generate-project-files script to run GYP (this
requires Python 2.6); then open the generated project files in the build/
directory.

For XCode, make sure to follow the instructions in src/mac/main.m to configure
the debugger properly.
