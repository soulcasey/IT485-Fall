#ifndef __GAME_H__
#define __GAME_H__

#include "entity.h"
#include <stdbool.h>
#include "gf3d_sprite.h"

bool floor_real_step();

bool dalgoona_game;

int dalgoona_count;

bool dalgoona_finish;

bool dalgoona_crunch;

int finalscore;

bool gamestart;

bool marble_game;

bool marble_even;

bool marble_finish;

bool marble_wrong;

bool done;

bool win;

int difficulty;

Sprite* difficulty_icon;

int difficulty_level;

#endif