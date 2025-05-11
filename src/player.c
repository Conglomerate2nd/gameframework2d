#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_input.h"
#include "player.h"
#include "world.h"
#include "camera.h"


void playerThink(Entity* self);
void playerUpdate(Entity* self);
void playerFree(Entity* self);
void playerPhysicsCalc(Entity* self);
void playerDamageSelf(Entity* self);


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
		"images/Sprites/gobblo.png",
		64,
		64,
		NULL,
		0
	);
	self->frame = 0;
	self->acceleration.y = .5;
	self->position = gfc_vector2d(128, 128);

	//setting the  offset by 64 seems to work well with the bounds, but needs more testing as sprite is 128*128 pixels
	self->offset.y = 0;
	self->offset.x = 0;
	
	self->bounds = gfc_rect(self->position.x+self->offset.x, self->position.y+self->offset.y, 64, 64);
	
	self->center.x = self->position.x + 32;
	self->center.y = self->position.y + 32;
	self->think = playerThink;
	self->update = playerUpdate;
	self->free = playerFree;
	self->damage = playerDamageSelf;
	self->team = ETT_player;
	self->directionY = 1;

	return self;
}

Entity* player_new_pos(int x,int y)
{

	Entity* self;
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
		"images/Sprites/gobblo.png",
		64,
		64,
		NULL,
		0
	);
	self->frame = 0;
	self->acceleration.y = .5;
	slog("%i x,%i y player spawn", x, y);
	self->position = gfc_vector2d(x, y);

	//setting the  offset by 64 seems to work well with the bounds, but needs more testing as sprite is 128*128 pixels
	self->offset.y = 0;
	self->offset.x = 0;

	self->bounds = gfc_rect(self->position.x + self->offset.x, self->position.y + self->offset.y, 64, 64);

	self->directionY = 1;
	self->think = playerThink;
	self->update = playerUpdate;
	self->free = playerFree;
	self->health = 5;
	self->team = ETT_player;
	//gfc_input_init("input.cfg");
	
	return self;
}


