#ifndef LEVEL_H
#define LEVEL_H
#include "entities.h"

typedef char undefined;
typedef short undefined2;
typedef int undefined4;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef struct {
    undefined* clients; //gclient_t
    gentity_t* gentitites;
    int gentitySize;
    int num_entities;
    gentity_t* firstFreeEnt;
    gentity_t* lastFreeEnt;
    undefined* vehicles; //vehicle_t
    undefined filler[0x798-0x1c];
    int time;
} level_locals_t; // unfinished struct

#endif