#include "simple_logger.h"
#include <SDL.h>
#include "gfc_types.h"
#include <stdbool.h>
#include "gf3d_camera.h"
#include "player.h"

bool player_move();
int player_position_y();
void player_think(Entity *self);
void player_update(Entity *self);

bool left = true; //Check if player is on the left glass
bool leftfeet = true;
bool move = false; //Check if the player is not standing still
double speed = 0.5;

float position_y;
double timer;
double jump_height = 2;
double jump_speed = 0.05;
double jump_forward_speed = 0.04;
double jump_side_speed = 0.028;

bool grounded = true; //Check if the player is on ground
bool peaked = false; //Check if the player reached maximum height
bool jump_forward = false;
bool jump_left = false;
bool jump_right = false;
bool jump_stop = false; //Avoid accidental double jump

Entity *player_new(Vector3D position)
{
    Entity *ent = NULL;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no player for you!");
        return NULL;
    }
    ent->model = gf3d_model_load("bot");
    ent->scale = vector3d(0.2, 0.2, 0.2);
    ent->think = player_think;
    ent->update = player_update;
    vector3d_copy(ent->position,position);
    ent->rotation.x = -M_PI;
    ent->rotation.y = M_PI;
    ent->position.z = -5.2;
    return ent;
}

int player_position_y()
{
    return position_y;
}


bool player_move()
{
    return move;
}

void player_think(Entity* self)
{
    const Uint8* keys;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

    if (keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_D] && grounded)
    {
        move = false;
        //Ignore Left and Right pressed at the same time
    }

    else if (keys[SDL_SCANCODE_A] && leftfeet && grounded)
    {
        self->position.y += speed;
        leftfeet = false;
        move = true;
        slog("Move Forward");
        jump_stop = false; //Reset jump to make sure user doesnt accidentally double jump
    }
    else if (keys[SDL_SCANCODE_D] && !leftfeet && grounded)
    {
        self->position.y += speed;
        leftfeet = true;
        move = true;
        slog("Move Forward");
        jump_stop = false;
    }

    else
    {
        move = true;
    }

    if ((keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_Q] || keys[SDL_SCANCODE_E] || jump_forward || jump_left || jump_right) && !jump_stop)
    {
        if (grounded)
        {
            if (keys[SDL_SCANCODE_W])
            {
                jump_forward = true;
            }
            else if (keys[SDL_SCANCODE_Q])
            {
                jump_left = true;
            }
            else if (keys[SDL_SCANCODE_E])
            {
                jump_right = true;
            }
            grounded = false;
            slog("Jump");
        }

        self->velocity.y = jump_forward_speed;
        if (jump_left)
        {
            self->velocity.x = -jump_side_speed;
        }
        else if (jump_right)
        {
            self->velocity.x = jump_side_speed;
        }
        if (!peaked)
        {
            self->velocity.z = jump_speed;
        }
        else
        {
            self->velocity.z = -jump_speed;
        }

        if (self->position.z >= jump_height)
        {
            peaked = true;
        }
        if ((self->position.z <= -5.2) && peaked)
        {
            self->position.z = -5.2;
            self->velocity.x = 0;
            self->velocity.y = 0;
            self->velocity.z = 0;
            grounded = true;
            peaked = false;
            if (jump_left)
            {
                if (!left)
                {
                    self->position.x = -4;
                    left = !left;
                }
                else
                {
                    self->velocity.z = -jump_speed * 1.5;
                }
            }
            else if (jump_right)
            {
                if (left)
                {
                    self->position.x = 4;
                    left = !left;
                }
                else
                {
                    self->velocity.z = -jump_speed * 1.5;
                }
            }
            jump_forward = false;
            jump_left = false;
            jump_right = false;

            jump_stop = true;
        }
    }
    position_y = self->position.y;
    if (keys[SDL_SCANCODE_UP])
    {
        self->position.z += 0.001;
    }
    if (keys[SDL_SCANCODE_DOWN])
    {
        self->position.z -= 0.001;
    }
}

void player_update(Entity *self)
{
    if (!self)return;
    Vector3D x = vector3d(self->position.x, self->position.y - 15, self->position.z + 3);
    gf3d_camera_set_position(x);
    gf3d_camera_set_rotation(self->rotation);
}


/*eol@eof*/