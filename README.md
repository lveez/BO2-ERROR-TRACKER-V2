# BO2-ERROR-TRACKER-V2

The program will work with both redacted and plutonium (make sure to disable the plutonium anticheat). 

![Screenshot 2023-05-27 171951](https://github.com/lveez/BO2-ERROR-TRACKER-V2/assets/65928629/e91b9a5d-d47b-4bd9-8eaa-810eeeff39c5)


### Errors tracked:
- exceeded maximum number of child server script variables
- exceeded maximum number of anim info
- G_Spawn: no free entities
- failed memory allocation for script usage
- MT_AllocIndex: failed allocation of bytes for script usage
- reset
- frametime

The program can also dump the entity info and anim info of the current game, this is done automatically when either a G_Spawn error or anim info error occurs.

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
- [-d e/a] dumps entity info or anim info respectively into a txt file. 
- [-s] show simple info (only max values)
- [-v] show verbose memtree information (all memtree nodes)
- [-p] disable plutonium anticheat warning
- [-t time_in_ms] change how often the values update (a lower update time will use more CPU)
