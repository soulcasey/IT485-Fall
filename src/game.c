#include <SDL.h>    
#include <time.h>
#include <math.h>
#include <stdio.h>

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"

#include "entity.h"
#include "agumon.h"
#include "player.h"
#include "world.h"
#include "floor.h"
#include "item.h"
#include "game.h"
#include "gf3d_sprite.h"

bool floor_real_step();
int floor_position_array[11];
int finalscore;
bool dalgoona_game = false;
int dalgoona_count = 0;
bool dalgonna_crunch = false;
bool dalgoona_finish = false;
bool dalgonna_mouse[] = { false, false, false };
bool dalgoona_crack = false;
bool dalgoona_boost = false;
Model* Dalgoona_model;
Model* Dalgonna_model_cracked;
Sprite* dalgoona_icon;

bool marble_game = false;
bool marble_finish = false;
bool marble_even;
bool marble_wrong = false;;
bool marble_boost = false;
Model* marble_model;
Model* marble_model_hold;
Sprite* marble_icon;

int timescore;

Sprite* distance = NULL;
Sprite* distancebar = NULL;
float distancey = 0.0;

int mousex, mousey;
float mouseFrame = 0;
Sprite* mouse = NULL;

Sprite* victory;

int mouse1[] = { 965, 325 };
int mouse2[] = { 711, 785 };
int mouse3[] = { 1220, 765 };

bool done = false;
bool gamestart = false;
bool win = false;

