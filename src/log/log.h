#pragma once
#include <Windows.h>


#define TERM_NONE 7
#define TERM_GREEN 2
#define TERM_RED 4
#define TERM_YELLOW 6
#define TERM_BLUE 1
#define TERM_CYAN 11


void set_stdout_color(WORD color);
void set_stderr_color(WORD color);


#define log_info(msg)  set_stdout_color(TERM_GREEN);  std::cout << "[vertex: info] "; set_stdout_color(TERM_NONE); std::cout << msg << "\n"
#define log_warn(msg)  set_stdout_color(TERM_YELLOW); std::cout << "[vertex: warn] "; set_stdout_color(TERM_NONE); std::cout << msg << "\n"
#define log_error(msg) set_stderr_color(TERM_RED);    std::cerr << "[vertex:  err] "; set_stderr_color(TERM_NONE); std::cerr << msg << "\n"

