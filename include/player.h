#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

/**
 * @brief Create a new player entity
 * @param position where to spawn the aguman at
 * @return NULL on error, or an player entity pointer on success
 */
Entity *player_new(Vector3D position);

bool player_move();

bool player_grounded();

bool reset;

bool player_dead();

bool left;

int player_position_y();

double speed;

int jump_rest_timer;

#endif
