#ifndef __RUNNAWAYER_H__
#define __RUNNAWAYER_H__

#include "entity.h"


/**
* @brief creates a new walker
* @return  returns a pointer to the new player returns null on failure
*/
Entity* runnawayer_new();
Entity* runnawayer_new_pos(int x, int y);
#endif