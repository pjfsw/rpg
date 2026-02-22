#include <string.h>
#include <stdio.h>
#include <ini.h>
#include "rpg/world.h"

static World world = {0};

static Room *find_or_add_room(World *w, const char *name) {
    for (int i = 0; i < w->room_count; i++) {
        if (strcmp(w->rooms[i].strid, name) == 0) {
            return &w->rooms[i];
        }
    }

    Room *r = &w->rooms[w->room_count++];
    memset(r, 0, sizeof(*r));
    strncpy(r->strid, name, sizeof(r->strid) - 1);
    r->id = w->room_count-1;
    strncpy(r->name, "unknown room", sizeof(r->name)-1);
    return r;
}

static int iniHandler(void *user, const char *section, const char *name, const char *value) {
    World *w = user;
    printf("[%s] %s = %s\n", section, name, value);

    if (strncmp(section, "room.", 5) != 0) {
        return 1;  // ignore other sections
    }

    const char *room_name = section + 5;
    Room *r = find_or_add_room(w, room_name);            

    if (strncmp(name, "nort", 4) == 0) {
        strncpy(r->north, value, sizeof(r->north) - 1);
    } else if (strncmp(name, "sout", 4) == 0) {
        strncpy(r->south, value, sizeof(r->south) - 1);
    } else if (strncmp(name, "east", 4) == 0) {
        strncpy(r->east, value, sizeof(r->east) - 1);
    } else if (strncmp(name, "west", 4) == 0) {
        strncpy(r->west, value, sizeof(r->west) - 1);    
    } else if (strcmp(name, "desc") == 0) {
        strncpy(r->desc[r->desc_count++], value, sizeof(r->desc[0]) - 1);
    } else if (strcmp(name, "name") == 0) {
        strncpy(r->name, value, sizeof(r->name)-1);
    }

    return 1; // unknown keys ignored for now    
}

void worldLoad() {
    char path[512];
    memset(&world, 0, sizeof(World));
    snprintf(path, sizeof(path), "%s/world.ini", RPG_ASSETS_DIR);

    if (ini_parse(path, iniHandler, &world) < 0) {
        printf("Failed to load world.ini\n");
    }
}

Room *worldGetRoom(int id) {
    if (id >= 0 && id < world.room_count) {
        return &world.rooms[id];
    }
}

Room *worldFindRoom(char *name) {
    for (int i = 0; i < world.room_count; i++) {
        if (strcmp(name, world.rooms[i].strid) == 0) {
            return &world.rooms[i];
        }
    }
    return NULL;
}