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

void menu();
void game(int argc, char* argv[]);
void end();
void dalgoona_square();

bool active = false;
bool hub = true;
bool done = false;

bool dalgoona_game = false;
int dalgoona_count = 0;

int score;
const Uint8* keys;
Uint8 validate = 0;
int floor_position_array[11];


int main(int argc,char *argv[])
{
    srand(time(NULL));
    int a;
    
    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1280,                   //screen width
        720,                    //screen height
        vector4d(1,0,0,1),      //background color
        0,                      //fullscreen
        validate                //validation
    );

    menu();
    game(argc, argv);
    //end();
}

void menu() //Main menu hub
{
    slog("Main Menu");
    slog("Press SPACE to start the game");
    while (hub)
    {
        keys = SDL_GetKeyboardState(NULL);
        SDL_PumpEvents();
        if (keys[SDL_SCANCODE_SPACE])
        {
            hub = false;
        }
        gf3d_vgraphics_render_start();
        gf3d_vgraphics_render_end();
    }
}

void game(int argc, char* argv[])
{
    slog("Game Begin");
    score = SDL_GetTicks();
    World* w;
    for (int a = 1; a < argc; a++)
    {
        if (strcmp(argv[a], "-disable_validate") == 0)
        {
            validate = 0;
        }
    }
    entity_system_init(1024);
    slog_sync();

    Entity* agumon = agumon_new(vector3d(0, 320, 0));
    floor_real_new(vector3d(-4, 0, -7));
    floor_position_array[0] = 0;
    Entity* item;

    int item_random = rand() % 1 + 1;
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
        if (i == item_random) //Summon random item on real floor between 1~5
        {
            item = item_new(vector3d(4 * random, (i + 1) * 30, -6));
        }
        floor_real_new(vector3d(4 * random, (i + 1) * 30, -7));
        floor_position_array[i + 1] = (i + 1) * 30  * random;
        floor_fake_new(vector3d(-4 * random, (i + 1) * 30, -7));
    }

    gf3d_camera_set_scale(vector3d(1, 1, 1));
    Entity* player = player_new(vector3d(-4, -11, 0));


    while (!hub)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

        entity_think_all();
        entity_update_all();
        gf3d_camera_update_view();
        gf3d_camera_get_view_mat4(gf3d_vgraphics_get_view_matrix());

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        gf3d_vgraphics_render_start();
        world_draw(w);
        entity_draw_all();

        gf3d_vgraphics_render_end();

        if ((abs(player_position_y() - (item_random+1)*30) <= 1) && item->_inuse == 1)
        {
            dalgoona_game = true;
            if (dalgoona_count == 7)
            {
                slog("Yum Dalgoona");
                dalgoona_game = false;
                speed *= 2;
                jump_rest_timer /= 2;

                entity_free(item);
            }
            dalgoona_square();

        }

        if (((-gf3d_camera_get_y_position() > 290) || player_dead()) && !done)
        {
            score = SDL_GetTicks() - score;
            slog("Score(s): %i", score / 1000);
            done = true;
        }

        if (done && keys[SDL_SCANCODE_ESCAPE])
        {
            hub = true;
        }
    }
    world_delete(w);
    entity_free(agumon);
    slog_sync();
}

void end() //end hub
{
    slog("Game Over");
    slog("Press SPACE or ESC to end the game");
    while (hub)
    {
        keys = SDL_GetKeyboardState(NULL);
        SDL_PumpEvents();
        if (keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_ESCAPE])
        {
            hub = false;
        }
        gf3d_vgraphics_render_start();
        gf3d_vgraphics_render_end();
    }
}

void dalgoona_square()
{
    if (keys[SDL_SCANCODE_S] && dalgoona_count == 0)
    {
        dalgoona_count = 1;
    }
    if (keys[SDL_SCANCODE_Q] && dalgoona_count == 1)
    {
        dalgoona_count = 2;
    }
    if (keys[SDL_SCANCODE_U] && dalgoona_count == 2)
    {
        dalgoona_count = 3;
    }
    if (keys[SDL_SCANCODE_A] && dalgoona_count == 3)
    {
        dalgoona_count = 4;
    }
    if (keys[SDL_SCANCODE_R] && dalgoona_count == 4)
    {
        dalgoona_count = 5;
    }
    if (keys[SDL_SCANCODE_E] && dalgoona_count == 5)
    {
        dalgoona_count = 6;
    }
    if (keys[SDL_SCANCODE_RETURN] && dalgoona_count == 6)
    {
        dalgoona_count = 7;
    }
}

/*eol@eof*/