#include "display.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/*
Sets console cursor to the specificed line, top line is 0
Returns the error code, 0 means successful
*/
int SetCursorToLine(int line_number)
{
    HANDLE std_out;
    COORD cursor_pos = {0, line_number};

    std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (std_out == INVALID_HANDLE_VALUE)
        return GetLastError();

    SetConsoleCursorPosition(std_out, cursor_pos);
    return GetLastError();
}