#include "simple_logger.h"
#include "simple_json.h"

#include "attack.h"
#include "world.h"

void attackThink(Entity* self);
void attackUpdate(Entity* self);
void attackFree(Entity* self);
void attackPhysicsCalc(Entity* self);



Entity* attack_new_pos1(int x, int y, int type, int direction)
{
	//SJson *json = sj_load("src/entity.cfg");
	Entity* self;
	self = entity_new();
	slog("shot");
	if (!self)
	{
		slog("Failed to create new attack");
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
		"images/Sprites/GobbloAttack1.png",
		64,
		64,
		NULL,
		0
	);
	self->frame = 0;
	self->position = gfc_vector2d(x, y);
	self->acceleration.y = .5;//gravity for now
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	self->think = attackThink;
	self->update = attackUpdate;
	self->free = attackFree;
	self->directionX = direction;//walke foward first
	self->team = ETT_weapon;
	self->type = type;
	
	switch (self->type) {
		case 0:self->ttl = 10; break;
		case 1:self->ttl = 100; break;
		case 2:self->ttl = 100; break;
		case 3:self->ttl = 100; break;
		case 4:self->ttl = 20; break;
		case 5:self->ttl = 10; break;
		default:self->ttl = 10; break;
	}
	return self;
}

Entity* attack_new_pos(int x, int y,int type,int ttl,int directionX)
{
	//SJson *json = sj_load("src/entity.cfg");
	Entity* self;
	self = entity_new();
	slog("shot");
	if (!self)
	{
		slog("Failed to create new attack");
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
		"images/Sprites/GobbloAttack1.png",
		64,
		64,
		NULL,
		0
	);
	self->frame = 0;
	self->position = gfc_vector2d(x, y);
	self->acceleration.y = .5;//gravity for now
	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	self->think = attackThink;
	self->update = attackUpdate;
	self->free = attackFree;
	self->team = ETT_weapon;
	self->directionX = directionX;//walke foward first
	//self->ttl == ttl;
	return self;
}



void attackThink(Entity* self)
{

	self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
	//world_tile_collide_active_entity(activeworld, self);
	
	
	switch (self->type) {
		case 0:attack0(self); break;
		case 1:attack1(self); break;
		case 2:attack2(self); break;
		case 3:attack3(self); break;
		case 4:attack4(self); break;
		case 5:attack5(self); break;
		default:attack0(self); break;
	}
	
	if (self->ttl > 0) { self->ttl--; }
	//slog("%i",self->position.x);

}
void attackUpdate(Entity* self)
{
	//slog(self->ttl);
	if (self->ttl == 0)entity_free(self);
}
void attackFree(Entity* self)
{
	if (!self)return;
	entity_free(self);
}

void attackPhysicsCalc(Entity* self) {

	//time is not a variable because adding a variable each frame is the same as multiplying over time

	//position = position + velocity * time
	//gfc_vector2d_add(self->position, self->position, self->velocity);


	//velocity = velocity + acceleration  * time
	//velocity = velocity + (acceleration + gravity ) * time for y

	//X AXIS
	if (get_tile_right(self->position.x, self->position.y) != 1 || get_tile_left(self->position.x + 62, self->position.y) != 1) {
		self->position.x += self->velocity.x;
		self->velocity.x += self->acceleration.x;
	}

	//YAXIS

	if (get_tile_below(self->position.x, self->position.y) != 0 || get_tile_above(self->position.x, self->position.y + 64) != 0) {
		self->position.y = self->position.y;
	}
	if (get_tile_below(self->position.x, self->position.y) == 0) {
		self->position.y += self->velocity.y;
		self->velocity.y += self->acceleration.y + self->gravity;
	}
	else self->velocity.y = 0;
}


void attack0(Entity* self)
{
	//stab
	self->position.x += 10 * self->directionX;
}
void attack1(Entity* self)
{
	attackPhysicsCalc(self);
	//bouncing arc
	self->position.x += 10 * self->directionX;
	self->position.y -= 10;
}
void attack2(Entity* self)
{
	//circle shot
	self->position.x = self->position.x + 64*cos(self->ttl);
	self->position.y = self->position.y + 64*sin(self->ttl);
	self->position.x += 5*self->directionX;
}
void attack3(Entity* self)
{
	//jittery plant
	self->position.x += 10 * cos(self->position.x);
	self->position.y -= 10 * sin(self->position.y);
}
void attack4(Entity* self)
{
	//farther shot - hollow knight dash attack
	self->position.x += 100;
}
void attack5(Entity* self) 
{
	//empty
}