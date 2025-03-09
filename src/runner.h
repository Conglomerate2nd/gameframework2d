#ifndef __RUNNER_H__
#define __RUNNER_H__

#include "entity.h"


/**
* @brief creates a new walker
* @return  returns a pointer to the new player returns null on failure
*/
Entity* runner_new();
Entity* runner_new_pos(int x, int y);
#endif