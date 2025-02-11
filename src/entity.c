#include "simple_logger.h"
#include "entity.h"

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
	if (!self)
	{
		return;
	}

	if (!self->sprite)return;

	gf2d_sprite_render(
		self->sprite,
		self->position,
		NULL,//scale
		NULL,//center
		NULL,//rotation
		NULL,//flip
		NULL,//color
		NULL,//clip
		self->frame
	);
		


}

void entityAnimate(Entity* self) {
	if (!self)return;
	self->frame += .1;
	if (self->frame >= 16)self->frame = 0;
}