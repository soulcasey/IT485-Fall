#ifndef __ITEM_H__
#define __ITEM_H__

#include "entity.h"
#include <stdbool.h>

/**
 * @brief Create a new item entity
 * @param position where to spawn the item at
 * @return NULL on error, or an item entity pointer on success
 */
Entity* item_new(Vector3D position);

int item_position_y();

#endif
