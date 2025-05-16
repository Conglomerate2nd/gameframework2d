#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_draw.h"
#include "runnawayer.h"
#include "player.h"
#include "world.h"

void runnawayerThink(Entity* self);
void runnawayerUpdate(Entity* self);
void runnawayerFree(Entity* self);


Entity* runnawayer_new()
{
	//SJson *json = sj_load("src/entity.cfg");
	Entity* self;
	self = entity_new();

	if (!self)
	{
		slog("Failed to create new runnawayer");
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
	self->think = runnawayerThink;
	self->update = runnawayerUpdate;
	self->free = runnawayerFree;

	//Entity* target = player_get();

	return self;
}


Entity* runnawayer_new_pos(int x, int y)
{
	Entity* target;
	//SJson *json = sj_load("src/entity.cfg");
	Entity* self;
	self = entity_new();

	if (!self)
	{
		slog("Failed to create new runnawayer");
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
		"images/Sprites/Swimmp.png",
		64,
		64,
		NULL,
		0
	);
	self->frame = 0;
	self->position = gfc_vector2d(x, y);
	self->acceleration.y = .5;//gravity for now
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	self->think = runnawayerThink;
	self->update = runnawayerUpdate;
	self->free = runnawayerFree;
	self->team = ETT_enemy;
	self->directionX = 1;//walke foward first
	self->center.x = self->position.x + 32;
	self->center.y = self->position.y + 32;
	self->sensor = gfc_circle(self->center.x, self->center.y, 200);
	target = entity_player_get();
	self->health = 1;
	return self;
}


void runnawayerThink(Entity* self)
{

	Entity* target;
	if (!self)return;
	target = entity_player_get();
	if (target)
	{
		//slog("runnawayer senor 1");
		//slog("%f target position.x",target->position.x);
		//pretty much the same as a point within radius of 384
		if (gfc_vector2d_distance_between_less_than(target->position, self->position, 384)) {
			//Player on right
			//slog("runnawayer senor");
			if (target->position.x > self->position.x) {
				self->velocity.x = -1.5;
			}
			//Player on left
			if (target->position.x < self->position.x) {
				self->velocity.x = 1.5;
			}
		}
		else self->velocity.x = 0;
	}

	gf2d_draw_circle(self->position, self->sensor.r, GFC_COLOR_BLUE);
	entityPhysicsCalc(self);
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	world_tile_collide_active_entity(activeworld, self);

}
void runnawayerUpdate(Entity* self)
{



	//slog("runnawayer THINK DONE");

}
void runnawayerFree(Entity* self)
{
	if (!self)return;
	entity_free(self);
}