#ifndef __WORLD_H_
#define __WORLD_H_

#include "gf2d_sprite.h"
#include "gfc_shape.h"
#include "entity.h"
#include "SDL_mixer.h"

typedef struct
{
	Sprite		*background;//the background image - loaded as a sprite
	Sprite		*tileSet;//The full image containing all of the world's sprites
	Sprite		*tileLayer;//Drawn to surface
	Uint8		*tileMap;//The tiles that make up the world
	Uint32		 tileHeight;//how tall tiles are
	Uint32		 tileWidth;//how wide tiles are
	Uint32		worldHeight;//height of world
	Uint32		worldWidth;//width of world
	GFC_Rect 	*bounds;
	Mix_Music	*music;
	int			tileToDraw;
	int			tileSwitchCooldown;
}World;

/**
* @brief loads world from json file
* @return Null on error or a usable world.
*/
World* world_load(const char* filename);

//Test Function
World* world_test_new();

/*
* @brief allocate a new empty world
* @return Null on error or a blank world.
*/
World* world_new();

/*
* @brief free a previously allocated world
* @param worldthe world to free
*/
void world_free(World* world);

/*
* @brief draw world
* @param world the world to draw
*/
void world_draw(World* world);

/**
* @brief checks for collision across tiles
* @param world the world colliding against
* @param bounds the bounds checked
*/
void world_tile_collide(World* world, GFC_Rect bounds);

static World* activeworld = NULL;

/** 
* @brief checks for collision across tiles
* @param world checked against
*/
void world_draw_active(World* world);

/**
* @brief draws bounds to see
* @param world checked against
*/
void world_draw_bounds(World* world);

/**
* @brief sets up world camera
*/
void world_setup_camera(World* world);

/**
* @brief checks for collision across tiles
* @param world, while not necessarry, helps readability and efficiency
*/
void world_tile_collide_active(World* world, GFC_Rect bounds);


/**
* @brief checks for collision across tiles
* @param world, while not necessarry, helps readability and efficiency
*/
void world_tile_collide_active_entity(World* world, Entity* self);
/**
* @brief each function dictates what each tile under a number should do
* @param i and j are counting variables from the original function. these are the same for readability
*/

/**
* List as of now
*	tile_1 - player  spawn
*	tile_2 - solid block
* 
* Planned
* 
*	tile_2 pass through bottom left and right but solid top
*	tile_3 solid on right pass through left - one way door
*	tile_4 solid on left pass through right - one way door
*	
*	tile_5 hazard
*	
*	tile_6 spawn points
*	tile_7  player spawn
*	tile_8	walker/rocco spawn
*	tile_9	flyer/buggo spawn
*	tile_10 
*	tile_11
*	tile_12
*/
void tile_1(int i, int j, World* world, Entity* self);
void tile_2(int i, int j, World* world, Entity* self);
void tile_3(int i, int j, World* world, Entity* self);
void tile_4(int i, int j, World* world, Entity* self);
void tile_5(int i, int j, World* world, Entity* self);
void tile_6(int i, int j, World* world, Entity* self);
void tile_7(int i, int j, World* world, Entity* self);
void tile_8(int i, int j, World* world, Entity* self);
void tile_9(int i, int j, World* world, Entity* self);
void tile_10(int i, int j, World* world, Entity* self);
void door(int i, int j, World* world, Entity* self);

//void tile_3test(int i, int j, World* world, Entity* self);

/**
* @brief, all functions get tile next to position point.
*/
int get_tile_right(float x, float y);
int get_tile_left(float x, float y);
int get_tile_above(float x, float y);
int get_tile_below(float x, float y);
int getDistance(float x, float y);

//Checks

//We do not need the cordinates because we will use in tandem with get_tiles
//Syntax HollowTileCheck(get_tiles)
int HollowTileCheckRight(int i);

int HollowTileCheckLeft(int i);

int HollowTileCheckUp(int i);

int HollowTileCheckDown(int i);


void HighlightTile(float x, float y, World *world);
//Only calledby highligh tile
void DrawTile(float mx, float my, World* world,int index, int tile);

void EraseTile(float mx, float my, World* world, int index);

void world_save();
#endif