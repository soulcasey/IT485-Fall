#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "entity.h"
#include <stdbool.h>

/**
 * @brief Create a new floor entity
 * @param position where to spawn the floor at
 * @return NULL on error, or an floor entity pointer on success
 */
Entity* floor_real_new(Vector3D position);

Entity* floor_fake_new(Vector3D position);

#endif
