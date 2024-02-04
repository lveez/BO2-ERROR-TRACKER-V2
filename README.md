# BO2-ERROR-TRACKER-V2

The program will work with both redacted and plutonium (make sure to disable the plutonium anticheat). 

![Screenshot 2023-05-27 171951](https://github.com/lveez/BO2-ERROR-TRACKER-V2/assets/65928629/e91b9a5d-d47b-4bd9-8eaa-810eeeff39c5)


### Errors tracked:
- exceeded maximum number of child server script variables (occurs when child_max / child_next hits 65536).
- exceeded maximum number of anim info (occurs when anim_next / anim_max hits 4096).
- G_Spawn: no free entities (occurs when num_entities is 1022 and there are no free ents).
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
- [-d e/a] dumps entity info or anim info respectively into a txt file. 
- [-s] show simple info (only max values)
- [-v] show verbose memtree information (all memtree nodes)
- [-p] disable plutonium anticheat warning
- [-t time_in_ms] change how often the values update (a lower update time will use more CPU)
