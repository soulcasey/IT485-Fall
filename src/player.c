#include "simple_logger.h"
#include <SDL.h>
#include "gfc_types.h"
#include <stdbool.h>
#include "gf3d_camera.h"
#include "player.h"
#include "agumon.h"
#include "game.h"
#include <math.h>

bool floor_real_step();
bool player_move();
int player_position_y();
bool player_grounded();
void player_think(Entity *self);
void player_update(Entity *self);
//int floor_position(int n);

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
int jump_rest_count = 0;//Need to walk 20 steps for next jump
Vector3D camera_position;

bool grounded = true; //Check if the player is on ground
bool peaked = false; //Check if the player reached maximum height
bool jump_forward = false;
bool jump_left = false;
bool jump_right = false;
bool jump_stop = false; //Avoid accidental double jump
bool dead = false;

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

}

int player_position_y()
{
    return position_y;
}

bool player_grounded()
{
    return grounded;
}

bool player_move()
{
    return move;
}

void player_think(Entity* self)
{
    const Uint8* keys;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

    if ((agumon_turn() && player_move()) || gf3d_camera_get_z_position() > 50) // Death when player move while agumon turn or fall
    {
        self->position = vector3d(0, 0, -100);
        dead = true;
        self->rotation.x = 0;
        self->rotation.y = 0;
    }

    if (!dead)
    {
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
            jump_rest_count++;
        }
        else if (keys[SDL_SCANCODE_D] && !leftfeet && grounded)
        {
            self->position.y += speed;
            leftfeet = true;
            move = true;
            slog("Move Forward");
            jump_stop = false;
            jump_rest_count++;
        }

        else
        {
            move = true;
        }

        if ((keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_Q] || keys[SDL_SCANCODE_E] || jump_forward || jump_left || jump_right) && !jump_stop && jump_rest_count >= 20)
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
                jump_rest_count = 0;
            }
        }
        position_y = self->position.y;

        if (grounded && !floor_real_step())
        {
            self->velocity.z = -jump_speed * 1.5;
        }
    }
}

void player_update(Entity *self)
{
    if (!self)return;
    if (!dead)
    {
        camera_position = vector3d(self->position.x, self->position.y - 15, self->position.z + 3);
    }
    else
    {
        camera_position = vector3d(self->position.x, self->position.y - 1.2, self->position.z);
    }
    gf3d_camera_set_position(camera_position);
    gf3d_camera_set_rotation(vector3d(-M_PI, M_PI, 0));
}

bool player_dead()
{
    return dead;
}

bool floor_real_step()
{
    for (int i = 0; i < 11; i++)
    {
        if (((floor_position_array[i] <= 0) && left) || ((floor_position_array[i] > 0) && !left))
        {
            if ((abs(player_position_y()) >= (abs(floor_position_array[i]) - 11)) && (abs(player_position_y()) <= (abs(floor_position_array[i]) + 11)))
            {
                return true;
            }
        }
    }
    return false;
}

/*eol@eof*/