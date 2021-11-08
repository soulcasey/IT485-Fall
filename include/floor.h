#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "entity.h"
#include <stdbool.h>

/**
 * @brief Create a new agumon entity
 * @param position where to spawn the aguman at
 * @return NULL on error, or an agumon entity pointer on success
 */
Entity* floor_real_new(Vector3D position);

Entity* floor_fake_new(Vector3D position);

#endif
