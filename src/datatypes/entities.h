#ifndef ENTITIES_H
#define ENTITIES_H

typedef char undefined;
typedef short undefined2;
typedef int undefined4;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef struct {
    ushort number;
    ushort infoIndex;
} EntHandle;

typedef struct {
    int number;
    undefined lerp[0x7c]; //LerpEntityState
    int time2;
    uint loopSoundId;
    int solid;
    undefined renderOptions[0x4]; //renderOptions_s
    undefined un2[0x10]; //unnamed-type-un2
    undefined un3[0x4]; //unnamed-type-un3
    uint partBits[0x5];
    undefined clientLinkInfo[0x4]; //clientLinkInfo_t
    int clientMask;
    uchar events[0x4];
    uint eventParms[0x4];
    uint evenParm;
    short eType;
    short groundEntityNum;
    undefined index[0x2]; //unnamed-type-index
    short otherEntityNum;
    short attackerEntityNum;
    short enemyModel;
    undefined weapon[0x4]; //Weapon
    undefined lastStandPrevWeapon[0x4]; //Weapon
    ushort targetname;
    short loopSoundFade;
    short eventSequence;
    uchar surfType;
    uchar clientNum;
    uchar iHeadIcon;
    uchar WeaponModel;
    undefined un1; //unnamed-type-un1
} entityState_s;

typedef struct {
    uchar linked;
    uchar bmodel;
    uchar svFlags;
    uchar inuse;
    int broadcastTime;
    undefined mins[0xc]; //vec3_t
    undefined maxs[0xc]; //vec3_t
    int contents;
    undefined absmin[0xc]; //vec3_t
    undefined absmax[0xc]; //vec3_t
    undefined currentOrigin[0xc]; //vec3_t
    undefined currentAngles[0xc]; //vec3_t
    EntHandle ownerNum; //EntHandle
    int eventTime;

} entityShared_t;

typedef struct gentity {
    entityState_s s;
    entityShared_t r;
    undefined* client; //gclient_t
    undefined* actor; //actor_t
    undefined* sentient; //sentient_t
    undefined* pTurretInfo; //TurretInfo
    undefined* destructible; //Destructible
    undefined* vehicle; //vehicle_t
    ushort model;
    uchar physicsObject;
    uchar takedamage;
    uchar active;
    uchar nopickup;
    uchar handler;
    uchar team;
    uchar avoidHandle;
    ushort classname;
    ushort target;
    ushort targetname;
    ushort script_noteworthy;
    uint attachIgnoreCollision;
    int spawnflags;
    int flags;
    int eventTime;
    int freeAfterEvent;
    int unlinkAfterEvent;
    int clipmask;
    int processedFrame;
    EntHandle parent; //EntHandle
    int nextthink;
    int health;
    int maxHealth;
    int damage;
    undefined flame_timed_damage[0x70]; //flame_timed_damage_t[4]
    int last_timed_damage_radius;
    int count;
    undefined gent[0x60]; //gentity_t_u_556
    EntHandle missileTargetEnt; //EntHandle
    undefined snd_wait[0x14]; //unnamed-type-snd-wait
    undefined* tagInfo; //taginfo_t
    struct gentity* tagChildren;
    undefined attachModelNames[0x26]; //ushort[19]
    undefined attachTagNames[0x26]; //ushort[19]
    undefined* pAnimTree; //XAnimTree_s
    ushort disconnectedLinks;
    int iDisconnectTime;
    int useCount;
    int physObjId;
    struct gentity* nextFree;
    int birthTime;
    int ikPlayerclipTerrainTime;
    int ikDisableTerrainMappingTime;
} gentity_t;

#endif