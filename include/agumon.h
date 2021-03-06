#ifndef __AGUMON_H__
#define __AGUMON_H__

#include "entity.h"
#include <stdbool.h>

/**
 * @brief Create a new agumon entity
 * @param position where to spawn the aguman at
 * @return NULL on error, or an agumon entity pointer on success
 */
Entity* agumon_new(Vector3D position);

bool agumon_turn();

bool agumon_initial();

double timer;

int status;

bool turn;
bool initial;

double front_turn_timer;
double back_turn_timer;
double back_stay_timer;

#endif
