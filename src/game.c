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

bool floor_real_step();
int floor_position_array[11];
int finalscore;
bool dalgoona_game = false;
int dalgoona_count = 0;
bool dalgonna_crunch = false;
int timescore;

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
        1280,                   //screen width
        720,                   //screen height
        vector4d(1, 0, 0, 1),      //background color
        0,                      //fullscreen
        validate                //validation
    );
    slog_sync();

    timescore = SDL_GetTicks();
    
    entity_system_init(1024);

    gfc_audio_init(3, 1, 1, 1, true, true);

    Entity* agumon = agumon_new(vector3d(0, 320, 0));

    Entity* death = agumon_death(vector3d(0, -100, 0));

    floor_real_new(vector3d(-4, 0, -7));
    floor_position_array[0] = 0;
    Entity* item;

    Entity* real[10];
    Entity* fake[10];

    int item_random = 4;
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

        if ((abs(player_position_y() - 11 - (item_random+1)*30) <= 1) && item->_inuse == 1 && !dalgoona_game && !player_dead()) // Start dalgoona game when reach
        {
            dalgoona_count = 0;
            dalgoona_game = true;
            slog("Dalgoona Start");
        }

        if (dalgoona_count >= 10) // End dalgoona game
        {
            dalgoona_count = 0;
            dalgoona_game = false;
            slog("Yum Dalgoona");
            dalgoona_game = false;
            speed *= 2;
            jump_rest_timer /= 2;
            item->scale = vector3d(0, 0, 0);
        }

        if (((-gf3d_camera_get_y_position() > 290) || player_dead()) && !done) // Death
        {
            timescore = SDL_GetTicks() - timescore;
            slog("Time(s): %i", timescore / 1000);
            slog("Distance: %i", finalscore);
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
                item->position.x *= random;
                item->scale = vector3d(0.5, 0.5, 0.5);
                fake[i]->position.x *= random;
                dalgoona_game = false;
            }
            timescore = SDL_GetTicks();
            reset = false;
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