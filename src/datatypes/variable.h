#ifndef VARIABLE_H
#define VARIABLE_H

typedef char undefined;
typedef short undefined2;
typedef int undefined4;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef enum {
    VAR_UNDEFINED = 0x0,
    VAR_BEGIN_REF = 0x1,
    VAR_POINTER = 0x1,
    VAR_STRING = 0x2,
    VAR_ISTRING = 0x3,
    VAR_VECTOR = 0x4,
    VAR_END_REF = 0x5,
    VAR_FLOAT = 0x5,
    VAR_INTEGER = 0x6,
    VAR_CODEPOS = 0x7,
    VAR_PRECODEPOS = 0x8,
    VAR_FUNCTION = 0x9,
    VAR_STACK = 0xA,
    VAR_ANIMATION = 0xB,
    VAR_DEVELOPER_CODEPOS = 0xC,
    VAR_INCLUDE_CODEPOS = 0xD,
    VAR_THREAD = 0xE,
    VAR_NOTIFY_THREAD = 0xF,
    VAR_TIME_THREAD = 0x10,
    VAR_CHILD_THREAD = 0x11,
    VAR_OBJECT = 0x12,
    VAR_DEAD_ENTITY = 0x13,
    VAR_ENTITY = 0x14,
    VAR_ARRAY = 0x15,
    VAR_DEAD_THREAD = 0x16,
    VAR_COUNT = 0x17,
    VAR_THREAD_LIST = 0x18,
    VAR_ENDON_LIST = 0x19
} VAR_TYPE;

typedef struct {
    ushort prev;
    ushort next;
} unnamed_type_f;

typedef struct {
    uchar* pos;
    int waitTime;
    ushort size;
    ushort bufLen;
    ushort localId;
    uchar buf;
} VariableStackBuffer;

typedef union {
    int intValue;
    uint uintValue;
    float floatValue;
    uint stringValue;
    float* vectorValue;
    uchar* codePosValue;
    uint pointerValue;
    VariableStackBuffer* stackValue; 
    uint entityOffset;
} VariableUnion;

typedef union {
    unnamed_type_f f;
    VariableUnion u; 
} unnamed_type_u;

typedef struct {
    ushort name_hi;
    ushort parentId;
} unnamed_type_keys;

typedef union {
    unnamed_type_keys keys; 
    uint match;
} ChildBucketMatchKeys;

typedef struct {
    unnamed_type_u u;
    ushort next;
    uchar type;
    uchar name_lo;
    ChildBucketMatchKeys k;
    ushort nextSibling;
    ushort prevSibling;
} ChildVariableValue;

typedef struct {
    ushort prev;
    ushort next;
} unnamed_type_f2;

typedef union {
    ushort size;
    ushort entnum;
    ushort nextEntId;
    ushort self;
} unnamed_type_u3;

typedef struct {
    ushort refCount;
    unnamed_type_u3 u;
} unnamed_type_o;

typedef union {
    unnamed_type_f2 f;
    unnamed_type_o o;
} unnamed_type_u2;

typedef union {
    uint typeAndData;
    uint classnum;
    uint notifyName;
    uint waitTime;
    uint parentLocalId;
} unnamed_type_w;

typedef struct {
    unnamed_type_u2 u;
    unnamed_type_w w; 
} ObjectVariableValue;

typedef struct {
    ushort firstChild;
    ushort lastChild;
} ObjectVariableChildren;

typedef struct {
    ObjectVariableValue* objectVariableValue;
    undefined filler1[0x80-0x4]; //empty
    ObjectVariableChildren* objectVariableChildren;
    undefined filler2[0x100-0x84]; //empty
    ushort* childVariableBucket; // seems to be for internal indexes
    undefined filler3[0x180-0x104]; //empty
    ChildVariableValue* childVariableValue;
} scrVarGlob_t;

#endif