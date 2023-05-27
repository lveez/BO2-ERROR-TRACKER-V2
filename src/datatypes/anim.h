#ifndef ANIM_H
#define ANIM_H

#include "stdint.h"

typedef char undefined;
typedef short undefined2;
typedef int undefined4;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef union {
    uchar* _1;
    ushort* _2;
    void* data;
} XAnimIndices;

typedef struct {
    ushort name;
    float time;
} XAnimNotifyInfo;

// typedef struct {
//     char* name;
//     ushort dataByteCount;
//     ushort dataShortCount;
//     ushort dataIntCount;
//     ushort randomDataByteCount;
//     ushort randomDataIntCount;
//     ushort numframes;
//     uchar bLoop;
//     uchar bDelta;
//     uchar bDelta3D;
//     uchar bLeftHandGripIK;
//     uint streamedFileSize;
//     uchar boneCount[10];
//     uchar notifyCount;
//     uchar assetType;
//     uchar isDefault;
//     uint randomDataShortCount;
//     uint indexCount;
//     float framerate;
//     float frequency;
//     float primedLength;
//     float loopEntryTime;
//     ushort* names;
//     uchar* dataByte;
//     int* dataInt;
//     short* randomDataShort;
//     uchar* randomDataByte;
//     int* randomDataInt;
//     undefined* indices; //XAnimIndices
//     undefined* notify; //XAnimNotifyInfo
//     undefined* deltaPart; //XAnimDeltaPart
// } XAnimParts;

typedef struct {
    ushort flags;
    ushort children;
} XAnimParent;



typedef struct {
    float currentAnimTime;
    float oldTime;
    short cycleCount;
    short oldCycleCount;
    float goalTime;
    float goalWeight;
    float weight;
    float rate;
    uchar instantWeightChange;
} XAnimState;

typedef struct {
    char* name;
    ushort dataByteCount;
    ushort dataShortCount;
    ushort dataIntCount;
    ushort randomDataByteCount;
    ushort randomDataIntCount;
    ushort numFrames;
    uchar bLoop;
    uchar bDelta;
    uchar bDelta3D;
    uchar bLeftHandGripIK;
    uint streamedFileSize;
    uchar boneCount[10];
    uchar notifyCount;
    uchar assetType;
    uchar bDefault;
    uchar pad[11];
    float framerate;
    float frequency;
    float primedLength;
    float loopEntryTime;
    ushort* boneIds;
    uchar* dataByte;
    uchar* dataShort;
    int* dataInt;
    short* randomDataShort;
    uchar* randomDataByte;
    int* randomDataInt;
    undefined* indices; //XAnimIndices
    undefined* notify; //XAnimNotifyInfo
    undefined* deltaPart; //XAnimDeltaPart
} XAnimParts;

typedef union {
    XAnimParts* parts;
    XAnimParent animParent; 
} XAnimInfo_u_20;

typedef struct {
    ushort notfiyChild;
    short notifyIndex;
    ushort notifyName;
    ushort notifyType;
    ushort prev;
    ushort next;
    ushort children;
    ushort parent;
    ushort animIndex;
    ushort animToModel;
    XAnimInfo_u_20 anim;
    XAnimState state;
    XAnimParts* streamedParts; 
    ushort streamedAnimToModel;
} XAnimInfo;

#endif