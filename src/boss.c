#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_draw.h"
#include "boss.h"
#include "player.h"
#include "world.h"

void bossThink(Entity* self);
void bossUpdate(Entity* self);
void bossFree(Entity* self);


Entity* boss_new()
{
	//SJson *json = sj_load("src/entity.cfg");
	Entity* self;
	self = entity_new();

	if (!self)
	{
		slog("Failed to create new boss");
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
	self->think = bossThink;
	self->update = bossUpdate;
	self->free = bossFree;

	//Entity* target = player_get();

	return self;
}


Entity* boss_new_pos(int x, int y)
{
	Entity* target;
	//SJson *json = sj_load("src/entity.cfg");
	Entity* self;
	self = entity_new();

	if (!self)
	{
		slog("Failed to create new boss");
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
		"images/Sprites/MoleGrin.png",
		64,
		64,
		NULL,
		0
	);
	self->frame = 0;
	self->position = gfc_vector2d(x, y);
	self->acceleration.y = .5;//gravity for now
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	self->think = bossThink;
	self->update = bossUpdate;
	self->free = bossFree;
	self->team = ETT_enemy;
	self->directionX = 1;//walke foward first
	self->center.x = self->position.x + 32;
	self->center.y = self->position.y + 32;
	self->sensor = gfc_circle(self->center.x, self->center.y, 200);
	target = entity_player_get();
	return self;
}


void bossThink(Entity* self)
{

	Entity* target;
	if (!self)return;
	target = entity_player_get();
	if (target)
	{
		//slog("boss senor 1");
		//slog("%f target position.x",target->position.x);
		//pretty much the same as a point within radius of 384
		if (self->velocity.y == 0) 
		{		
			if (gfc_vector2d_distance_between_less_than(target->position, self->position, 384))
			{
				//Player on right
				//slog("boss senor");
				if (target->position.x > self->position.x) {
					self->velocity.x = 1.5;
					self->velocity.y = -1.5;
				}
				//Player on left
				if (target->position.x < self->position.x) {
					self->velocity.x = -1.5;
				}
			}
			else self->velocity.x = 0;
		}
	}

	gf2d_draw_circle(self->position, self->sensor.r, GFC_COLOR_BLUE);
	entityPhysicsCalc(self);
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	world_tile_collide_active_entity(activeworld, self);

}
void bossUpdate(Entity* self)
{



	//slog("boss THINK DONE");

}
void bossFree(Entity* self)
{
	if (!self)return;
	entity_free(self);
}


//Program Distant ontraint for multiple particles