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

float position_y;

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
    ent->scale = vector3d(0.5, 0.5, 0.5);
    ent->think = dalgoona_think;
    ent->update = dalgoona_update;
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




/*eol@eof*/
