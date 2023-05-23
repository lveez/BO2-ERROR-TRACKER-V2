/*
Contains functions used for writing to console
*/

#ifndef DISPLAY_H
#define DISPLAY_H

/*
Sets console cursor to the specificed line, top line is 0
Returns the error code, 0 means successful
*/
int SetCursorToLine(int line_number);
int GetCurrentConsoleLine();
int ClearConsole(int start_line, int max_line);

#endif