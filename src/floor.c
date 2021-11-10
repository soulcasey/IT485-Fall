#include <stdbool.h>
#include <SDL.h>
#include "simple_logger.h"
#include "floor.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "agumon.h"

void floor_think(Entity* self);
void floor_update(Entity* self);


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

Entity *floor_fake_new(Vector3D position)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("No world");
        return NULL;
    }

    ent->model = gf3d_model_load("floor_real");
    ent->scale = vector3d(0, 0, 0);
    ent->think = floor_think;
    ent->update = floor_update;
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
    if (!agumon_turn() && agumon_initial()) //Reveals fake floors when agumon turns around
    {
        self->scale = vector3d(3, 11, 0.5);
    }
    else
    {
        self->scale = vector3d(0, 0, 0);
    }
}


/*eol@eof*/
