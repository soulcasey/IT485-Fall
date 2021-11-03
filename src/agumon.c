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

int status; //1 = turning back, 2 = back, 3 = turning front, 4 = front

double front_turn_timer = 0.5;
double front_stay_timer = 2;

double back_turn_timer = 1;
double back_stay_timer = 4;

double timer; //Timer to check how many seconds passed 
double turn_count; //Track turn to have accurate 3.14 radian turn

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
    ent->model = gf3d_model_load("player");
    ent->think = agumon_think;
    ent->update = agumon_update;
    vector3d_copy(ent->position,position);

    status = 1;
    timer = SDL_GetTicks() / 1000.0;
    turn_count = 0;

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
        double rotation = 3.14 * (SDL_GetTicks() / 1000.0 - timer) / back_turn_timer;
        self->rotation.z -= rotation;
        turn_count += rotation;
        timer = SDL_GetTicks() / 1000.0;

        if (turn_count >= 3.14)
        {
            turn_count = 0;
            status = 2;
            timer = SDL_GetTicks() / 1000.0;
            turn = false;
            slog("RED LIGHT");
        }
    }

    if (status == 2)
    {
        if ((SDL_GetTicks()/1000 - timer) >= back_stay_timer)
        {
            status = 3;
            timer = SDL_GetTicks() / 1000.0;
        }
    }

    if (status == 3)
    {
        double rotation = 3.14 * (SDL_GetTicks() / 1000.0 - timer) / front_turn_timer;
        self->rotation.z += rotation;
        turn_count += rotation;
        timer = SDL_GetTicks() / 1000.0;

        if (turn_count >= 3.14)
        {
            turn_count = 0;
            status = 4;
            timer = SDL_GetTicks() / 1000.0;
            turn = true;
            slog("GREEN LIGHT");
        }
    }

    if (status == 4)
    {
        if ((SDL_GetTicks() / 1000 - timer) >= front_stay_timer)
        {
            status = 1;
            timer = SDL_GetTicks() / 1000.0;
            back_stay_timer *= 0.9; //Makes timer faster and faster 
            back_turn_timer *= 0.9;
            front_turn_timer *= 0.9;
        }
    }
}

/*eol@eof*/
