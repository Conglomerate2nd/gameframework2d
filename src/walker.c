#include "simple_logger.h"
#include "simple_json.h"

#include "walker.h"

void walkerThink(Entity* self);
void walkerUpdate(Entity* self);
void walkerFree(Entity* self);

int sign = 1;

Entity* walker_new()
{
	//SJson *json = sj_load("src/entity.cfg");
	Entity* self;
	self = entity_new();

	if (!self)
	{
		slog("Failed to create new walker");
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
	self->position = gfc_vector2d(0,100);
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	self->think = walkerThink;
	self->update = walkerUpdate;
	self->free = walkerFree;



	return self;
}

void walkerThink(Entity* self)
{ 
	self->position.x += 5*sign;
	if (self->position.x <= 50) {
		//self->position.x += 5;
		sign = 1;
	}

	if (self->position.x >= 500) {
		//self->position.x -= 5;
		sign = -1;
	}

	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
}
void walkerUpdate(Entity* self)
{

}
void walkerFree(Entity* self)
{
	if (!self)return;
	entity_free(self);
}