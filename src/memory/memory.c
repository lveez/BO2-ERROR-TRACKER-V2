#include "memory.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>

// returns the process id of Black Ops II if successful, otherwise returns 0
// doesn't work with pluto due to anti-cheat
DWORD GetProcessID() {
  PROCESSENTRY32 entry;
  entry.dwSize = sizeof(entry);

  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (snapshot == INVALID_HANDLE_VALUE)
    {
        DWORD error = GetLastError();
        return 0;
    }
    
  if (Process32First(snapshot, &entry))
  {
      do
      {
          if (!strcmp("t6zmv41.exe", entry.szExeFile))
          {
              CloseHandle(snapshot);
              return entry.th32ProcessID;
          }
      } while (Process32Next(snapshot, &entry));
  }
  CloseHandle(snapshot);
  return 0;
}

/*
simple wrapper for OpenProcess() which adds some error handling
returns handle to Black Ops II if successful, otherwise returns NULL
*/
HANDLE GetProcessHandle(DWORD process_id)
{
    HANDLE temp_handle = OpenProcess(PROCESS_VM_READ, FALSE, process_id);
    
    if (temp_handle == NULL)
    {
        DWORD error = GetLastError();
        return 0;
    }

    return temp_handle;
}

/* 
simple wrapper for ReadProcessMemory() which adds some error handling
returns the error code from the function, if 0 read was successful
*/
DWORD ReadValue(HANDLE process_handle, LPCVOID address_to_read, LPVOID value_buffer, SIZE_T bytes_to_read)
{
    ReadProcessMemory(process_handle, address_to_read, value_buffer, bytes_to_read, NULL);
    return GetLastError();
}