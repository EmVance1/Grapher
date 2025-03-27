#include "pch.h"
#include "log.h"


#define TERM_GREEN 2
#define TERM_RED 4
#define TERM_YELLOW 6
#define TERM_BLUE 1
#define TERM_CYAN 11


void list_colors() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for(WORD k = 1; k < 255; k++) {
        SetConsoleTextAttribute(hConsole, k);
        std::cout << "code: " << k << ", example text\n";
    }
}


void set_stdout_color(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void set_stderr_color(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

