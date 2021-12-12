#include "simple_logger.h"
#include <SDL.h>
#include "gfc_types.h"
#include <stdbool.h>
#include "gf3d_camera.h"
#include "player.h"
#include "agumon.h"
#include "game.h"
#include <math.h>
#include "gfc_audio.h"
#include "simple_json.h"

bool floor_real_step();
bool player_move();
int player_position_y();
bool player_grounded();
void player_think(Entity *self);
void player_update(Entity *self);

bool reset = false;
bool left = true; //Check if player is on the left glass
bool leftfeet = true;
bool move = true; //Check if the player is not standing still
double speed = 0.5;

float position_y;
double timer;
double jump_height = 2;
double jump_speed = 0.05;
double jump_forward_speed = 0.04;
double jump_side_speed = 0.028;
int jump_rest_count = 0;//Need to walk x steps for next jump
float jump_rest_timer = 30;
Vector3D camera_position;

bool grounded = true; //Check if the player is on ground
bool peaked = false; //Check if the player reached maximum height
bool jump_forward = false;
bool jump_left = false;
bool jump_right = false;
bool jump_stop = false; //Avoid accidental double jump
bool dead = false;

Model* stand;
Model* runleft;
Model* runright;
Model* jump;

const char* color;
SJson* json, * wjson;

bool end = false;

Entity *player_new(Vector3D position)
{
    json = sj_load("config/player.json");
    wjson = sj_object_get_value(json, "player");
    color = sj_get_string_value(sj_object_get_value(wjson, "color"));

    stand = gf3d_model_load_player("player_stand",color);
    runleft = gf3d_model_load_player("player_left", color);
    runright = gf3d_model_load_player("player_right", color);
    jump = gf3d_model_load_player("player_jump", color);

    Entity *ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no player for you!");
        return NULL;
    }
    ent->model = stand;
    ent->scale = vector3d(0.2, 0.2, 0.2);
    ent->think = player_think;
    ent->update = player_update;
    vector3d_copy(ent->position,position);
    ent->position.z = -5.2;
    ent->rotation.y = M_PI;
    ent->rotation.z = M_PI;

    camera_position = vector3d(ent->position.x, ent->position.y +10, ent->position.z + 2);
    gf3d_camera_set_position(camera_position);
    gf3d_camera_set_rotation(vector3d(-M_PI, M_PI, -M_PI));

}

