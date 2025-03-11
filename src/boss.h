#ifndef __BOSS_H__
#define __BOSS_H__

#include "entity.h"


/**
* @brief creates a new boss
* @return  returns a pointer to the new player returns null on failure
*/
Entity* boss_new();
Entity* boss_new_pos(int x, int y);
#endif