int main(int argc, char* argv[])
{
    srand(time(NULL));

    bool active = false;
    bool hub = false;

    Uint8 validate = 0;
    World* w;

    for (int a = 1; a < argc; a++)
    {
        if (strcmp(argv[a], "-disable_validate") == 0)
        {
            validate = 0;
        }
    }

    init_logger("gf3d.log");
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1920,                   //screen width
        1080,                   //screen height
        vector4d(1, 0, 0, 1),      //background color
        0,                      //fullscreen
        validate                //validation
    );
    slog_sync();
    
    entity_system_init(1024);

    mouse = gf3d_sprite_load("images/pointer.png", 32, 32, 16);

    distance = gf3d_sprite_load("images/distance.png", 100, 100, 1);
    distancebar = gf3d_sprite_load("images/distancebar.png", 100, 500, 1);
    timescore = SDL_GetTicks();

    dalgoona_icon = gf3d_sprite_load("images/dalgoona_gain.png", 250, 250, 1);
    marble_icon = gf3d_sprite_load("images/marble_gain.png", 250, 250, 1);

    gfc_audio_init(4, 1, 1, 1, true, true);

    Entity* money = money_new(vector3d(0, 320, 0));

    Entity* agumon = agumon_new(vector3d(0, 320, 0));

    Entity* death = agumon_death(vector3d(0, -100, 0));

    Entity* mainmenu = main_menu(vector3d(-2.5, -35, -5));
    Entity* background = game_background(vector3d(0, 1000, 0));

    floor_real_new(vector3d(-4, 0, -7));
    floor_position_array[0] = 0;

    Entity* dalgoona;
    Entity* marble;
    marble_model_hold = gf3d_model_load("marble_hold");

    Dalgoona_model = gf3d_model_load("dalgoona");
    Dalgonna_model_cracked = gf3d_model_load("dalgoona_crack");

    victory = gf3d_sprite_load("images/win.png", 2303, 383, 1);

    Entity* real[10];
    Entity* fake[10];

    int dalgoona_random = 0;
    int marble_random = 4;

    int random = rand() % 100;
    if (random < 50)
    {
        marble_even = true;
        marble_model = gf3d_model_load("marble_even");

    }
    else
    {
        marble_even = false;
        marble_model = gf3d_model_load("marble_odd");
    }

    for (int i = 0; i < 10; i++)
    {
        int random = rand() % 100;
        if (random < 50)
        {
            random = 1;
        }
        else
        {
            random = -1;
        }
        if (i == dalgoona_random) //Summon dalgoona
        {
            dalgoona = dalgoona_new(vector3d(4 * random, (dalgoona_random + 1) * 30, -5.5));
        }
        else if (i == marble_random) //Summon marble
        {
            marble = marble_new(vector3d(4 * random, (marble_random + 1) * 30, -5.5));
            marble->model = marble_model;
        }
        real[i] = floor_real_new(vector3d(4 * random, (i + 1) * 30, -7));
        floor_position_array[i + 1] = (i + 1) * 30  * random;
        fake[i] = floor_fake_new(vector3d(-4 * random, (i + 1) * 30, -7));
    }

    gf3d_camera_set_scale(vector3d(1, 1, 1));
    Entity* player = player_new(vector3d(-4, -11, -0));

    while (!hub)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        const Uint8* keys;
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

        SDL_GetMouseState(&mousex, &mousey);
        mouseFrame += 0.01;
        if (mouseFrame >= 16)mouseFrame = 0;

        entity_think_all();
        entity_update_all();
        gf3d_camera_update_view();
        gf3d_camera_get_view_mat4(gf3d_vgraphics_get_view_matrix());

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from tdddddddddddddddddddddddddddddddddddhe pool
        gf3d_vgraphics_render_start();
        world_draw(w);
        entity_draw_all();

        distancey = 185.0 * player_position_y() / 290.0;
        gf3d_sprite_draw(distancebar, vector2d(200, 300), vector2d(1, 1), (Uint32)0);
        gf3d_sprite_draw(distance, vector2d(200, 500 - distancey), vector2d(1, 1), (Uint32)0);
        gf3d_sprite_draw(mouse, vector2d(mousex, mousey), vector2d(1, 1), (Uint32)mouseFrame);

        money->rotation.z += 0.001;

        if (dalgoona_boost)
        {
            gf3d_sprite_draw(dalgoona_icon, vector2d(160, 560), vector2d(1, 1), (Uint32)0);
        }

        if (marble_boost)
        {
            gf3d_sprite_draw(marble_icon, vector2d(160, 700), vector2d(1, 1), (Uint32)0);
        }

        if (win)
        {
            gf3d_sprite_draw(victory, vector2d(400, 180), vector2d(1, 1), (Uint32)0);
        }

        gf3d_vgraphics_render_end();

        if ((abs(player_position_y() - 11 - (dalgoona_random+1)*30) <= 1) && !dalgoona_finish && !dalgoona_game && !player_dead()) // Start dalgoona game when reach
        {
            dalgoona_count = 0;
            dalgoona_game = true;
            dalgoona->scale = vector3d(0.5, 0.5, 0.5);
            dalgoona->position.y -= 13.5;
            dalgoona->position.z = -2.2;
            slog("Dalgoona Start");
        }

        if (!dalgoona_crack && dalgoona_game && (SDL_GetMouseState(&mousex, &mousey) & SDL_BUTTON_LMASK) != 0) // mouse dalgoona
        {
            if ((abs(mousex - mouse1[0]) <= 30) && (abs(mousey - mouse1[1]) <= 30))
            {
                dalgonna_mouse[0] = true;
            }
            else if ((abs(mousex - mouse2[0]) <= 30) && (abs(mousey - mouse2[1]) <= 30))
            {
                dalgonna_mouse[1] = true;
            }
            else if ((abs(mousex - mouse3[0]) <= 30) && (abs(mousey - mouse3[1]) <= 30))
            {
                dalgonna_mouse[2] = true;
            }
            else
            {
                dalgoona->model = Dalgonna_model_cracked;
                dalgoona_crack = true;
            }
        }

        if (((dalgoona_count >= 10) || (dalgonna_mouse[0] && dalgonna_mouse[1] && dalgonna_mouse[2])) && !dalgoona_crack && dalgoona_game) // End dalgoona game
        {
            if (dalgoona_count >= 10)
            {
                speed *= 1.2;
                jump_rest_timer /= 1.2;
            }
            else
            {
                speed *= 1.5;
                jump_rest_timer /= 1.5;
            }
            dalgoona_count = 0;
            dalgoona_game = false;
            dalgoona->scale = vector3d(0, 0, 0);
            dalgoona_finish = true;
            for (int i = 0; i < 3; i++)
            {
                dalgonna_mouse[i] = false;
            }
            dalgoona_boost = true;
        }

        if ((abs(player_position_y() - 11 - (marble_random + 1) * 30) <= 1) && !marble_finish && !marble_game && !player_dead()) // Start marble game when reach
        {
            marble_game = true;
            marble->scale = vector3d(0.5, 0.5, 0.5);
            marble->model = marble_model_hold;
            marble->position.y -= 13.5;
            marble->position.z = -2.2;
            slog("Marble Start");
        }

        if (marble_game && marble_finish)
        {
            marble->scale = vector3d(0, 0, 0);
            marble_game = false;
            marble_boost = true;
            speed *= 1.2;
            jump_rest_timer /= 1.2;
        }

        if (marble_game && marble_wrong)
        {
            marble->model = marble_model;
        }


        if ((-gf3d_camera_get_y_position() > 290) && !done) // Reached
        {
            timescore = SDL_GetTicks() - timescore;
            agumon->velocity.z = -0.01;
            slog("Time(s): %i", timescore / 1000);
            slog("Distance: %i", finalscore);
            win = true;
            done = true;
        }

        if (player_dead() && !done) // Death
        {
            background->model = gf3d_model_load("background_death");
            done = true;
        }

        if (reset) // Rerandomize floor
        {
            for (int i = 0; i < 10; i++)
            {
                int random = rand() % 100;
                if (random < 50)
                {
                    random = 1;
                }
                else
                {
                    random = -1;
                }

                real[i]->position.x *= random;
                floor_position_array[i + 1] *= random;
                if (i == dalgoona_random)
                {
                    dalgoona->position.x *= random;
                    dalgoona->position.y = (dalgoona_random + 1) * 30;
                    dalgoona->position.z = -5.5;
                    dalgoona->scale = vector3d(1, 1, 1);
                }
                else if (i == marble_random) //Summon marble
                {
                    marble->position.x *= random;
                    marble->position.y = (marble_random + 1) * 30;
                    marble->position.z = -5.5;
                    marble->scale = vector3d(1, 1, 1);
                }
                fake[i]->position.x *= random;
            }
            for (int i = 0; i < 3; i++)
            {
                dalgonna_mouse[i] = false;
            }
            dalgoona_game = false;
            dalgoona_finish = false;
            dalgonna_crunch = false;
            dalgoona_crack = false;
            dalgoona_boost = false;
            dalgoona->model = Dalgoona_model;

            agumon->velocity.z = 0;
            agumon->position.z = 0;

            marble_game = false;
            marble_finish = false;
            marble_wrong = false;
            marble_boost = false;
            int random = rand() % 100;
            if (random < 50)
            {
                marble_even = true;
                marble_model = gf3d_model_load("marble_even");

            }
            else
            {
                marble_even = false;
                marble_model = gf3d_model_load("marble_odd");
            }
            marble->model = marble_model;

            status = 1;
            agumon->rotation.z = 0;
            timer = SDL_GetTicks() / 1000.0;
            reset = false;
            done = false;
            background->model = gf3d_model_load("background_game");
        }

        if (keys[SDL_SCANCODE_ESCAPE])
        {
            hub = true;
        }
    }
    world_delete(w);
    entity_free(agumon);
    slog_sync();
}

bool floor_real_step()
{
    for (int i = 0; i < 11; i++)
    {
        if (((floor_position_array[i] <= 0) && left) || ((floor_position_array[i] > 0) && !left))
        {
            if ((player_position_y() >= (abs(floor_position_array[i]))) && (player_position_y() <= (abs(floor_position_array[i]) + 22)))
            {
                return true;
            }
        }
    }
    return false;
}

/*eol@eof*/