Sound* death_sound()
{
    Sound* audio = NULL;
    audio = gfc_sound_load("sounds/death.wav", 1, 0);
    if (!audio)
    {
        slog("Failed AHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
        return NULL;
    }
    return audio;
}

Sound* winsound()
{
    Sound* audio = NULL;
    audio = gfc_sound_load("sounds/yay.wav", 1, 0);
    if (!audio)
    {
        slog("Failed AHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
        return NULL;
    }
    return audio;
}

int player_position_y()
{
    return position_y + 11;
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

    position_y = self->position.y;

    if (keys[SDL_SCANCODE_RETURN] && !gamestart) //Start game
    {
        timer = SDL_GetTicks() / 1000.0;
        gamestart = true;
    }

   if (((agumon_turn() && player_move()) || gf3d_camera_get_z_position() > 50 || dead) && !reset && !done) // Death when player move while agumon turn or fall
    {
        if (!dead)
        {
            gfc_sound_play(death_sound(), 0, 0.2, -1, -1);
        }
        finalscore = self->position.y + 11;
        self->position = vector3d(0, -115, 0);
        self->velocity.z = 0;
        self->velocity.y = 0;
        self->velocity.x = 0;
        dead = true;
    }
   
   if (win && !end)
   {
       gfc_sound_play(winsound(), 0, 0.2, -1, -1);
       self->velocity.z = 0;
       self->velocity.y = 0;
       self->velocity.x = 0;
       self->model = stand;
       end = true;
   }



    /*
    if (keys[SDL_SCANCODE_1])
    {
        color = "yellow";
        
        stand = gf3d_model_load_player("player_stand", color);
        run = gf3d_model_load_player("player", color);
        jump = gf3d_model_load_player("player_jump", color);
        self->model = run;
    }

    else if (keys[SDL_SCANCODE_2])
    {
        color = "blue";
        stand = gf3d_model_load_player("player_stand", color);
        run = gf3d_model_load_player("player", color);
        jump = gf3d_model_load_player("player_jump", color);
        self->model = run;
    }

    else if (keys[SDL_SCANCODE_3])
    {
        color = "green";
        stand = gf3d_model_load_player("player_stand", color);
        run = gf3d_model_load_player("player", color);
        jump = gf3d_model_load_player("player_jump", color);
        self->model = run;
    }

    else if (keys[SDL_SCANCODE_4])
    {
        color = "red";
        stand = gf3d_model_load_player("player_stand", color);
        run = gf3d_model_load_player("player", color);
        jump = gf3d_model_load_player("player_jump", color);
        self->model = run;
    }
    */
    


    if (((dead || win) && keys[SDL_SCANCODE_RETURN])) // Restart
    {
        slog("reset");
        dead = false;
        self->scale = vector3d(0.2, 0.2, 0.2);
        self->position.x = -4;
        self->position.y = -11;
        self->position.z = -5.2;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->velocity.z = 0;
        left = true;
        grounded = true;
        turn = false;
        initial = false;
        front_turn_timer = 0.5;
        back_turn_timer = 1;
        back_stay_timer = 4;
        speed = 0.5;
        jump_rest_timer = 30; 
        finalscore = 0;
        jump_left = false;
        jump_right = false;
        jump_forward = false;
        position_y = -11;
        end = false;
        win = false;
        reset = true;
    }

    if (dalgoona_game && keys[SDL_SCANCODE_SPACE] && !dalgoona_crunch) // Eating Dalgoona On
    {
        dalgoona_count++;
        dalgoona_crunch = true;
        slog("Munch");
    }

    if (dalgoona_game && !keys[SDL_SCANCODE_SPACE] && dalgoona_crunch) // Eating Dalgoona Off
    {
        dalgoona_crunch = false;
    }

    if (marble_game && !marble_wrong)
    {
        if (keys[SDL_SCANCODE_1])
        {
            if (!marble_even)
            {
                marble_finish = true;
            }
            else
            {
                marble_wrong = true;
            }
        }

        else if (keys[SDL_SCANCODE_2])
        {
            if (marble_even)
            {
                marble_finish = true;
            }
            else
            {
                marble_wrong = true;
            }
        }
    }


    if (!dead && !dalgoona_game && !marble_game && gamestart && !done)
    {
        if (keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_D] && grounded)
        {
            move = false;
            self->model = stand;
            //Ignore Left and Right pressed at the same time
        }

        else if (keys[SDL_SCANCODE_A] && leftfeet && grounded)
        {
            self->position.y += speed;
            leftfeet = false;
            move = true;
            jump_stop = false; //Reset jump to make sure user doesnt accidentally double jump
            jump_rest_count++;
            self->model = runleft;
        }
        else if (keys[SDL_SCANCODE_D] && !leftfeet && grounded)
        {
            self->position.y += speed;
            leftfeet = true;
            move = true;
            jump_stop = false;
            jump_rest_count++;
            self->model = runright;
        }

        else
        {
            move = true;
        }

        if ((keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_Q] || keys[SDL_SCANCODE_E] || jump_forward || jump_left || jump_right) && !jump_stop && jump_rest_count >= jump_rest_timer && self->position.z > -7.2)
        {
            if (grounded)
            {
                self->model = jump;
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
                self->model = stand;
            }
        }

        if (grounded && !floor_real_step())
        {
            slog("fall");
            self->velocity.z = -jump_speed * 1.5;
        }
    }
}

void player_update(Entity *self)
{
    if (!self)return;
    if (gamestart)
    {
        if (!dead)
        {
            camera_position = vector3d(self->position.x, self->position.y - 15, self->position.z + 3);
        }
        else
        {
            camera_position = vector3d(self->position.x, self->position.y + 1, self->position.z + 1);
        }
        gf3d_camera_set_position(camera_position);
        gf3d_camera_set_rotation(vector3d(-M_PI, M_PI, 0));
    }
}

bool player_dead()
{
    return dead;
}

/*eol@eof*/