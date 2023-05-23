#include "memory.h"

// returns the process id of Black Ops II if successful, otherwise returns 0
// doesn't work with pluto due to anti-cheat
DWORD GetProcessID(const char* name) {
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
          if (!strcmp(name, entry.szExeFile))
          {
              CloseHandle(snapshot);
              return entry.th32ProcessID;
          }
      } while (Process32Next(snapshot, &entry));
  }
  CloseHandle(snapshot);
  return 0;
}

BOOL CALLBACK enumWindowsProc(
  __in  HWND hWnd,
  __in  LPARAM lParam
) {

  char buffer[1024];
  char* check1, check2, check3;
  
  GetWindowText( hWnd, buffer, 1024 );
  check1 = strstr(buffer, "Call of Duty");
  check2 = strstr(buffer, "Black Ops II");
  check3 = strstr(buffer, "Zombies");

  if (check1 && check2 && check3)
  {
    DWORD process_id;
    GetWindowThreadProcessId(hWnd, &process_id);
    *(DWORD*)lParam = process_id;
    return FALSE;
  }
  return TRUE;
}

DWORD GetProcessIDFromWindow()
{
    DWORD processID = 0;
    EnumWindows(enumWindowsProc, &processID);
    return processID;
}



/*
simple wrapper for OpenProcess() which adds some error handling
returns handle to Black Ops II if successful, otherwise returns NULL
*/
HANDLE GetProcessHandle(DWORD process_id)
{
    HANDLE temp_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);
    
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
    if(!ReadProcessMemory(process_handle, address_to_read, value_buffer, bytes_to_read, NULL))
        return GetLastError();
    return 0;
}