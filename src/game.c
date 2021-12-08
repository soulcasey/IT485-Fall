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
int timescore;

int mousex, mousey;
float mouseFrame = 0;
Sprite *mouse = NULL;

bool gamestart = true;

int main(int argc, char* argv[])
{
    srand(time(NULL));

    bool active = false;
    bool hub = false;
    bool done = false;

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

    timescore = SDL_GetTicks();

    gfc_audio_init(3, 1, 1, 1, true, true);

    Entity* agumon = agumon_new(vector3d(0, 320, 0));

    Entity* death = agumon_death(vector3d(0, -100, 0));

    Entity* background = game_background(vector3d(0, 1000, 0));

    floor_real_new(vector3d(-4, 0, -7));
    floor_position_array[0] = 0;
    Entity* dalgoona;

    Entity* real[10];
    Entity* fake[10];

    int dalgoona_random = 0;
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
        if (i == dalgoona_random) //Summon random dalgoona on real floor between 1~5
        {
            dalgoona = dalgoona_new(vector3d(4 * random, (dalgoona_random + 1) * 30, -6));
        }
        real[i] = floor_real_new(vector3d(4 * random, (i + 1) * 30, -7));
        floor_position_array[i + 1] = (i + 1) * 30  * random;
        fake[i] = floor_fake_new(vector3d(-4 * random, (i + 1) * 30, -7));
    }

    gf3d_camera_set_scale(vector3d(1, 1, 1));
    Entity* player = player_new(vector3d(-4, -11, 0));

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
        gf3d_sprite_draw(mouse, vector2d(mousex, mousey), vector2d(1, 1), (Uint32)mouseFrame);
        gf3d_vgraphics_render_end();

        if ((abs(player_position_y() - 11 - (dalgoona_random+1)*30) <= 1) && !dalgoona_finish && !dalgoona_game && !player_dead()) // Start dalgoona game when reach
        {
            dalgoona_count = 0;
            dalgoona_game = true;
            dalgoona->position.y -= 13.5;
            dalgoona->position.z = -2.2;
            slog("Dalgoona Start");
        }


        if (dalgoona_count >= 10) // End dalgoona game
        {
            dalgoona_count = 0;
            dalgoona_game = false;
            speed *= 2;
            jump_rest_timer /= 2;
            dalgoona->scale = vector3d(0, 0, 0);
            dalgoona_finish = true;
        }

        if (((-gf3d_camera_get_y_position() > 290) || player_dead()) && !done) // Death
        {
            timescore = SDL_GetTicks() - timescore;
            slog("Time(s): %i", timescore / 1000);
            slog("Distance: %i", finalscore);
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
                    dalgoona->position.z = -6;
                     
                }
                dalgoona->scale = vector3d(0.5, 0.5, 0.5);
                fake[i]->position.x *= random;
                dalgoona_game = false;
                dalgoona_finish = false;
                dalgonna_crunch = false;
            }
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