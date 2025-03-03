#include "simple_logger.h"
#include "simple_json.h"

#include "flyer.h"

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
		"images/buggo.png",
		128,
		128,
		NULL,
		0
	);
	self->frame = 0;
	self->position = gfc_vector2d( 100,0);

	self->think = flyerThink;
	self->update = flyerUpdate;
	self->free = flyerFree;



	return self;
}

void flyerThink(Entity* self)
{

}
void flyerUpdate(Entity* self) 
{

}
void flyerFree(Entity* self) 
{
	if (!self)return;
	entity_free(self);
}

