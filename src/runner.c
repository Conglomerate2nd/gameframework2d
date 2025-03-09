#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_draw.h"
#include "runner.h"
#include "player.h"
#include "world.h"

void runnerThink(Entity* self);
void runnerUpdate(Entity* self);
void runnerFree(Entity* self);

Entity* target; 

Entity* runner_new()
{
	//SJson *json = sj_load("src/entity.cfg");
	Entity* self;
	self = entity_new();

	if (!self)
	{
		slog("Failed to create new runner");
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
	self->think = runnerThink;
	self->update = runnerUpdate;
	self->free = runnerFree;
	
	Entity* target = player_get();

	return self;
}


Entity* runner_new_pos(int x, int y)
{
	//SJson *json = sj_load("src/entity.cfg");
	Entity* self;
	self = entity_new();

	if (!self)
	{
		slog("Failed to create new runner");
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
		"images/Sprites/Spikeen.png",
		64,
		64,
		NULL,
		0
	);
	self->frame = 0;
	self->position = gfc_vector2d(x, y);
	self->acceleration.y = .5;//gravity for now
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	self->think = runnerThink;
	self->update = runnerUpdate;
	self->free = runnerFree;
	self->team = ETT_enemy;
	self->directionX = 1;//walke foward first
	self->center.x = self->position.x - 64;
	self->center.y = self->position.y / 2;
	self->sensor = gfc_circle(self->center.x, self->center.y, 64);
	target = player_get();
	return self;
}


void runnerThink(Entity* self)
{
	//will not go through code until player is initialized 
		//slog("RUNNER THINK");

	if (player != NULL)
	{
		slog("RUNNER senor 1");
		if (gfc_point_in_cicle(target->position, self->sensor)) {
			//Player on right
			slog("RUNNER senor");
			if (target->position.x > self->position.x) {
				self->velocity.x = 3;
			}
			//Player on left
			if (target->position.x < self->position.x) {
				self->velocity.x = -3;
			}
		}
	}
		gf2d_draw_circle(self->position, self->sensor.r, GFC_COLOR_BLUE);
		entityPhysicsCalc(self);
		self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
		world_tile_collide_active_entity(activeworld, self);
	
}
void runnerUpdate(Entity* self)
{
	
	
	
	//slog("RUNNER THINK DONE");
	
}
void runnerFree(Entity* self)
{
	if (!self)return;
	entity_free(self);
}