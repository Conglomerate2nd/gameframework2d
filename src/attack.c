#include "simple_logger.h"
#include "simple_json.h"

#include "attack.h"
#include "world.h"

void attackThink(Entity* self);
void attackUpdate(Entity* self);
void attackFree(Entity* self);



Entity* attack_new_pos1(int x, int y, int type)
{
	//SJson *json = sj_load("src/entity.cfg");
	Entity* self;
	self = entity_new();
	slog("shot");
	if (!self)
	{
		slog("Failed to create new attack");
		return NULL;
	}


	//can all be defined either here through hard coding or by using a json file

	//GFC_TextLine	name; /**<name of the entity for debuggin>*/
	//Sprite* sprite;/**<Graphical represenations*/
	//float			frame;/**<for drawing*/
	//GFC_Vector2D	position;/**<where to draw*/
	//GFC_Vector2D	velocity;/**<how are we moving*/
	//GFC_Vector2D	acceleration;

	//to gt frame dimensions ddivide image dimensions by number of frames. 
	self->sprite = gf2d_sprite_load_all(
		"images/Sprites/GobbloAttack1.png",
		64,
		64,
		NULL,
		0
	);
	self->frame = 0;
	self->position = gfc_vector2d(x, y);
	self->acceleration.y = .5;//gravity for now
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	self->think = attackThink;
	self->update = attackUpdate;
	self->free = attackFree;
	self->directionX = 1;//walke foward first
	self->team = ETT_weapon;
	
	return self;
}

Entity* attack_new_pos(int x, int y,int type,int ttl,int directionX)
{
	//SJson *json = sj_load("src/entity.cfg");
	Entity* self;
	self = entity_new();
	slog("shot");
	if (!self)
	{
		slog("Failed to create new attack");
		return NULL;
	}


	//can all be defined either here through hard coding or by using a json file

	//GFC_TextLine	name; /**<name of the entity for debuggin>*/
	//Sprite* sprite;/**<Graphical represenations*/
	//float			frame;/**<for drawing*/
	//GFC_Vector2D	position;/**<where to draw*/
	//GFC_Vector2D	velocity;/**<how are we moving*/
	//GFC_Vector2D	acceleration;

	//to gt frame dimensions ddivide image dimensions by number of frames. 
	self->sprite = gf2d_sprite_load_all(
		"images/Sprites/GobbloAttack1.png",
		64,
		64,
		NULL,
		0
	);
	self->frame = 0;
	self->position = gfc_vector2d(x, y);
	self->acceleration.y = .5;//gravity for now
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	self->think = attackThink;
	self->update = attackUpdate;
	self->free = attackFree;
	self->team = ETT_weapon;
	self->directionX = directionX;//walke foward first
	//self->ttl == ttl;
	return self;
}



void attackThink(Entity* self)
{

	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	//world_tile_collide_active_entity(activeworld, self);
	self->position.x += 10*self->directionX;
	//if (self->ttl > 0) { self->ttl--; }
	slog("%i",self->position.x);
}
void attackUpdate(Entity* self)
{
	//slog(self->ttl);
	//if (self->ttl == 0)entity_free(self);
}
void attackFree(Entity* self)
{
	if (!self)return;
	entity_free(self);
}

