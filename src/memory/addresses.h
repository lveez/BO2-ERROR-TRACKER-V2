#ifndef ADDRESSES_H
#define ADDRESSES_H

/* address of a ptr that points to the array 
of ushorts gScrMemTreeGlob.head that can be 
used for MT error tracking. */
/*
max unknown (potentially 0xffff)
*/
unsigned long pGScrMemTreeGlobHead = 0x2BF3180;

/* address of the level.num_entities value 
used to track GSpawn. */
/*
max = 0x3fe
*/
unsigned long vNumEntites = 0x0233878C;

/* address of the ptr for the child 
script error */
/*
max = 0xffff
*/
unsigned long pChildVariableValue = 0x02DEA580;
unsigned long oChildNext = 0x4;

/* address of the otr for the parent 
script error */
/*
max = 0x3fff
*/
unsigned long pParentVariableValue = 0x02DEA400;
unsigned long oParentNext = 0x2;

/* address of the value for g_AnimInfo.next */
/*
max = 0x0fff
*/
unsigned long vAnimInfoNext = 0x02B3F14A;

/* Other entity things*/
unsigned long numSnapshotEntities = 0x1278020;
unsigned long numEntites = 0x1278010;

/* frametime */
unsigned long vComFrametime = 0x025B6130;


#endif