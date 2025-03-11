#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"


/**
* @brief creates a new player
* @return  returns a pointer to the new player returns null on failure
*/
Entity* player_new();

/**
* @brief creates a new player
* @param x and y are the ccordinates of where the plahyer spawns
* @return  returns a pointer to the new player returns null on failure
*/
Entity* player_new_pos(int x, int y);
Entity* player_get();
void playerFree(Entity* self);
void playerDamageSelf(Entity* self);

#endif