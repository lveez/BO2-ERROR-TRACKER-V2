#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>
#include <time.h>

#include "getopt/getopt.h"
#include "memory/memory.h"
#include "memory/addresses.h"
#include "display/display.h"

#include "datatypes/entities.h"
#include "datatypes/level.h"
#include "datatypes/variable.h"
#include "datatypes/anim.h"

/* options struct */
typedef struct {
    uchar dump_ents;
    uchar dump_anim;

    uchar child;
    uchar parent;
    uchar anim;
    uchar memtree;
    uchar gspawn;
    uchar reset;
    uchar frametime;

    uchar verbose;
    uchar simple;
    ushort sleep_time;
} launchOptions;

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
    printf("%s%*s\n", "-d e/a", length - 6, "dump entity or anim info respectively to file");
    printf("%s%*s\n", "-s", length-2, "display simple information (only max values)");
    printf("%s%*s\n", "-v", length-2, "display verbose memtree information");
    printf("%s%*s\n", "-p", length-2, "disable plutonium anticheat warning");
    printf("%s%*s\n", "-t time_in_ms", length-13, "change how often the values refresh");
    printf("%s%*s\n", "-h", length-2, "display help information");
    printf("\nFor more info see github.com/lveez/BO2-ERROR-TRACKER-V2.");
}

