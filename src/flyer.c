#include "simple_logger.h"
#include "simple_json.h"

#include "flyer.h"
#include "world.h"

void flyerThink(Entity* self);
void flyerUpdate(Entity* self);
void flyerFree(Entity* self);



Entity* flyer_new()
{
	Entity* self;
	self = entity_new();

	if (!self)
	{
		slog("Failed to create new flyer");
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
		"images/Sprites/Buggo.png",
		64,
		64,
		NULL,
		0
	);
	self->frame = 0;
	self->position = gfc_vector2d( 100,0);

	self->think = flyerThink;
	self->update = flyerUpdate;
	self->free = flyerFree;

	self->team = ETT_enemy;
	self->isFlying = 1;

	return self;
}


Entity* flyer_new_pos(int x,int y)
{
	Entity* self;
	self = entity_new();

	if (!self)
	{
		slog("Failed to create new flyer");
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
		"images/Sprites/Buggo.png",
		64,
		64,
		NULL,
		0
	);
	self->frame = 0;
	self->position = gfc_vector2d(x, y);

	self->think = flyerThink;
	self->update = flyerUpdate;
	self->free = flyerFree;

	self->team = ETT_enemy;
	self->isFlying = 1;
	self->directionX = 1;
	self->directionY = -1;
	self->health = 1;

	return self;
}

void flyerThink(Entity* self)
{
	self->position.x += 2 * self->directionX;
	self->position.y += 2 * self->directionY;
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	world_tile_collide_active_entity(activeworld, self);
}
void flyerUpdate(Entity* self) 
{
	if (self->health == 0) {
		slog("dead");
		walkerFree(self);

	}
}
void flyerFree(Entity* self) 
{
	if (!self)return;
	entity_free(self);
}

