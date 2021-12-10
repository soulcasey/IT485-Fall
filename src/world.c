#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_types.h"

#include "world.h"

/*
typedef struct
{

    Model *worldModel;
    List *spawnList;        //entities to spawn
    List *entityList;       //entities that exist in the world
}World;
*/

Entity* main_menu(Vector3D position)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("No menu");
        return NULL;
    }

    ent->model = gf3d_model_load("ui");
    ent->scale = vector3d(1, 1, 1);
    ent->rotation.z = M_PI;
    vector3d_copy(ent->position, position);
    return ent;
}


Entity* game_background(Vector3D position)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("No world");
        return NULL;
    }
    ent->model = gf3d_model_load("background_game");
    ent->scale = vector3d(2000, 1, 2000);
    vector3d_copy(ent->position, position);
    return ent;
}

World *world_load(char *filename)
{
    SJson *json,*wjson;
    World *w = NULL;
    const char *modelName = NULL;
    w = gfc_allocate_array(sizeof(World),1);
    if (w == NULL)
    {
        slog("failed to allocate data for the world");
        return NULL;
    }
    json = sj_load(filename);
    if (!json)
    {
        slog("failed to load json file (%s) for the world data",filename);
        free(w);
        return NULL;
    }
    wjson = sj_object_get_value(json,"world");
    if (!wjson)
    {
        slog("failed to find world object in %s world condig",filename);
        free(w);
        sj_free(json);
        return NULL;
    }
    modelName = sj_get_string_value(sj_object_get_value(wjson,"model"));
    if (modelName)
    {
        w->worldModel = gf3d_model_load((char *)modelName);
        gfc_matrix_identity(w->modelMat);
        gfc_matrix_scale(
            w->modelMat,
            vector3d(10,10,10)
        );
        gfc_matrix_translate(
            w->modelMat,
            vector3d(0,0,-20)
        );
    }
    else
    {
        slog("world data (%s) has no model",filename);
    }
    sj_free(json);
    return w;
}

void world_draw(World *world)
{
    if (!world)return;
    if (!world->worldModel)return;// no model to draw, do nothing
    gf3d_model_draw(world->worldModel,world->modelMat);
}

void world_delete(World *world)
{
    if (!world)return;
    gf3d_model_free(world->worldModel);
    free(world);
}

void world_run_updates(World *world);

void world_add_entity(World *world,Entity *entity);


/*eol@eof*/
