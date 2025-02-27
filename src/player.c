#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_input.h"
#include "player.h"
#include "world.h"


void playerThink(Entity* self);
void playerUpdate(Entity* self);
void playerFree(Entity* self);
void playerPhysicsCalc(Entity* self);



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
	self->acceleration.y = .5;

	self->position = gfc_vector2d(64,64);
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	
	self->think = playerThink;
	self->update = playerUpdate;
	self->free = playerFree;



	return self;
}

void playerThink(Entity* self)
{
	GFC_Vector2D dir = { 0 };
	const Uint8* keys;
	keys = SDL_GetKeyboardState(NULL);
	if (!self)return;

	
	
	if (keys[SDL_SCANCODE_A]){
		//slog("here Left");
		self->velocity.x = -3;
	}
	else if (keys[SDL_SCANCODE_D]){
		//slog("here Right");
		self->velocity.x = 3;
	}
	else self->velocity.x = 0;

	if (keys[SDL_SCANCODE_SPACE]) {
		//slog("here Left");
		self->velocity.y = -.5;
	}
	else self->velocity.y += self->acceleration.y;


	
	/*
	if (keys[SDL_SCANCODE_S]){
		//slog("here Down");
		self->velocity.y += 3;
	}
	else if (keys[SDL_SCANCODE_W]){
		//slog("here Up");
		self->velocity.y = -3;
	}
	else self->velocity.y = 0;//eliminate this line for gravity
	*/

	/*  TEST FOR FOLLOW MOuse
	Sint32 mx = 0, my = 0;
	SDL_GetMouseState(&mx, &my);
	if (self->position.x < mx)dir.x = 1;
	if (self->position.y < my)dir.y = 1;
	if (self->position.x > mx)dir.x = -1;
	if (self->position.y > my)dir.y = -1;
	*/
	
	playerPhysicsCalc(self);

	gfc_vector2d_normalize(&dir);
	//kinda important but ruins gravity
	//gfc_vector2d_scale(self->velocity,dir,3);

	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	//slog("in player");
	world_tile_collide_active(activeworld, self->bounds);
	
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
	

	
}

void playerFree(Entity* self)
{
	if (!self)return;
	entity_free(self);
}

//Calculations for velocity gravity acceleration etc
void playerPhysicsCalc(Entity* self) {

	//time is not a variable because adding a variable each frame is the same as multiplying over time

	//position = position + velocity * time
	
	self->position.x += self->velocity.x;
	self->position.y += self->velocity.y;
	
	//gfc_vector2d_add(self->position, self->position, self->velocity);

	
	//velocity = velocity + acceleration  * time
	//velocity = velocity + (acceleration + gravity ) * time for y
	
	self->velocity.x += self->acceleration.x;
	self->velocity.y += self->acceleration.y + self->gravity;
	
	//for gravity 



	//self->acceleration.y = self->gravity;
	//gfc_vector2d_add(self->velocity, self->velocity, self->acceleration);

}