#include <stdbool.h>
#include <SDL.h>
#include "simple_logger.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "item.h"

void dalgoona_think(Entity* self);
void dalgoona_update(Entity* self);
int dalgoona_position_y();
void marble_think(Entity* self);
void marble_update(Entity* self);
int marble_position_y();

float position_y;
float position_y2;

Entity* dalgoona_new(Vector3D position)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("No world");
        return NULL;
    }

    ent->model = gf3d_model_load("dalgoona");
    ent->scale = vector3d(1, 1, 1);
    ent->think = dalgoona_think;
    ent->update = dalgoona_update;
    vector3d_copy(ent->position, position);
    return ent;
}

Entity* marble_new(Vector3D position)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("No world");
        return NULL;
    }
    ent->scale = vector3d(1, 1, 1);
    ent->think = marble_think;
    ent->update = marble_update;
    vector3d_copy(ent->position, position);
    return ent;
}

Entity* money_new(Vector3D position)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("No world");
        return NULL;
    }
    ent->model = gf3d_model_load("money");
    ent->scale = vector3d(2, 2, 2);
    vector3d_copy(ent->position, position);
    return ent;
}


void dalgoona_think(Entity* self)
{
    if (!self)return;
    position_y = self->position.y;
}

void dalgoona_update(Entity* self)
{
    if (!self)return;

}

int dalgoona_position_y()
{
    return position_y;
}


void marble_think(Entity* self)
{
    if (!self)return;
    position_y2 = self->position.y;
}

void marble_update(Entity* self)
{
    if (!self)return;

}

int marble_position_y()
{
    return position_y2;
}




/*eol@eof*/
