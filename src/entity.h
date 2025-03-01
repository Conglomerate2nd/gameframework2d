#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_text.h"
#include "gfc_vector.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"

typedef enum 
{
	ETT_none,
	ETT_player,
	ETT_enemy,
	ETT_item,
	ETT_world_object,
	ETT_MAX
}EntityTeamType;

typedef enum
{
	ECL_noLayer =1,
	ECL_worldLayer = 2,
	ECL_entityLayer = 4,
	ECL_itemLayer = 8,
	ECL_MAXLayer = 15
}EntityCollisionLayers;

typedef struct Entity_S {
	GFC_TextLine			name; /**<name of the entity for debuggin>*/
	Sprite					*sprite;/**<Graphical represenations*/
	float					frame;/**<for drawing*/
	GFC_Vector2D			position;/**<where to draw*/
	GFC_Vector2D			velocity;/**<how are we moving*/
	GFC_Vector2D			acceleration;/**/
	float					gravity;/*keepiing seperate from acceleration just in case I need both*/
	GFC_Vector2D			ceneter;/*Determines the center of the character		USE Bottom center for platformer, dead center for not that*/
	GFC_Rect				bounds;
	GFC_Vector2D			*pointOfContact;//for collision function
	GFC_Vector2D			*normal;
	GFC_Vector2D			offset;

	EntityTeamType			team;
	EntityCollisionLayers	layer;
	Uint8					inuse;// can make variables private 


	
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

/**
* @brief gets collision between two entities
* @returns 1 if true
*/
int entity_collision(Entity* self,  Entity* other);

/**
* @brief gets collision between two entities 
* @param poc will store the vector point of collision
* @param normal will send the normal of the vector
* @returns 1 if true
*/
int entity_collision_poc(Entity* self, GFC_Rect rect, GFC_Vector2D* poc, GFC_Vector2D* normal);

/**
* @brief gets collision between two entities
* @param poc will store the vector point of collision
* @param normal will send the normal of the vector
* @returns 1 if true
*/
int entity_obj_collision_poc(Entity* self, GFC_Rect rect, GFC_Vector2D* poc, GFC_Vector2D* normal);

/**
* @brief gets collision of all enities
* @returns 1 if true
*/
void entity_system_collision();
#endif