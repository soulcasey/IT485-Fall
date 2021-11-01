#include <stdbool.h>
#include <SDL.h>
#include "simple_logger.h"
#include "agumon.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

void agumon_think(Entity *self);
void agumon_update(Entity *self);
bool agumon_turn();

int countdown = 0;

int status = 1; //1 = turning back, 2 = back, 3 = turning front, 4 = front
double front_turn_speed = 300;
double front_stay_timer = 2;

double back_turn_speed = 600;
double back_stay_timer = 4;

unsigned int timer;

bool turn = false;

Entity *agumon_new(Vector3D position)
{
    Entity *ent = NULL;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no agumon for you!");
        return NULL;
    }
    
    ent->model = gf3d_model_load("dino");
    ent->think = agumon_think;
    ent->update = agumon_update;
    vector3d_copy(ent->position,position);
    return ent;
}

bool agumon_turn()
{
    return turn;
}

void agumon_think(Entity *self)
{
    if (!self)return;
}

void agumon_update(Entity *self)
{
    if (!self)return;

    if (status == 1)
    {
        self->rotation.z -= 3.1 / back_turn_speed; //Turn from front to back
        if (countdown >= back_turn_speed)
        {
            status = 2;
            timer = SDL_GetTicks()/1000;
            turn = false;
            slog("RED LIGHT");
        }
    }

    if (status == 2)
    {
        if ((SDL_GetTicks()/1000 - timer) >= back_stay_timer)
        {
            status = 3;
            countdown = 0;
        }
    }

    if (status == 3)
    {
        self->rotation.z += 3.1 / front_turn_speed; //Turn from back to front
        if (countdown >= front_turn_speed)
        {
            status = 4;
            timer = SDL_GetTicks() / 1000;
            turn = true;
            slog("GREEN LIGHT");
        }
    }

    if (status == 4)
    {
        if ((SDL_GetTicks() / 1000 - timer) >= front_stay_timer)
        {
            status = 1;
            countdown = 0;
            back_turn_speed *= 0.85;
        }
    }
    countdown += 1;
}

/*eol@eof*/
