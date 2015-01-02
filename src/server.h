#pragma once

typedef struct {
  // No flags yet.
} command_line_flags;

void parse_flags(int argc, const char **argv, command_line_flags *flags);

void run_server(command_line_flags *flags);
