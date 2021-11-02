#include <stdbool.h>
#include <SDL.h>
#include "simple_logger.h"
#include "environment.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>


Entity *environment_new(Vector3D position)
{
    Entity *ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("No world");
        return NULL;
    }

    ent->model = gf3d_model_load("testworld");
    ent->scale = vector3d(3, 11, 0.5);
    vector3d_copy(ent->position, position);
    return ent;
}

/*eol@eof*/