int DumpAnim(HANDLE process_handle)
{
     /* open file for dump */
    FILE* file;
    char filename[128];

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    if (sprintf(filename, "%d %02d %02d %02d-%02d-%02d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec) < 0)
        return -1;

    file = fopen(filename, "w");
    if (!file)
        return -1;

    fprintf(file, "ANIM INFO DUMP\n");

    int error;
    /* get anim info */
    XAnimInfo ganiminfo[4096];
    error = ReadValue(process_handle, 0x2b3f140, ganiminfo, sizeof(XAnimInfo)*4096);
    if (error)
    {
        printf("ERROR: ReadValue failed (code %d).\n", error);
        CloseHandle(process_handle);
        return -1;
    } 
    
    XAnimParts streamedParts;
    memset(&streamedParts, 0, sizeof(XAnimParts));

    char buffer[256];
    int num_found = 0;
    for (int i = 0; i < 4096; i++)
    {
        memset(buffer, 0, 256);
        fprintf(file, "\n---------------\n");
        fprintf(file, "anim_index: %d\n", i);

        /* check if there is a valid pointer to XAnimParts */
        if (ganiminfo[i].anim.parts == 0)
        {
            fprintf(file, "status: no parts found\n");
            continue;
        }

        /* get the data from the XAnimParts* */
        ReadValue(process_handle, ganiminfo[i].anim.parts, &streamedParts, sizeof(XAnimParts));
        if (streamedParts.frequency < 0.01 )
        {
            fprintf(file, "status: likely unused\n");
        }
        else 
        {
            fprintf(file, "status: likely used\n");
            num_found++;
        }

        /* get anin name */
        ReadValue(process_handle, streamedParts.name, buffer, 256);
        fprintf(file, "anim_name: %s\n", buffer);

        /* get notify name*/
        if (ganiminfo[i].notifyName)
        {
            ReadValue(process_handle, 0x2bf3280+(0x18*ganiminfo[i].notifyName)+4, &buffer, 256);
            fprintf(file, "notify_name: %s\n", buffer);
        }
        else 
        {
            fprintf(file, "notify_name: \n");
        }

        fprintf(file, "is_looping: %d\n", streamedParts.bLoop);

        if (ganiminfo[i].notifyType)
        {
            ReadValue(process_handle, 0x2bf3280+(0x18*ganiminfo[i].notifyType)+4, &buffer, 256);
            fprintf(file, "notify_type: %s\n", buffer);
        }
        else
        {
            fprintf(file, "notify_type: \n");
        }

        fprintf(file, "notify_count: %d\n", streamedParts.notifyCount);
        fprintf(file, "num_frames: %d\n", streamedParts.numFrames);
        fprintf(file, "frame_rate: %.2f\n", streamedParts.framerate);
        fprintf(file, "frequency: %.4f\n", streamedParts.frequency);
        fprintf(file, "anim_time: %.4f\n", ganiminfo[i].state.currentAnimTime);
        fprintf(file, "goal_time: %.4f\n", ganiminfo[i].state.goalTime);
    }
    fprintf(file, "\n%d anims in use\n", num_found);
    return 0;
}

int DumpEntities(HANDLE process_handle)
{
    /* open file for dump */
    FILE* file;
    char filename[128];

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    if (sprintf(filename, "%d %02d %02d %02d-%02d-%02d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec) < 0)
        return -1;

    file = fopen(filename, "w");
    if (!file)
        return -1;

    fprintf(file, "ENTITY INFO DUMP\n");

    int error;
    /* get level struct */
    level_locals_t level;
    error = ReadValue(process_handle, 0x2338780, &level, sizeof(level));
    if (error)
    {
        printf("ERROR: ReadValue failed (code %d).\n", error);
        CloseHandle(process_handle);
        return -1;
    } 

    /* get entities */
    gentity_t gentities[1024];
    error = ReadValue(process_handle, (DWORD)level.gentitites, &gentities, sizeof(gentity_t)*1024);
    if (error)
    {
        printf("ERROR: ReadValue failed (code %d).\n", error);
        CloseHandle(process_handle);
        return -1;
    } 

    /* write entity info to dump file */
    char buffer[256];
    int in_use_count = 0;
    for (int i = 0; i < 0x400; i++)
    {
        memset(buffer, 0, 256);
        fprintf(file, "\n---------------\n");
        fprintf(file, "ent_num: %d\n", i);
        fprintf(file, "in_use: %d\n", gentities[i].r.inuse);

        if (gentities[i].r.inuse)
            in_use_count++;

        if (gentities[i].birthTime)
            fprintf(file, "time_active: %d\n", level.time-gentities[i].birthTime);
        else
            fprintf(file, "time_active: 0\n");

        /* get type */
        if (gentities[i].s.eType)
        {
            ReadValue(process_handle, 0x2bf3280+(0x18*gentities[i].s.eType)+4, buffer, 256);
            fprintf(file, "type: %s\n", buffer);
        }
        else
        {
            fprintf(file, "type: null\n");
        }

        /* get class */
        if (gentities[i].classname)
        {
            ReadValue(process_handle, 0x2bf3280+(0x18*gentities[i].classname)+4, buffer, 256);
            fprintf(file, "class: %s\n", buffer);
        }
        else
        {
            fprintf(file, "class: null\n");
        }

        /* get optional values */
        if (gentities[i].targetname)
        {
            ReadValue(process_handle, 0x2bf3280+(0x18*gentities[i].targetname)+4, buffer, 256);
            fprintf(file, "targetname: %s\n", buffer);
        }

        if (gentities[i].target)
        {
            ReadValue(process_handle, 0x2bf3280+(0x18*gentities[i].target)+4, buffer, 256);
            fprintf(file, "target: %s\n", buffer);
        }
        
        if (gentities[i].script_noteworthy)
        {
            ReadValue(process_handle, 0x2bf3280+(0x18*gentities[i].script_noteworthy)+4, buffer, 256);
            fprintf(file, "script_noteworthy: %s\n", buffer);
        }
    }
    fprintf(file, "\n\nTotal number of entities in use: %d\n", in_use_count);
    return 0;
}

int main(int argc, char** argv)
{
    /* options */
    launchOptions options;
    memset(&options, 0, sizeof(launchOptions));
    uchar selected = 0;
    options.sleep_time = 50; // default 50ms

    /* handle arguments */
    {
    int opt;
    while ((opt = getopt(argc, argv, "cpamgrfvsht:d:")) != -1)
    {
        switch (opt) {
            // just use to dump current information
            case 'd':
                switch (optarg[0]) {
                    case 'e':
                        /* dump entities */
                        options.dump_ents = 1;
                        break;
                    case 'a':
                        /* dump anims */
                        options.dump_anim = 1;
                        break;
                    default:
                        /* invalid argument */
                        printf("Invalid arguments passed.\nUse -d e or -d a to dump entities or anims respectively.\n");
                        return -1;
                }
                break;
            
            // select what will be shown
            case 'c':
                /* child */
                options.child = 1;
                selected = 1;
                break;
            
            case 'p':
                /* parent */
                options.parent = 1;
                selected = 1;
                break;

            case 'a':
                /* anim */
                options.anim = 1;
                selected = 1;
                break;
            
            case 'm':
                /* memtree */
                options.memtree = 1;
                selected = 1;
                break;
            
            case 'g':
                /* gspawn */
                options.gspawn = 1;
                selected = 1;
                break;

            case 'r':
                /* reset */
                options.reset = 1;
                selected = 1;
                break;

            case 'f':
                /* frametime */
                options.frametime = 1;
                selected = 1;
                break;

            // select how it will be shown
            case 'v':
                /* verbose memtree */
                options.verbose = 1;
                break;
            
            case 's':
                /* simple */
                options.simple = 1;
                break;

            case 't':
                /* sleep time */
                options.sleep_time = strtol(optarg);
                if (options.sleep_time == 0)
                {
                    options.sleep_time = 50;
                    printf("Invalid time passed with -t. Reset to default of 50ms.\n");
                }
                break;

            default: 
                /* invalid inputs */
                printf("Invalid arguments passed.\n");;
            case 'h':
                /* help */  
                PrintHelp(); 
                return -1;
        }
    }
    }
    
    if (!selected)
    {
        options.child = 1;
        options.parent = 1;
        options.anim = 1;
        options.memtree = 1;
        options.gspawn = 1;
        options.reset = 1;
        options.frametime = 1;
    }
    
    /* get start line */
    printf("\n");
    int start_line = GetCurrentConsoleLine();

    DWORD process_id = 0;
    HANDLE process_handle = 0;
    int error;

    /* values to be read from game */
    level_locals_t gLevel;
    memset(&gLevel, 0, sizeof(level_locals_t));

    scrVarGlob_t gScrVarGlob;
    memset(&gScrVarGlob, 0, sizeof(scrVarGlob_t));

    ChildVariableValue child_first;
    ushort child_next = 0;
    ushort child_max = 0;

    ObjectVariableValue parent_first;
    ushort parent_next = 0;
    ushort parent_max = 0;

    XAnimInfo gAnimInfo[4096];
    XAnimParts parts;
    memset(&parts, 0, sizeof(XAnimParts));
    memset(gAnimInfo, 0, 4096*sizeof(XAnimInfo));
    ushort anim_in_use;
    ushort anim_next;
    ushort anim_max;

    uint memtree_head = 0;
    ushort memtree_next[16];
    memset(memtree_next, 0, 16*sizeof(ushort));
    ushort memtree_max = 0;

    gentity_t gEntities[1024];
    memset(gEntities, 0, sizeof(gentity_t)*1024);
    ushort num_entities = 0;
    ushort num_ents_in_use = 0;

    uint snapshot_ents = 0;
    uint num_snapshot_ents = 0;

    uint frametime = 0;

    while (1)
    {
        error = 0;
        /* Look for process repeatedly until it is found. */
        Sleep(2500);

        /* This gets the process id from the window name. */
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

        /* if it pluto display warning about anticheat. */
        {
        char buffer[512];
        GetModuleBaseName(process_handle, NULL, buffer, 512);
        if (strstr(buffer, "plutonium-bootstrapper-win32.exe") != 0)
        {
            printf("Make sure plutonium anticheat is disabled then press enter.\n");
            scanf("s");
        }
        }

        /* process found can now start reading values. */
        /* wait for map to be loaded up */
        printf("Waiting for map to be loaded...\n");
        while (gLevel.clients == 0)
        {
            error = ReadValue(process_handle, a_level, &gLevel, sizeof(level_locals_t));
            if (error)
            {
                printf("ERROR: ReadValue failed (code %d).\n", error);
                CloseHandle(process_handle);
                break;
            } 
            Sleep(500);
        }

        if (error)
            continue;

        /* dump ents and anims */
        if (options.dump_anim)
        {
            printf("Dumping anims... ");
            if (DumpAnim(process_handle) == -1)
                printf("Error.\n");
            else
                printf("Done.\n");
            return 0;
        }

        if (options.dump_ents)
        {
            printf("Dumping ents... ");
            if (DumpEntities(process_handle) == -1)
                printf("Error.\n");
            else
                printf("Done.\n");
            return 0;
        }

        /* read addresses */
        error = ReadValue(process_handle, a_level, &gLevel, sizeof(level_locals_t));
        if (error)
        {
            printf("ERROR: ReadValue failed (code %d).\n", error);
            CloseHandle(process_handle);
            break;
        } 

        error = ReadValue(process_handle, a_gScrVarGlob, &gScrVarGlob, sizeof(scrVarGlob_t));
        if (error)
        {
            printf("ERROR: ReadValue failed (code %d).\n", error);
            CloseHandle(process_handle);
            break;
        } 

        error = ReadValue(process_handle, p_gScrMemTreeGlobHead, &memtree_head, sizeof(uint));
        if (error)
        {
            printf("ERROR: ReadValue failed (code %d).\n", error);
            CloseHandle(process_handle);
            break;
        } 

        start_line = GetCurrentConsoleLine();

        while (1)
        {
            /* update gspawn */
            if (options.gspawn)
            {
                num_ents_in_use = 0;
                error = ReadValue(process_handle, gLevel.gentitites, &gEntities, sizeof(gentity_t)*1024);
                if (error)
                {
                    printf("ERROR4: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                } 

                for (int i = 0; i < 1024; i++)
                {
                    if (gEntities[i].r.inuse)
                        num_ents_in_use++;
                }

                error = ReadValue(process_handle, a_level+0xc, &gLevel.num_entities, sizeof(int));
                if (error)
                {
                    printf("ERROR5: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                } 

                if (gLevel.num_entities == 1022)
                {
                    DumpEntities(process_handle);
                    return 0;
                }
            }

            if (gLevel.num_entities == 0)
            {
                /* not in a game */
                /* reset values */
                child_max = 0;
                parent_max = 0;
                anim_max = 0;
                memtree_max = 0;
            }

            /* update child */
            if (options.child && gLevel.num_entities)
            {
                error = ReadValue(process_handle, gScrVarGlob.childVariableValue, &child_first, sizeof(ChildVariableValue));
                if (error)
                {
                    printf("ERROR: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                } 

                child_next = child_first.next;
                if (child_next > child_max)
                    child_max = child_next;
            }
            else   
                child_next = 0;

            /* update parent */
            if (options.parent && gLevel.num_entities)
            {
                error = ReadValue(process_handle, gScrVarGlob.objectVariableValue, &parent_first, sizeof(ObjectVariableValue));
                if (error)
                {
                    printf("ERROR: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                } 

                parent_next = parent_first.u.f.next;
                if (parent_next > parent_max)
                    parent_max = parent_next;
            }
            else
                parent_next = 0;

            /* update anim */
            if (options.anim && gLevel.num_entities)
            {
                anim_in_use = 0;
                error = ReadValue(process_handle, a_gAnimInfo, gAnimInfo, sizeof(XAnimInfo)*4096);
                if (error)
                {
                    printf("ERROR1: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                } 

                for (int i = 0; i < 4096; i++)
                {
                    if (gAnimInfo[i].anim.parts == 0)
                        continue;
                    
                    error = ReadValue(process_handle, gAnimInfo[i].anim.parts, &parts, sizeof(XAnimParts));
                    if (error)
                    {
                        printf("ERROR2: ReadValue failed (code %d) (%08x) [%d].\n", error, gAnimInfo[i].anim.parts, i);
                        CloseHandle(process_handle);
                        break;
                    } 
                    if (parts.frequency > 0.001)
                        anim_in_use++;
                }

                if (error)
                    break;

                anim_next = gAnimInfo[0].next;
                if (anim_next > anim_max)
                    anim_max = anim_next;

                if (anim_next == 0)
                {
                    DumpAnim(process_handle);
                    return 0;
                }
            }
            else
                anim_next = 0, anim_in_use = 0;

            /* update memtree */
            if (options.memtree && gLevel.num_entities)
            {
                error = ReadValue(process_handle, memtree_head, &memtree_next, sizeof(ushort)*16);
                if (error)
                {
                    printf("ERROR3: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                } 

                for (int i = 0; i < 6; i++)
                {
                    if (memtree_next[i] > memtree_max)
                        memtree_max = memtree_next[i];
                }
            }
            else
                memset(memtree_next, 0, sizeof(ushort)*16);

            /* update reset */
            if (options.reset)
            {
                error = ReadValue(process_handle, a_numEntites, &snapshot_ents, sizeof(int));
                if (error)
                {
                    printf("ERROR6: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                }

                error = ReadValue(process_handle, a_numSnapshotEntities, &num_snapshot_ents, sizeof(int));
                if (error)
                {
                    printf("ERROR7: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                }
            }

            /* update frametime */
            if (options.frametime)
            {
                error = ReadValue(process_handle, a_comFrametime, &frametime, sizeof(int));
                if (error)
                {
                    printf("ERROR8: ReadValue failed (code %d).\n", error);
                    CloseHandle(process_handle);
                    break;
                }
            }

            /* display information */
            SetCursorToLine(start_line);
            if (options.child)
            {
                if (!options.simple)
                    printf("child_next: %16d\n", child_next);
                printf("child_max: %17d / 65536\n", child_max);
            }

            if (options.parent)
            {
                if (!options.simple)
                    printf("parent_next: %15d\n", parent_next);
                printf("parent_max: %16d / 16384\n", parent_max);
            }

            if (options.anim)
            {
                if (!options.simple)
                {
                    printf("anims_active: %14d\n", anim_in_use);
                    printf("anims_next: %16d\n", anim_next);
                }
                printf("anims_max: %17d / 4096\n", anim_max);
            }

            if (options.memtree)
            {
                printf("memtree_next[0]: %11d\n", memtree_next[0]);
                if (options.verbose)
                {
                    for (int i = 1; i < 16; i++)
                    {
                        if (i < 10)
                            printf("memtree_next[%d]: %11d\n", i, memtree_next[i]);
                        else
                            printf("memtree_next[%d]: %10d\n", i, memtree_next[i]);
                    }
                }
                printf("memtree_max: %15d / 65536\n", memtree_max);
            }

            if (options.gspawn)
            {
                if (!options.simple)
                    printf("ents_in_use: %15d\n", num_ents_in_use);
                printf("num_entities: %14d / 1022\n", gLevel.num_entities);
            }

            if (options.reset)
            {   
                printf("current_snap_ents: %9d\n", snapshot_ents);
                printf("num_snap_ents: %13d / 2147483647\n", num_snapshot_ents);
            }
        
            if (options.frametime)
            {
                printf("com_frametime: %13d / 2147483647\n", frametime);
            }

            Sleep(options.sleep_time);
        }

    }

}