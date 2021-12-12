#ifndef __ITEM_H__
#define __ITEM_H__

#include "entity.h"
#include <stdbool.h>

/**
 * @brief Create a new item entity
 * @param position where to spawn the item at
 * @return NULL on error, or an item entity pointer on success
 */
Entity* dalgoona_new(Vector3D position);
Entity* marble_new(Vector3D position);
Entity* money_new(Vector3D position);

int dalgoona_position_y();

int marble_position_y();

#endif