void playerThink(Entity* self)
{
	GFC_Vector2D dir = { 0 };
	const Uint8* keys;
	keys = SDL_GetKeyboardState(NULL);
	if (!self)return;

	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	//slog("in player");
	world_tile_collide_active_entity(activeworld, self);

	

	gfc_input_update();


	/*OLD AND BUGGY WITH NO CHECKS*/
	/*
	if (gfc_input_command_down("left")) {
		 self->velocity.x = -3;
	}
	else if (gfc_input_command_down("right")) {
		self->velocity.x = 3;
	}
	else self->velocity.x = 0;
	*/


	//**TESTING ATTENTIION PLEASE**//

	//Why +60? - TO CREATE a pixel gap between the left collision, avoiding edge cases, 63 and 64 have edge cases.
	//Using %2 to get left tile allows one way there to work, but using an
	if (gfc_input_command_down("left") && HollowTileCheckLeft(get_tile_left(self->position.x+60,self->position.y)) != 1){
		self->velocity.x = -3;
		//self->position.x -= 15;
		self->directionX = -1;
	} 
	else if (gfc_input_command_down("right") && HollowTileCheckRight(get_tile_right(self->position.x, self->position.y)) != 1) {
		 self->velocity.x = 3;
		//self->position.x += 15;
		 self->directionX = 1;
	}
	else self->velocity.x = 0;
	

	//Working dash with direction
	if (gfc_input_command_down("dash")&&self->cooldownDash==0) 
	{
		if (self->directionX == 1 && HollowTileCheckRight(get_tile_right(self->position.x, self->position.y) != 1))
		{
			if ((HollowTileCheckRight(get_tile_right(self->position.x + 192, self->position.y)) != 1)&&(HollowTileCheckRight(get_tile_right(self->position.x + 128, self->position.y)) != 1) && (HollowTileCheckRight(get_tile_right(self->position.x + 64, self->position.y)) != 1)) {
				self->position.x += 190;
			}
			if ((HollowTileCheckRight(get_tile_right(self->position.x + 128, self->position.y)) != 1) && (HollowTileCheckRight(get_tile_right(self->position.x + 64, self->position.y)) != 1)) {
				self->position.x += 128;
			}
			else if (HollowTileCheckRight(get_tile_right(self->position.x + 64, self->position.y)) != 1) {
				self->position.x += 64;

			}
		}

		if (self->directionX == -1 && HollowTileCheckLeft(get_tile_left(self->position.x, self->position.y) != 1))
		{
			if ((HollowTileCheckRight(get_tile_left(self->position.x - 192, self->position.y)) != 1) && (HollowTileCheckRight(get_tile_left(self->position.x - 128, self->position.y)) != 1) && (HollowTileCheckLeft(get_tile_left(self->position.x - 64, self->position.y)) != 1)) {
				self->position.x -= 190;
			}
			if ((HollowTileCheckRight(get_tile_left(self->position.x - 128, self->position.y)) != 1) && (HollowTileCheckLeft(get_tile_left(self->position.x - 64, self->position.y)) != 1)) {
				self->position.x -= 128;
			}
			else if (HollowTileCheckLeft(get_tile_left(self->position.x - 64, self->position.y)) != 1) {
				self->position.x -= 64;

			}
		}
		self->cooldownDash = 32;
		//else self->position.x += 64;

		//self->position.x += 15;
	}

	/*
	if (gfc_input_command_down("right") && gfc_input_command_down("dash") && HollowTileCheckRight(get_tile_right(self->position.x, self->position.y)) != 1) {
		if ((HollowTileCheckRight(get_tile_right(self->position.x + 128, self->position.y)) != 1) && (HollowTileCheckRight(get_tile_right(self->position.x + 64, self->position.y)) != 1)) {
			self->position.x += 120;
		}
		else if (HollowTileCheckRight(get_tile_right(self->position.x + 64, self->position.y)) != 1) {
			self->position.x += 64;

		}
		//else self->position.x += 64;

		//self->position.x += 15;
	}*/

	//DASH
	/*
	if (gfc_input_command_down("left") && HollowTileCheckLeft(get_tile_left(self->position.x + 60, self->position.y)) != 1) {
		self->velocity.x = -3;
		//self->position.x -= 15;
	}
	else if (gfc_input_command_down("right") && HollowTileCheckRight(get_tile_right(self->position.x, self->position.y)) != 1) {
		self->velocity.x = 3;
		//self->position.x += 15;
	}
	else self->velocity.x = 0;
	*/

	if (gfc_input_command_down("jump")) {

		//slog("%f velocity y", self->velocity.y);
		//self->position.y += -15*self->directionY;
		if (get_tile_above(self->position.x, self->position.y + 62) == 0)
		{
			self->position.y += -15;
		}
		
	}
	else self->velocity.y = self->velocity.y;
	
	//TODO: DASH - get distance between tile in front and if it is solid, add the distance to X, else add set distance


	playerPhysicsCalc(self);

	gfc_vector2d_normalize(&dir);
	//kinda important but ruins gravity
	//gfc_vector2d_scale(self->velocity,dir,3);

	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	//slog("in player");
	world_tile_collide_active_entity(activeworld,self);
	//world_tile_collide_active(activeworld, self->bounds);
	//BOUND TESTING DRAW RECTS
	
	if (self->cooldownDash > 0) {
		self->cooldownDash--;
	}
}
void playerUpdate(Entity* self)
{
	if (self->health == 0) {
		slog("dead");
		playerFree(self);

	}

	/*
	if (self->directionX == -1) {

		gf2d_sprite_draw(
			self->sprite,
			self->position,
			NULL,
			&self->center,
			0,
			-1,
			NULL,
			NULL);
	}
	*/

	/*
	* Some of my sprites are a single frame
	* 
	if (!self)return;
	self->frame += .1;
	if (self->frame >= 16)self->frame = 0;
	*/

	if(!self)return;
	//entityAnimate(self);//Uncomment to animate
	
	/*
	for (int i = 0; i < player->health; i++) {
		slog("in here");
		GFC_Rect healthBar = gfc_rect(128 + (i * 64), 128, 64, 64);

		gf2d_draw_shape(gfc_shape_from_rect(healthBar), GFC_COLOR_RED, gfc_vector2d(0, 0));
	}
	*/

	camera_ceneter_at(self->position);
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
	//gfc_vector2d_add(self->position, self->position, self->velocity);


	//velocity = velocity + acceleration  * time
	//velocity = velocity + (acceleration + gravity ) * time for y
	
	//X AXIS
	if (get_tile_right(self->position.x, self->position.y) != 1 || get_tile_left(self->position.x+62, self->position.y) != 1) {
		self->position.x += self->velocity.x;
		self->velocity.x += self->acceleration.x;
	}

	//YAXIS

	if (get_tile_below(self->position.x, self->position.y) != 0 || get_tile_above(self->position.x, self->position.y+64) != 0) {
		self->position.y = self->position.y;
	}
	if (get_tile_below(self->position.x, self->position.y) == 0) {
		self->position.y += self->velocity.y;
		self->velocity.y += self->acceleration.y + self->gravity;
	}
	else self->velocity.y = 0;



	/*
	int x = (int)(self->position.x/64);
	int y = (int)(self->position.y+64);

	//ESSENTIALLY self->position.x and divide by 64 to get x position in array
	slog("did it work");
	if (activeworld->tileMap[x + y] == 0|| activeworld->tileMap[x + y + y]==NULL) {
		self->velocity.y = -10;
	}
	slog(" it did");
	//for gravity 
	*/


	//self->acceleration.y = self->gravity;
	//gfc_vector2d_add(self->velocity, self->velocity, self->acceleration);

	//IF TILE(ABOVE) is 1 push down if 2 dont

}

void playerDamageSelf(Entity* self) {
	//slog("damage");
	if (self->healthTickCount == 0)
	{
		self->health--;
		self->healthTickCount = 2000;
	}
	else self->healthTickCount--;
	
}

int isJumpValid(Entity* self) {
	return self->cooldown;
}

void jumpIsFalse(Entity* self) {
	self->cooldown = 1;
}
void jumpIsTrue(Entity* self) {
	 self->cooldown = 0;
}


