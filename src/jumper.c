#include "simple_logger.h"
#include "simple_json.h"

#include "jumper.h"
#include "world.h"

void jumperThink(Entity* self);
void jumperUpdate(Entity* self);
void jumperFree(Entity* self);



Entity* jumper_new()
{
	//SJson *json = sj_load("src/entity.cfg");
	Entity* self;
	self = entity_new();

	if (!self)
	{
		slog("Failed to create new jumper");
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
		"images/rocco.png",
		128,
		128,
		NULL,
		0
	);
	self->frame = 0;
	self->position = gfc_vector2d(0, 100);
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	self->think = jumperThink;
	self->update = jumperUpdate;
	self->free = jumperFree;

	

	return self;
}


Entity* jumper_new_pos(int x, int y)
{
	//SJson *json = sj_load("src/entity.cfg");
	Entity* self;
	self = entity_new();

	if (!self)
	{
		slog("Failed to create new jumper");
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
		"images/Sprites/StalactMight.png",
		64,
		64,
		NULL,
		0
	);
	self->frame = 0;
	self->position = gfc_vector2d(x, y);
	self->acceleration.y = .5;//gravity for now
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	self->think = jumperThink;
	self->update = jumperUpdate;
	self->free = jumperFree;
	self->team = ETT_enemy;
	self->cooldown = 0;
	self->directionX = 1;//walke foward first
	self->health = 1;
	return self;
}


void jumperThink(Entity* self)
{
	if (self->cooldown == 0) {
		self->velocity.y -= 20;
		self->cooldown = 100;
	}
	else self->cooldown--;
	
	entityPhysicsCalc(self);
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	world_tile_collide_active_entity(activeworld, self);
}
void jumperUpdate(Entity* self)
{

}
void jumperFree(Entity* self)
{
	if (!self)return;
	entity_free(self);
}