#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>

#include "getopt/getopt.h"
#include "memory/memory.h"
#include "memory/addresses.h"
#include "display/display.h"

DWORD UpdateFrametime(HANDLE process_handle, int* frametime)
{
    return ReadValue(process_handle, vComFrametime, frametime, 4);
}

DWORD UpdateNumSnapshotEntities(HANDLE process_handle, int* num_snapshot_entities)
{
    return ReadValue(process_handle, numSnapshotEntities, num_snapshot_entities, 4);
}

DWORD UpdateEntities(HANDLE process_handle, int* entity_count)
{
    return ReadValue(process_handle, numEntites, entity_count, 4);
}

DWORD UpdateNumEntities(HANDLE process_handle, int* num_entities)
{
    return ReadValue(process_handle, vNumEntites, num_entities, 4);
}

DWORD UpdateAnimInfo(HANDLE process_handle, unsigned short* animinfo)
{
    return ReadValue(process_handle, vAnimInfoNext, animinfo, 2);
}

DWORD UpdateParent(HANDLE process_handle, unsigned long address, unsigned short* parent_next)
{
    return ReadValue(process_handle, address, parent_next, 2);
}

unsigned short UpdateChild(HANDLE process_handle, unsigned long address, unsigned short* child_next)
{
    return ReadValue(process_handle, address, child_next, 2);
}

unsigned short UpdateMemTree(HANDLE process_handle, unsigned long address, unsigned short* head)
{
    return ReadValue(process_handle, address, head, 2*0x10);
}

void PrintHelp()
{
    int length = 60;
    printf("Selecting what variables to track:\n");
    printf("If no options are selected all variables will be tracked.\n\n");
    printf("%s%*s\n", "-c", length-2, "track child variable info");
    printf("%s%*s\n", "-p", length-2, "track parent variable info");
    printf("%s%*s\n", "-a", length-2, "track animinfo");
    printf("%s%*s\n", "-m", length-2, "track memtree info");
    printf("%s%*s\n", "-g", length-2, "track gspawn info");
    printf("%s%*s\n", "-r", length-2, "track reset info");
    printf("%s%*s\n", "-f", length-2, "track frametime info");
    printf("\nMiscellaneous:\n\n");
    printf("%s%*s\n", "-s", length-2, "display simple information (only max values)");
    printf("%s%*s\n", "-v", length-2, "display verbose memtree information");
    printf("%s%*s\n", "-p", length-2, "disable plutonium anticheat warning");
    printf("%s%*s\n", "-t time_in_ms", length-13, "change how often the values refresh");
    printf("%s%*s\n", "-h", length-2, "display help information");
    printf("\nFor more info see the github.");
}

