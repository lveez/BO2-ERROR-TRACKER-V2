# BO2-ERROR-TRACKER-V2

The program will work with both redacted and plutonium (make sure to disable the plutonium anticheat). 

![Screenshot 2023-05-23 203158](https://github.com/lveez/BO2-ERROR-TRACKER-V2/assets/65928629/cffe863b-17c7-44d4-8b19-490b589965dc)

### Errors tracked:
- exceeded maximum number of child server script variables
- exceeded maximum number of anim info
- G_Spawn: no free entities
- failed memory allocation for script usage
- MT_AllocIndex: failed allocation of bytes for script usage
- reset
- frametime

----------------

### Usage
You can launch the program as normal for the default options, this will track all the possible errors. If wanted you can also launch the program with the following command line arguments:
- [-c] show child variable info
- [-p] show parent variable info
- [-a] show animinfo
- [-m] show memtree info (failed memory allocation/MT_AllocIndex error)
- [-g] show gspawn info
- [-r] show reset info
- [-f] show frametime info

You can also change some options for how the program displays the information:
- [-s] show simple info (only max values)
- [-v] show verbose memtree information (all memtree nodes)
- [-p] disable plutonium anticheat warning
- [-t time_in_ms] change how often the values update (a lower update time will use more CPU)
