#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_text.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

typedef struct Entity_S {
	GFC_TextLine	name; /**<name of the entity for debuggin>*/
	Sprite			*sprite;/**<Graphical represenations*/
	float			frame;/**<for drawing*/
	GFC_Vector2D	position;/**<where to draw*/
	GFC_Vector2D	velocity;/**<how are we moving*/
	GFC_Vector2D	acceleration;/**/
	GFC_Vector2D	gravity;/*keepiing seperate from acceleration just in case I need both*/

	Uint8	inuse;// can make variables private 
	
	void (*think)(struct Entity_S* self);// For making decisions
	void (*update)(struct Entity_S* self);//execute decisions
	void (*free)(struct Entity_S* self);//Destroy Self
	void* data;//for ad hoc addition data for entity
				//Data is the example format for adding anything
	
}Entity;//basically typedef struct Entity


/*
* 
* @brief initialize sub entity_system_init
* @param  maxEnts uper limit for system entity existence
*/

void entity_system_init(Uint32 maxEnts);

/*
* @breif Draw all entitie
*/
void entity_system_draw();
/*
* @breif Draw an entitie
*/
void entityDraw(Entity* self);


/*
* @breif clear all entity
* @param iggnore - ignore this one clear rest
*/
void entity_free_all(Entity* ignore);

/*
* @breif clear an entity
* @param clear - what is being cleared
*/
void entity_free(Entity* self);

/*
*  @brief new blank entity
*/
Entity *entity_new();

/*
*  @brief all think for entity
*/
void entity_system_think();

/*
*  @brief all update for entity
*/
void entity_system_update();

/*@brief Incase I want to animate something */
void entityAnimate(Entity* self);
#endif