#ifndef __ITEMS_H__
#define __ITEMS_H__

#include "entity.h"


/**
* @brief creates a new walker
* @return  returns a pointer to the new player returns null on failure
*/
Entity* items_new();
Entity* items_new_pos(int x, int y);
#endif