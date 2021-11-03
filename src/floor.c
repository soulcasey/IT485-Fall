#include <stdbool.h>
#include <SDL.h>
#include "simple_logger.h"
#include "floor.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

void agumon_think(Entity* self);
void agumon_update(Entity* self);

Entity *floor_real_new(Vector3D position)
{
    Entity *ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("No world");
        return NULL;
    }

    ent->model = gf3d_model_load("floor_real");
    ent->scale = vector3d(3, 11, 0.5);
    vector3d_copy(ent->position, position);
    return ent;
}

Entity* floor_fake_new(Vector3D position)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("No world");
        return NULL;
    }

    ent->model = gf3d_model_load("floor_fake");
    ent->scale = vector3d(3, 11, 0.5);
    vector3d_copy(ent->position, position);
    return ent;
}

void floor_think(Entity* self)
{
    if (!self)return;
}

void floor_update(Entity* self)
{
    if (!self)return;
}

/*eol@eof*/
