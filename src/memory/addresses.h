#ifndef ADDRESSES_H
#define ADDRESSES_H

/* address of gAnimInfo */
/* data type: XAnimInfo[4096] */
unsigned int a_gAnimInfo = 0x02B3F140;

/* address of level */
/* data type: level_locals_t */
unsigned int a_level = 0x02338780;

/* address of mt_buffer */
/* this is a section of memory that contains */
/* strings used by the game */
/* data type: char* */
unsigned int a_mtBuffer = 0x02BF3280;

/* address of gScrVarGlob */
/* data type: scrvarglob_t*/
unsigned int a_gScrVarGlob = 0x02DEA400;

/* address of a ptr that points to the array 
of ushorts gScrMemTreeGlob.head that can be 
used for MT error tracking. */
unsigned long p_gScrMemTreeGlobHead = 0x02BF3180;

/* Other entity things*/
unsigned long a_numSnapshotEntities = 0x1278020;
unsigned long a_numEntites = 0x1278010;

/* frametime */
unsigned long a_comFrametime = 0x025B6130;


#endif