#include "simple_logger.h"
#include "gfc_types.h"
#include <stdbool.h>
#include "gf3d_camera.h"
#include "player.h"

bool player_move();
void player_think(Entity *self);
void player_update(Entity *self);
Bool leftfeet = true;
Bool move = false;
float speed = 1;

Entity *player_new(Vector3D position)
{
    Entity *ent = NULL;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no player for you!");
        return NULL;
    }
    
//    ent->model = gf3d_model_load("dino");
    ent->think = player_think;
    ent->update = player_update;
    vector3d_copy(ent->position,position);
    ent->rotation.x = -M_PI;
    return ent;
}

bool player_move()
{
    return move;
}

void player_think(Entity *self)
{
    const Uint8 * keys;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

    if (keys[SDL_SCANCODE_LEFT] && keys[SDL_SCANCODE_RIGHT])
    {
        move = false;
        //Ignore A and D pressed at the same time
    }

    else if (keys[SDL_SCANCODE_LEFT] && leftfeet)
    {
        self->position.y += speed;
        leftfeet = false;
        move = true;
        slog("Move Forward");
    }
    else if (keys[SDL_SCANCODE_RIGHT] && !leftfeet)
    {
        self->position.y += speed;
        leftfeet = true;
        move = true;
        slog("Move Forward");
    }

    else
    {
        move = true;
    }
}

void player_update(Entity *self)
{
    if (!self)return;
    gf3d_camera_set_position(self->position);
    gf3d_camera_set_rotation(self->rotation);
}

/*eol@eof*/
