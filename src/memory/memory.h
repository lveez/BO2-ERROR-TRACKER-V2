/*
Contains functions for reading process memory
*/

#ifndef MEMORY_H
#define MEMORY_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>

// returns the process id of Black Ops II if successful, otherwise returns 0
// doesn't work with pluto due to anti-cheat
DWORD GetProcessID();

/*
simple wrapper for OpenProcess() which adds some error handling
returns handle to Black Ops II if successful, otherwise returns NULL
*/
HANDLE GetProcessHandle(DWORD process_id);

/* 
simple wrapper for ReadProcessMemory() which adds some error handling
returns the error code from the function, if 0 read was successful
*/
DWORD ReadValue(HANDLE process_handle, LPCVOID address_to_read, LPVOID value_buffer, SIZE_T bytes_to_read);

#endif