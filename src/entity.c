#include "simple_logger.h"
#include "gf2d_draw.h"
#include "entity.h"
#include "world.h"

typedef struct {
	Entity*		entityList;
	Uint32		entityMax;
}EntityManager;

void entity_system_close();//called before declaration

static EntityManager _entManager = { 0 };//Initializes alocal variable

void entity_system_close()
{
	
	entity_free_all(NULL);
	if(_entManager.entityList)free(_entManager.entityList);
	memset(&_entManager, 0, sizeof(EntityManager));// & makes a pointer to address
}

void entity_system_init(Uint32 maxEnts)
{
	if (_entManager.entityList) 
	{
		slog("Cant have two lists at at time");
		return;
	}
	if (!maxEnts) 
	{
		slog("cannot allocat 0 entities");
		return;
	}
	_entManager.entityList = gfc_allocate_array(sizeof(Entity), maxEnts);

	if (!_entManager.entityList) 
	{
		slog("Failed to allocate global entiity list");
		return;
	}
	_entManager.entityMax = maxEnts;
	atexit(entity_system_close);
}

void entity_free_all(Entity* ignore) 
{
	int i;

	for (i = 0; i < _entManager.entityMax; i++) 
	{

		if (&_entManager.entityList[i] == ignore) continue;
		if (!_entManager.entityList[i].inuse) continue;// skip this iteraion
		//Put anything that needs to be erased here.
		entity_free(&_entManager.entityList[i]);
	}
}

void entity_free(Entity* self) 
{
	if (!self)return;//self is not exist
	gf2d_sprite_free(self->sprite);
	if (self->free) self->free(self->data);

}

Entity* entity_new()
{
	int i;

	for (i = 0; i < _entManager.entityMax; i++) 
	{
		if (_entManager.entityList[i].inuse) continue;// skip this iteraion
		memset(&_entManager.entityList[i], 0, sizeof(Entity));
		_entManager.entityList[i].inuse = 1;
		//setting the deault color. - if transparent invisible
		//setting default scale - if 0 too small
		return &_entManager.entityList[i];
	}
	slog("no more available entities");
	return NULL;
}

void entityThink(Entity* self) {
	int i;
	if (!self)return;
	if (self->think)self->think(self);

}

void entity_system_think() 
{
	int i;
	for (i = 0; i < _entManager.entityMax; i++)
	{
		if (!_entManager.entityList[i].inuse) continue;// skip this iteraion
		entityThink(&_entManager.entityList[i]);
	}
}

void entityUpdate(Entity* self) {
	if (!self)return;
	if (self->update)self->update(self);
}

void entity_system_update()
{
	int i;
	for (i = 0; i < _entManager.entityMax; i++)
	{
		if (!_entManager.entityList[i].inuse) continue;// skip this iteraion
		entityUpdate(&_entManager.entityList[i]);
	}
}


void entity_system_draw()
{
	int i;
	for (i = 0; i < _entManager.entityMax; i++)
	{
		if (!_entManager.entityList[i].inuse) continue;// skip this iteraion
		entityDraw(&_entManager.entityList[i]);
	}
}

void entityDraw(Entity* self)
{
	GFC_Vector2D drawAtOffset;
	if (!self)
	{
		return;
	}

	if (!self->sprite)return;

	gfc_vector2d_add(drawAtOffset, self->position, self->offset);

	gf2d_sprite_render(
		self->sprite,
		drawAtOffset,
		NULL,//scale
		NULL,//center
		NULL,//rotation
		NULL,//flip
		NULL,//color
		NULL,//clip
		self->frame
	);
		
	//BOUND TESTING DRAW RECTS
	gf2d_draw_rect(self->bounds,GFC_COLOR_BLUE);
}

void entityAnimate(Entity* self) {
	if (!self)return;
	self->frame += .1;
	if (self->frame >= 16)self->frame = 0;
}

int entity_layer_check(Entity* self, EntityCollisionLayers layer) {
	if (!self)return 0;
	return self->layer & layer;//binary math
}

void entity_remove_collision_layer(Entity* self, EntityCollisionLayers layer) {
	if (!self)return;
	self->layer &= ~layer;// ~not layer
}

void entity_set_layer(Entity* self, EntityCollisionLayers layer) {
	if (!self)return;
	self->layer |= layer;
}

 //Providd by professor
int entity_collision(Entity* self, Entity* other) 
{
	GFC_Rect bounds1, bounds2;

	if ((!self) || (!other)) return 0;
	if (self== other) return 0;
	//if ((self->team == other->team)) return 4;
	//if (!(self->layer == other->layer)) return;

	gfc_rect_copy(bounds1, self->bounds);
	gfc_rect_copy(bounds2, other->bounds);

	
	return gfc_rect_overlap(bounds1, bounds2);
}


int entity_collision_poc(Entity* self, Entity* other, GFC_Vector2D *poc, GFC_Vector2D *normal) 
{
	GFC_Rect bounds1, bounds2;
	

	if ((!self) || (!other)) return 0;
	if (self == other) return 0;
	if ((self->team == other->team)) return 0;
	//if (!(self->layer == other->layer)) return;

	gfc_rect_copy(bounds1, self->bounds);
	gfc_rect_copy(bounds2, other->bounds);

	return gfc_rect_overlap_poc(bounds1, bounds2, poc, normal);
}

int entity_obj_collision_poc(Entity* self,GFC_Rect rect, GFC_Vector2D* poc, GFC_Vector2D* normal) 
{
	GFC_Rect bounds1, bounds2;


	if ((!self)) return 0;
	//if (self == other) return 0;
	//if ((self->team == other->team)) return 0;
	//if (!(self->layer == other->layer)) return;

	gfc_rect_copy(bounds1, self->bounds);
	gfc_rect_copy(bounds2, rect);

	return gfc_rect_overlap_poc(bounds1, bounds2, poc, normal);
}

void entity_system_collision()
{
	int i,j;
	for (i = 0; i < _entManager.entityMax; i++)
	{
		if (!_entManager.entityList[i].inuse) continue;// skip this iteraion
		
		for (j = 0; j < _entManager.entityMax; j++)
		{
			if (!_entManager.entityList[j].inuse) continue;// skip this iteraion
			
			if (&_entManager.entityList[j] == &_entManager.entityList[i]) {
				//if the adress is the same, skip.
				//slog("hit the same");
				continue;
			}
			
			if (entity_collision(&_entManager.entityList[i], &_entManager.entityList[j]))
			{
				//slog("colliding");
				//THIS WORKS
			}

			entity_collision(&_entManager.entityList[i], &_entManager.entityList[j]);
		}
	}
}