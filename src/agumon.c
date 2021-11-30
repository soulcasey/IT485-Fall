#include <stdbool.h>
#include <SDL.h>
#include "simple_logger.h"
#include "agumon.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <gfc_audio.h>

void agumon_think(Entity *self);
void agumon_update(Entity *self);

bool agumon_turn();
bool agumon_intial();

Sound RedLight;
Sound GreenLight;

int status = 1; //1 = turning back, 2 = back, 3 = turning front, 4 = front

double front_turn_timer = 0.5;
double front_stay_timer = 2;

double back_turn_timer = 1;
double back_stay_timer = 4;

double timer; //Timer to check how many seconds passed 

bool turn = false;
bool initial = false;

Entity *agumon_new(Vector3D position)
{
    Entity *ent = NULL;
    
    ent = entity_new();
    if (!ent)
    {
        return NULL;
    }
    ent->model = gf3d_model_load("bot");
    ent->think = agumon_think;
    ent->update = agumon_update;
    ent->scale = vector3d(2, 2, 2);
    vector3d_copy(ent->position,position);
    timer = SDL_GetTicks() / 1000.0;
    return ent;
}

Sound *red_light()
{
    Sound *audio = NULL;
    audio = gfc_sound_load("sounds/redlight.wav", 1.0, 0);
    if (!audio)
    {
        slog("Failed AHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
        return NULL;
    }
    return audio;
}

Sound* green_light()
{
    Sound* audio = NULL;
    audio = gfc_sound_load("sounds/greenlight.wav", 1.0, 0);
    if (!audio)
    {
        slog("Failed AHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
        return NULL;
    }
    return audio;
}

bool agumon_turn() //True if bot turned front, false if bot turned back
{
    return turn;
}

bool agumon_initial() // Turns true one agumon first turns back
{
    return initial;
}

void agumon_think(Entity *self)
{
    if (!self)return;
}

void agumon_update(Entity* self)
{
    if (!self)return;

    if (status == 1)
    {
        double rotation = 3.14 * (SDL_GetTicks() / 1000.0 - timer) / back_turn_timer;
        self->rotation.z -= rotation;
        timer = SDL_GetTicks() / 1000.0;

        if (self->rotation.z <= -3.14)
        {
            self->rotation.z = -3.14; //Accurately adjust back turn to 3.14 radian
            status = 2;
            timer = SDL_GetTicks() / 1000.0;
            turn = false;
            initial = true;
            slog("RED LIGHT");
            gfc_sound_play(red_light(), 0, 1.0, -1, -1);
        }
    }

    if (status == 2)
    {
        if ((SDL_GetTicks() / 1000 - timer) >= back_stay_timer)
        {
            status = 3;
            timer = SDL_GetTicks() / 1000.0;
        }
    }

    if (status == 3)
    {
        double rotation = 3.14 * (SDL_GetTicks() / 1000.0 - timer) / front_turn_timer;
        self->rotation.z += rotation;
        timer = SDL_GetTicks() / 1000.0;

        if (self->rotation.z >= 0)
        {
            self->rotation.z = 0; //Accurately adjust front turn to 0 radian
            status = 4;
            timer = SDL_GetTicks() / 1000.0;
            turn = true;
            slog("GREEN LIGHT");
            gfc_sound_play(green_light(), 0, 1.0, -1, -1);
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