int main(int argc, char** argv)
{
    
    /* handle arguments */
    int opt = 0;
    /* options */
    int selected = 0;
    int child = 0; // -c
    int parent = 0; // -p
    int anim = 0; // -a
    int memtree = 0; // -m
    int gspawn = 0; // -g
    int reset = 0; // -r
    int frametime = 0; // -f
    int verbose = 0; // -v
    int simple = 0; // -s
    int pluto = 0; // -p
    unsigned int refresh_rate = 50; // -t time


    while ((opt = getopt(argc, argv, "cpamgrhfvspt:")) != -1)
    {
        switch (opt) {
            case 'c':
                child = 1;
                selected = 1;
                break;
            case 'p':
                parent = 1;
                selected = 1;
                break;
            case 'a':
                anim = 1;
                selected = 1;
                break;
            case 'm':
                memtree = 1;
                selected = 1;
                break;
            case 'g':
                gspawn = 1;
                selected = 1;
                break;
            case 'r':
                reset = 1;
                selected = 1;
                break;
            case 'f':
                frametime = 1;
                selected = 1;
                break;
            case 'v':
                verbose = 1;
                break;
            case 't':
                refresh_rate = atoi(optarg);
                break;
            case 's':
                simple = 1;
                break;
            case 'n':
                pluto = 1;
                break;
            case 'h':
                PrintHelp();
                return 0;
            default:
                printf("Invalid arguments passed to %s.\n", argv[0]);
                PrintHelp();
                return -1;
        }
    }

    if (!selected)
    {
        child = 1; // -c
        parent = 1; // -p
        anim = 1; // -a
        memtree = 1; // -m
        gspawn = 1; // -g
        reset = 1; // -r
        frametime = 1; // -f
    }    

    printf("\n");
    int start_line = GetCurrentConsoleLine();
    int max_line = start_line;

    int error = 0;
    int com_frametime = 0;
    int num_snapshot_ents = 0;
    int entity_count = 0;
    int num_ents = 0;
    unsigned short animinfo = 0;
    unsigned short parent_next = 0;
    unsigned short child_next = 0;
    unsigned short memtree_head[0x10] = { 0 };

    unsigned short anim_max = 0;
    unsigned short parent_max = 0;
    unsigned short child_max = 0;
    unsigned short memtree_max = 0;

    unsigned long parent_address = 0;
    unsigned long child_address = 0;
    unsigned long memtree_address = 0;

    while (1)
    {
        DWORD process_id = 0;
        HANDLE process_handle = 0;
        Sleep(2500);

        process_id = GetProcessIDFromWindow();
        if (!process_id)
        {
            printf("ERROR: Could not find process.\n");
            continue;
        }

        process_handle = GetProcessHandle(process_id);
        if (!process_handle)
        {
            printf("ERROR: Could not get process handle.\n");
            CloseHandle(process_handle);
            continue;
        }   
        char buffer[5096] = { 0 };  
        GetModuleBaseName(process_handle, NULL, buffer, 1024);
        if (strstr(buffer, "plutonium-bootstrapper-win32.exe") != 0)
        {
            if (!pluto)
            {
                printf("Make sure plutonium anticheat is disabled then press enter.\n");
                scanf("s");
            }
        }

        /* get addresses */
        error = ReadValue(process_handle, pParentVariableValue, &parent_address, 4);
        if (error)
        {
            printf("ERROR: ReadValue failed (code %d).\n", error);
            CloseHandle(process_handle);
            continue;
        } 
        parent_address = parent_address + oParentNext;

        error = ReadValue(process_handle, pChildVariableValue, &child_address, 4);
        if (error)
        {
            printf("ERROR: ReadValue failed (code %d).\n", error);
            CloseHandle(process_handle);
            continue;
        } 
        child_address = child_address + oChildNext;

        error = ReadValue(process_handle, pGScrMemTreeGlobHead, &memtree_address, 4);
        if (error)
        {
            printf("ERROR: ReadValue failed (code %d).\n", error);
            CloseHandle(process_handle);
            continue;
        } 

        start_line = GetCurrentConsoleLine();

        while (1)
        {
            /* Update values */
            if (frametime)
            {
                error = UpdateFrametime(process_handle, &com_frametime);
                if (error)
                {
                    printf("ERROR: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                }
            }

            if (reset)
            {
                error = UpdateNumSnapshotEntities(process_handle, &num_snapshot_ents);
                if (error)
                {
                    printf("ERROR: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                }

                error = UpdateEntities(process_handle, &entity_count);
                if (error)
                {
                    printf("ERROR: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                }
            }

            if (gspawn)
            {
                error = UpdateNumEntities(process_handle, &num_ents);
                if (error)
                {
                    printf("ERROR: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                }
            }

            if (anim)
            {
                error = UpdateAnimInfo(process_handle, &animinfo);
                if (error)
                {
                    printf("ERROR: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                }
            }

            if (parent)
            {
                error = UpdateParent(process_handle, parent_address, &parent_next);
                if (error)
                {
                    printf("ERROR: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                }
            }

            if (child)
            {
                error = UpdateChild(process_handle, child_address, &child_next);
                if (error)
                {
                    printf("ERROR: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                }
            }

            if (memtree)
            {
                error = UpdateMemTree(process_handle, memtree_address, memtree_head);
                if (error)
                {
                    printf("ERROR: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                }
            }

            if (animinfo > anim_max)
                anim_max = animinfo;

            if (parent_next > parent_max)
                parent_max = parent_next;
            
            if (child_next > child_max)
                child_max = child_next;

            for (int i = 0; i < 6; i++)
            {
                if (memtree_head[i] > memtree_max)
                    memtree_max = memtree_head[i];
            }
            
            /* Update display */
            SetCursorToLine(start_line);
            if (frametime)
                printf("com_frametime: %14d / 2147483647\n", com_frametime);
            if (reset)
            {
                printf("entity count: %15d\n", entity_count);
                printf("snapshot entities: %10d / 2147483647\n", num_snapshot_ents);
            }
            if (gspawn)
                printf("num entities: %6d / 1022\n", num_ents);
            if (anim)
            {
                if (!simple)
                    printf("anim next: %18d / 4096\n", animinfo);
                printf("anim max: %19d / 4096\n", anim_max);
            }
            if (parent)
            {
                if (!simple)
                    printf("parent next:  %15d / 16383\n", parent_next);
                printf("parent max:  %16d / 16383\n", parent_max);
            }
            if (child)
            {
                if (!simple)
                    printf("child next:  %16d / 65535\n", child_next);
                printf("child max:  %17d / 65535\n", child_max);
            }
            if (memtree)
            {
                printf("memtree max:  %15d / 65535\n", memtree_max);
                if (verbose)
                {
                    for (int i = 0; i < 16; i++)
                    {
                        if (i < 10)
                            printf("memtree[%d]:  %16d\n", i, memtree_head[i]);
                        else
                            printf("memtree[%d]:  %15d\n", i, memtree_head[i]);
                    }
                }
            }

            max_line = GetCurrentConsoleLine();

            Sleep(refresh_rate);
        }
    }
}