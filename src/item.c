#include <stdbool.h>
#include <SDL.h>
#include "simple_logger.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "item.h"

void item_think(Entity* self);
void item_update(Entity* self);
int item_position_y();

float position_y;

Entity* item_new(Vector3D position)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("No world");
        return NULL;
    }

    ent->model = gf3d_model_load("item");
    ent->scale = vector3d(0.5, 0.5, 0.5);
    ent->think = item_think;
    ent->update = item_update;
    vector3d_copy(ent->position, position);
    return ent;
}

void item_think(Entity* self)
{
    if (!self)return;
    position_y = self->position.y;
}

void item_update(Entity* self)
{
    if (!self)return;
}

int item_position_y()
{
    return position_y;
}




/*eol@eof*/
