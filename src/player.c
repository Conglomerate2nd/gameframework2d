#include "simple_logger.h"
#include "player.h"


void playerThink(Entity* self);
void playerUpdate(Entity* self);
void playerFree(Entity* self);

Entity* player_new() 
{
	Entity *self;
	self = entity_new();
	if (!self) 
	{
		slog("Failed to create new player");
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
		"images/gobblo.png",
		128,
		128,
		NULL,
		0
	);
	self->frame = 0;
	self->position = gfc_vector2d(0,0);

	self->think = playerThink;
	self->update = playerUpdate;
	self->free = playerFree;

	return self;
}

void playerThink(Entity* self)
{
	GFC_Vector2D dir = { 0 };
	Sint32 mx = 0, my = 0;
	if (!self)return;

	SDL_GetMouseState(&mx, &my);
	if (self->position.x < mx)dir.x = 1;
	if (self->position.y < my)dir.y = 1;
	if (self->position.x > mx)dir.x = -1;
	if (self->position.y > my)dir.y = -1;

	gfc_vector2d_normalize(&dir);
	gfc_vector2d_scale(self->velocity,dir,3);
}
void playerUpdate(Entity* self)
{
	/*
	* Some of my sprites are a single frame
	* 
	if (!self)return;
	self->frame += .1;
	if (self->frame >= 16)self->frame = 0;
	*/

	if(!self)return;
	//entityAnimate(self);//Uncomment to animate
	gfc_vector2d_add(self->position, self->position, self->velocity);
}
void playerFree(Entity* self)
{
	if (!self)return;
}