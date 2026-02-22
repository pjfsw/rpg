#pragma once

#define ROOMID_LENGTH 32
typedef struct {
    int id;
    char strid[ROOMID_LENGTH];
    char north[ROOMID_LENGTH];
    char south[ROOMID_LENGTH];
    char east[ROOMID_LENGTH];
    char west[ROOMID_LENGTH];    
    char name[80];    
    char desc[8][80];
    int desc_count;
} Room;

typedef struct {
    Room rooms[16];
    int room_count;
} World;

Room *worldGetRoom(int id);

Room *worldFindRoom(char *name);


void worldLoad();