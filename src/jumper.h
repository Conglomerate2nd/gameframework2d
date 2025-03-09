#ifndef __JUMPER_H__
#define __JUMPER_H__

#include "entity.h"


/**
* @brief creates a new walker
* @return  returns a pointer to the new player returns null on failure
*/
Entity* jumper_new();
Entity* jumper_new_pos(int x, int y);
#endif