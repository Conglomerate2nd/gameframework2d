#ifndef __ATTACK_H__
#define __ATTACK_H__

#include "entity.h"


/**
* @brief creates a new walker
* @return  returns a pointer to the new player returns null on failure
*/
Entity* attack_new_pos1(int x, int y, int type, int direction);

Entity* attack_new_pos(int x, int y, int type, int ttl,int directionX);

void attack0(Entity* self);
void attack1(Entity* self);
void attack2(Entity* self);
void attack3(Entity* self);
void attack4(Entity* self);
void attack5(Entity* self);

//ttl is time to live. If set to -1, then do not use this
#endif