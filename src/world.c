#include "simple_logger.h"
#include "simple_json.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "gfc_shape.h"

#include "entity.h"
#include "player.h"
#include "walker.h"
#include "flyer.h"
#include "runner.h"
#include "jumper.h"
#include "runnawayer.h"
#include "boss.h"
#include "world.h"
#include "camera.h"



typedef struct {
	GFC_Rect bounds[];
}worldData;

void world_tile_layer_build(World* world) {
	int i, j;
	Uint32 frame, index;
	GFC_Vector2D position;

	if (!world) return;
	if (!world->tileSet) return;
	if (world->tileLayer) {
		gf2d_sprite_free(world->tileLayer);
	}


	world->tileLayer = gf2d_sprite_new();
	world->tileLayer->surface = gf2d_graphics_create_surface(
		world->tileWidth * world->tileSet->frame_w,
		world->tileHeight * world->tileSet->frame_h
	);

	world->tileLayer->frame_w = world->tileWidth * world->tileSet->frame_w;
	world->tileLayer->frame_h = world->tileHeight * world->tileSet->frame_h;

	if (!world->tileLayer->surface) {
		slog("failed to create tileLayer surface");
		return;
	}


	for (j = 0; j < world->tileHeight; j++)
	{
		for (i = 0; i < world->tileWidth; i++)
		{
			index = i + (j * world->tileWidth);//converts position on tilemap into position in array
			if (world->tileMap[index] == 0) continue;//skip this cause its empty

			position.x = i * world->tileSet->frame_w;
			position.y = j * world->tileSet->frame_h;

			frame = world->tileMap[index] - 1;

			gf2d_sprite_draw_to_surface(
				world->tileSet,
				position,
				NULL,
				NULL,
				frame,
				world->tileLayer->surface
			);
			//gf2d_draw_rect(gfc_rect(i * 64, j * 64, 64, 64), GFC_COLOR_YELLOW);

		}
	}


	world->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), world->tileLayer->surface);
	if (!world->tileLayer->texture)
	{
		slog("failed to convert world tile layer to textue");
		return;
	}

}

World* world_load(const char* filename)
{
	World* world = NULL;
	SJson* json = NULL;
	SJson* worldJson = NULL;
	SJson* verticalArray, * horizontalArray;
	Uint32 width = 0, height = 0;
	int i, j;
	int tile;
	SJson* object;
	const char* tileSet;
	int frame_w, frame_h;
	int frames_per_line;


	if (!filename)
	{
		slog("No filename provided for world_load");
		return NULL;
	}

	//Loading json files and checking for errors
	json = sj_load(filename);
	if (!json) {
		slog("failed to load world file %s", filename);
		return NULL;
	}
	worldJson = sj_object_get_value(json, "world");
	if (!worldJson) {
		slog("'world' object missing in %s", filename);
		sj_free(json);
		return NULL;
	}

	//Loading arrays in tle map as vertial arrays and horizontal arrays
	verticalArray = sj_object_get_value(worldJson, "tileMap");
	if (!verticalArray) {
		slog("'tileMap' object missing in %s", filename);
		sj_free(json);
		return NULL;
	}

	height = sj_array_get_count(verticalArray);
	horizontalArray = sj_array_get_nth(verticalArray, 0);
	width = sj_array_get_count(horizontalArray);

	//test for bounds.


	//slog(width, height);
	slog("%i wide", width);
	slog("%i high", height);
	world = world_new(width, height);
	//world->tileMap = gfc_allocate_array(sizeof(Uint8), height * width);
	if (!world) {
		slog("failed to create space for a new world for file %s", filename);
		sj_free(json);
		return NULL;
	}

	for (j = 0; j < height; j++)
	{
		horizontalArray = sj_array_get_nth(verticalArray, j);
		if (!horizontalArray)continue; //might not be necessarry or might be better to error over but skip for now.
		for (i = 0; i < width; i++)
		{

			object = sj_array_get_nth(horizontalArray, i);
			if (!object)continue;

			tile = 0;
			sj_get_integer_value(object, &tile);//this sets the value of object and allocates it to tile's address, effectively setting it to object
			world->tileMap[i + (j * width)] = tile;

			//spawn statements
			//Dont know why when multiplying by tileWidth or tileHeight will leaadd to spawning in the wrong place, but haard coding works fine
			

			if (world->tileMap[i + (j * width)] == 17) {
				player_new_pos(i * 64, j * 64);
			}
			if (world->tileMap[i + (j * width)] == 18) {
				walker_new_pos(i * 64, j * 64);
			}
			if (world->tileMap[i + (j * width)] == 19) {
				jumper_new_pos(i * 64, j * 64);
			}
			if (world->tileMap[i + (j * width)] == 20) {
				flyer_new_pos(i * 64, j * 64);
			}
			if (world->tileMap[i + (j * width)] == 21) {
				runner_new_pos(i * 64, j * 64);
			}
			if (world->tileMap[i + (j * width)] == 22) {
				runnawayer_new_pos(i * 64, j * 64);
			}
			if (world->tileMap[i + (j * width)] == 23) {
				boss_new_pos(i * 64, j * 64);
			}
		}

	}

	//slog("loop failed");
	tileSet = sj_object_get_value_as_string(worldJson, "tileSet");
	if (!tileSet) {
		slog("'tileset' object missing in %s", filename);
		sj_free(json);
		return NULL;
	}
	sj_object_get_value_as_int(worldJson, "frame_w", &frame_w);
	sj_object_get_value_as_int(worldJson, "frame_h", &frame_h);
	sj_object_get_value_as_int(worldJson, "frames_per_line", &frames_per_line);

	world->background = gf2d_sprite_load_image(sj_object_get_value_as_string(worldJson, "background"));


	slog("%i frame wide", frame_w);
	slog("%i frame high", frame_h);
	slog("%i frames per line", frames_per_line);
	world->tileSet = gf2d_sprite_load_all(
		tileSet,
		frame_w,
		frame_h,
		frames_per_line,
		1
	);
	world_tile_layer_build(world);

	slog("assigning to activeworld");
	activeworld = world;

	slog("assigned");
	sj_free(json);
	return world;
}

World* world_test_new()
{
	int i, j;
	int width, height;
	width = 65;
	height = 45;
	World* world;
	world = world_new(width, height);
	if (!world) {
		slog("failed to make a new world ");
		return NULL;
	}

	world->background = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
	world->tileSet = gf2d_sprite_load_all(
		"images/tileSet1.png",
		16,
		16,
		1,
		1
	);
	for (i = 0; i < width; i++)
	{
		//slog(" at width");
		world->tileMap[i] = 1;
		world->tileMap[i + ((height - 1) * width)] = 1;
		//slog("Segment at width");
	}
	//i = 0;
	for (i = 0; i < height; i++)
	{
		//slog(" at height");
		world->tileMap[i * width] = 1;
		world->tileMap[i * width + (width - 1)] = 1;
		//slog("Segment at height");
	}
	world_tile_layer_build(world);
	return world;
}

World* world_new(Uint32 width, Uint32 height)
{
	World* world;
	if ((!width) || (!height) || (width <= 0) || (height <= 0)) {
		slog("cannot make a world with no width or height");
		return NULL;
	}

	//slog("failed to allocate a new world into array");
	world = gfc_allocate_array(sizeof(World), 1);//each one is the ize of world. we only want 1 so 1 is second paaram right now
	if (!world) {
		slog("failed to allocate a new world into array");
		return NULL;
	}
	//all defaults would go here
	world->tileHeight = height;
	world->tileWidth = width;
	world->tileMap = gfc_allocate_array(sizeof(Uint8), height * width);

	return world;
}

void world_free(World* world)
{
	if (!world) {
		slog("no world to free");
		return;
	}

	gf2d_sprite_free(world->background);
	gf2d_sprite_free(world->tileSet);
	gf2d_sprite_free(world->tileLayer);
	free(world->tileMap);
	free(world);
}

void world_draw(World* world)
{

	if (!world) {
		slog("no world to draw");
		return;
	}


	GFC_Vector2D position;
	//slog("It was poition");
	position.x = 0;
	position.y = 0;

	//originally the draw imag function had  vector2D(0,0), that was giving me an error, so I initialized position to 0,0 and am using it once
	//to call draw at 0,0 and then for the for loops which will override the value.

	gf2d_sprite_draw_image(world->background, position);

	gf2d_sprite_draw_image(world->tileLayer, position);

}



void world_draw_active(World* world)
{


	if (!activeworld) {
		slog("no active world to draw");
		return;
	}


	GFC_Vector2D position;
	GFC_Vector2D offset;

	offset = camera_get_offset();
	//slog("It was poition");
	position.x = 0;
	position.y = 0;


	//originally the draw imag function had  vector2D(0,0), that was giving me an error, so I initialized position to 0,0 and am using it once
	//to call draw at 0,0 and then for the for loops which will override the value.

	gf2d_sprite_draw_image(activeworld->background, position);

	gf2d_sprite_draw_image(activeworld->tileLayer, offset);


	int i, j, index;
	GFC_Rect bounds;
	//NOTE refering to world crashes, but using activeworld works.

	/*
	if (self->bounds==NULL) {
		return;
	}
	*/
	for (j = 0; j < activeworld->tileHeight; j++)
	{
		for (i = 0; i < activeworld->tileWidth; i++)
		{

			index = i + (j * activeworld->tileWidth);//converts position on tilemap into position in array
			if (activeworld->tileMap[index] == 0)continue;
			bounds = gfc_rect(i * 64+offset.x, j * 64+offset.y, 64, 64);
			gf2d_draw_rect(bounds, GFC_COLOR_BLUE);
			/*
			*/
		}
	}


}



void world_setup_camera(World* world)
{
	if (!world)return;
	if ((!world->tileLayer) || (!world->tileLayer->surface))
	{
		slog("no tile layer set for world");
		return;
	}
	camera_set_bounds(gfc_rect(0, 0, world->tileLayer->surface->w, world->tileLayer->surface->h));
	camera_apply_bounds();
	camera_enable_binding(1);

}

void world_tile_collide(World* world, GFC_Rect bounds) {
	int i, j, index;
	//NOTE refering to world crashes, but using activeworld works.
	for (j = 0; j < world->tileHeight; j++)
	{
		for (i = 0; i < world->tileWidth; i++)
		{

			index = i + (j * world->tileWidth);//converts position on tilemap into position in array
			if (world->tileMap[index] == 0) continue;//skip this cause its empty

			if (world->tileMap[index] == 1)
			{
				//gfc_rect(i * 64, j * 64, 64, 64);
				if (gfc_rect_overlap(bounds, gfc_rect(i * 64, j * 64, 64, 64))) {
					slog("hit");
				}
			}
			//bound test begin
			/*
			if (tile == 1) {
				world->bounds[i + (j * width)] = gfc_rect(i * 64, j * 64, 64, 64);

			}
			*/
			//bound test end
		}

	}
}

void world_tile_collide_active(World* world, GFC_Rect bounds)
{
	int i, j, index;
	//NOTE refering to world crashes, but using activeworld works.
	for (j = 0; j < activeworld->tileHeight; j++)
	{
		for (i = 0; i < activeworld->tileWidth; i++)
		{

			index = i + (j * activeworld->tileWidth);//converts position on tilemap into position in array
			if (activeworld->tileMap[index] == 0) continue;//skip this cause its empty

			if (activeworld->tileMap[index] == 1)
			{
				//gfc_rect(i * 64, j * 64, 64, 64);
				if (gfc_rect_overlap(bounds, gfc_rect(i * 64, j * 64, 64, 64))) {
					slog("hit");
				}
			}
			//bound test begin
			/*
			if (tile == 1) {
				world->bounds[i + (j * width)] = gfc_rect(i * 64, j * 64, 64, 64);

			}
			*/
			//bound test end
		}

	}
}


void world_tile_collide_active_entity(World* world, Entity* self)
{
	int i, j, index;
	GFC_Rect bounds;
	//NOTE refering to world crashes, but using activeworld works.

	/*
	if (self->bounds==NULL) {
		return;
	}
	*/
	for (j = 0; j < activeworld->tileHeight; j++)
	{
		for (i = 0; i < activeworld->tileWidth; i++)
		{
			
			index = i + (j * activeworld->tileWidth);//converts position on tilemap into position in array
			bounds = gfc_rect(i * 64, j * 64, 64, 64);
			/*
			if (activeworld->tileMap[index] == 0) continue;//skip this cause its empty

			if (activeworld->tileMap[index] == 1)
			{
				//gfc_rect(i * 64, j * 64, 64, 64);

				//if (gfc_rect_overlap_poc(self->bounds, gfc_rect(i * 64, j * 64, 64, 64), self->pointOfContact, self->normal)) {
				if (gfc_rect_overlap(self->bounds, gfc_rect(i * 64, j * 64, 64, 64))) {
					tile_1(i, j, world, self);
				}


			}

			if (activeworld->tileMap[index] == 2)
			{
				//gfc_rect(i * 64, j * 64, 64, 64);

				//if (gfc_rect_overlap_poc(self->bounds, gfc_rect(i * 64, j * 64, 64, 64), self->pointOfContact, self->normal)) {
				if (gfc_rect_overlap(self->bounds, gfc_rect(i * 64, j * 64, 64, 64))) {
					tile_2(i, j, world, self);
				}

			}
			*/
			if (gfc_rect_overlap(self->bounds,bounds)) {
				switch (activeworld->tileMap[index]) {
					case 0: break;
					case 1:tile_1(i, j, world, self); break;
					case 2:tile_2(i, j, world, self); break;
					case 3:tile_3(i, j, world, self); break;
					case 4:tile_4(i, j, world, self); break;
					case 5:tile_5(i, j, world, self); break;
					case 6:tile_6(i, j, world, self); break;
					case 7:tile_7(i, j, world, self); break;
					case 8:tile_8(i, j, world, self); break;
					case 9:tile_9(i, j, world, self); break;
					case 10:tile_10(i, j, world, self); break;
					default:break;
				}
			}
			//bound test begin
			/*
			if (tile == 1) {
				world->bounds[i + (j * width)] = gfc_rect(i * 64, j * 64, 64, 64);

			}
			*/
			//bound test end
		}

	}
}
//try subtracting vectors

void tile_1(int i, int j, World* world, Entity* self)
{

	//Not working for full collision, perfect for one way tiles  for top or left modidfied
	if (get_tile_left(i * 64, j * 64) != 1)
	{
		if ((self->bounds.x < i * 64) && (!((self->bounds.y + self->bounds.h > j * 64 + 64) || (self->bounds.y < j * 64))))
		{
			//slog("on left");

			//slog("%f x", i * 64);
			//self->position.x = i * 64+64;
			self->velocity.x = 0;
			self->position.x = i * 64 - 64;
			if (self->team == ETT_enemy)
			{
				self->directionX *= -1;
			}
			return;//NOTE: I want the collision to check for one then pass, because we can't be colliding on multiple sidesof the same tile,
			//if the player is colliding on the side, then they are definitely colliding with the top or bottommm

		}
	}

	if (get_tile_right(i * 64, j * 64) != 1)
	{
		if ((self->bounds.x + self->bounds.w > i * 64) && (!((self->bounds.y + self->bounds.h > j * 64 + 64) || (self->bounds.y < j * 64))))
		{
			//slog("on right")
			//slog("%f x", i * 64);
			//self->position.x = i * 64;
			self->velocity.x = 0;
			self->position.x = i * 64 + 64;
			if (self->team == ETT_enemy)
			{
				self->directionX *= -1;//IF enemy flip their direction of movement
			}
			return;//NOTE: I want the collision to check for one then pass, because we can't be colliding on multiple sidesof the same tile,
			//if the player is colliding on the side, then they are definitely colliding with the top or bottommm

		}
	}
	//if at wall of tile, and not ontop or below same tile
	//if that and is not there it  will push the player - use for treadmill tiles


	if (get_tile_above(i*64,j*64) != 1)
	{
		if ((self->bounds.y < j * 64))
		{
			//self->position.y = j * 64;
			//slog("on top");

			self->velocity.y = 0;
			self->position.y = (j - 1) * 64;//What this does is set the y position of self to the tile bottom of the tile above j
			if (self->isFlying == 1)
			{
				self->directionY *= -1;//IF enemy flip their direction of movement
			}
			return;
		}
	}

	if (get_tile_below(i * 64, j * 64) != 1)
	{
		if ((self->bounds.y + self->bounds.h > j * 64 + 64))
		{
			//slog("on bottom");
			//self->position.y = j * 64 + 64;
			self->velocity.y = 0;
			//Move to +1 below the tile so player does not stick and will fall affected by gravity
			self->position.y = j * 64 + 65;//What this does is set the y position of self to the tile bottom of the tile above j
			if (self->isFlying == 1)
			{
				self->directionY *= -1;//IF enemy flip their direction of movement
			}
			return;
		}
	}

}



void tile_2(int i, int j, World* world, Entity* self)
{

	//Not working for full collision, perfect for one way tiles  for top or left modidfied

	if ((self->bounds.x < i * 64) && (!((self->bounds.y + self->bounds.h > j * 64 + 64) || (self->bounds.y < j * 64))))
	{
		//slog("on left");

		//slog("%f x", i * 64);
		//self->position.x = i * 64+64;
		self->velocity.x = 0;
		self->position.x = i * 64 - 64;
		if (self->team == ETT_enemy)
		{
			self->directionX *= -1;
		}
		return;//NOTE: I want the collision to check for one then pass, because we can't be colliding on multiple sidesof the same tile,
		//if the player is colliding on the side, then they are definitely colliding with the top or bottommm

	}

	if ((self->bounds.x + self->bounds.w > i * 64) && (!((self->bounds.y + self->bounds.h > j * 64 + 64) || (self->bounds.y < j * 64))))
	{
		//slog("on right");

		//slog("%f x", i * 64);
		//self->position.x = i * 64;
		self->velocity.x = 0;
		self->position.x = i * 64 + 64;
		if (self->team == ETT_enemy)
		{
			self->directionX *= -1;//IF enemy flip their direction of movement
		}
		return;//NOTE: I want the collision to check for one then pass, because we can't be colliding on multiple sidesof the same tile,
		//if the player is colliding on the side, then they are definitely colliding with the top or bottommm

	}
	//if at wall of tile, and not ontop or below same tile
	//if that and is not there it  will push the player - use for treadmill tiles



	if ((self->bounds.y < j * 64))
	{
		//self->position.y = j * 64;
		//slog("on top");

		self->velocity.y = 0;
		self->position.y = (j - 1) * 64;//What this does is set the y position of self to the tile bottom of the tile above j
		if (self->isFlying == 1)
		{
			self->directionY *= -1;//IF enemy flip their direction of movement
		}
		return;
	}

	if ((self->bounds.y + self->bounds.h > j * 64 + 64))
	{
		//slog("on bottom");
		//self->position.y = j * 64 + 64;
		self->velocity.y = 0;
		//Move to +1 below the tile so player does not stick and will fall affected by gravity
		self->position.y = j * 64 + 65;//What this does is set the y position of self to the tile bottom of the tile above j
		if (self->isFlying == 1)
		{
			self->directionY *= -1;//IF enemy flip their direction of movement
		}
		return;
	}

}




void tile_3(int i, int j, World* world, Entity* self)
{

	//ONE WAY ON RIGHT
	if ((self->bounds.x + self->bounds.w > i * 64) && (!((self->bounds.y + self->bounds.h > j * 64 + 64) || (self->bounds.y < j * 64))))
	{
		//slog("on right");

		//slog("%f x", i * 64);
		//self->position.x = i * 64;
		self->velocity.x = 0;
		self->position.x = i * 64 + 64;
		if (self->team == ETT_enemy)
		{
			self->directionX *= -1;//IF enemy flip their direction of movement
		}
		return;//NOTE: I want the collision to check for one then pass, because we can't be colliding on multiple sidesof the same tile,
		//if the player is colliding on the side, then they are definitely colliding with the top or bottommm

	}
	//if at wall of tile, and not ontop or below same tile
	//if that and is not there it  will push the player - use for treadmill tiles

}

void tile_4(int i, int j, World* world, Entity* self) {
	self->directionY = -1;
	self->acceleration.y = -.5;
}

void tile_5(int i, int j, World* world, Entity* self) {
	self->directionY = 1;
	self->acceleration.y = .5;//Default  Gravity
}
void tile_6(int i, int j, World* world, Entity* self) {
	slog("hazard hit");
	//entity_free(self);
	damageSelf(entity_player_get());
}

void tile_7(int i, int j, World* world, Entity* self) {
	self->directionY = -1;
	self->acceleration.y = .15;
}

void tile_8(int i, int j, World* world, Entity* self) {
	self->directionY = 1;
	self->acceleration.y = 2;
}


void tile_9(int i, int j, World* world, Entity* self)
{

	//Not working for full collision, perfect for one way tiles  for top or left modidfied


	if (self->bounds.x + self->bounds.w > i * 64)
	{
		//slog("on right");

		//slog("%f x", i * 64);
		//self->position.x = i * 64;
		self->velocity.x = 0;
		self->position.x = i * 64 + 64;
		if (self->team == ETT_enemy)
		{
			self->directionX *= -1;//IF enemy flip their direction of movement
		}
		return;//NOTE: I want the collision to check for one then pass, because we can't be colliding on multiple sidesof the same tile,
		//if the player is colliding on the side, then they are definitely colliding with the top or bottommm

	}
	//if at wall of tile, and not ontop or below same tile
	//if that and is not there it  will push the player - use for treadmill tiles



	if ((self->bounds.y < j * 64))
	{
		//self->position.y = j * 64;
		//slog("on top");

		self->velocity.y = 0;
		self->position.y = (j - 1) * 64;//What this does is set the y position of self to the tile bottom of the tile above j
		if (self->isFlying == 1)
		{
			self->directionY *= -1;//IF enemy flip their direction of movement
		}
		return;
	}

}


void tile_10(int i, int j, World* world, Entity* self)
{

	//Not working for full collision, perfect for one way tiles  for top or left modidfied

	if (self->bounds.x < i * 64) 
	{
		//slog("on left");

		//slog("%f x", i * 64);
		//self->position.x = i * 64+64;
		self->velocity.x = 0;
		self->position.x = i * 64 - 64;
		if (self->team == ETT_enemy)
		{
			self->directionX *= -1;
		}
		return;//NOTE: I want the collision to check for one then pass, because we can't be colliding on multiple sidesof the same tile,
		//if the player is colliding on the side, then they are definitely colliding with the top or bottommm

	}
	//if at wall of tile, and not ontop or below same tile
	//if that and is not there it  will push the player - use for treadmill tiles



	if ((self->bounds.y < j * 64))
	{
		//self->position.y = j * 64;
		//slog("on top");

		self->velocity.y = 0;
		self->position.y = (j - 1) * 64;//What this does is set the y position of self to the tile bottom of the tile above j
		if (self->isFlying == 1)
		{
			self->directionY *= -1;//IF enemy flip their direction of movement
		}
		return;
	}

}

//TODO : SET EACH EDGE CHECK AS A NEW SEPERATE FUNCTION, TILE THREE HAS PERFECT COLLISION, but is one way on right
//IF EDGE() only check for left and calc

void oneWayRight(int i, int j, World* world, Entity* self)
{

	//ONE WAY ON RIGHT
	if ((self->bounds.x + self->bounds.w > i * 64) && (!((self->bounds.y + self->bounds.h > j * 64 + 64) || (self->bounds.y < j * 64))))
	{
		//slog("on right");

		//slog("%f x", i * 64);
		//self->position.x = i * 64;
		self->velocity.x = 0;
		self->position.x = i * 64 + 64;
		if (self->team == ETT_enemy)
		{
			self->directionX *= -1;//IF enemy flip their direction of movement
		}
		return;//NOTE: I want the collision to check for one then pass, because we can't be colliding on multiple sidesof the same tile,
		//if the player is colliding on the side, then they are definitely colliding with the top or bottommm

	}
	//if at wall of tile, and not ontop or below same tile
	//if that and is not there it  will push the player - use for treadmill tiles
}

void oneWayLeft(int i, int j, World* world, Entity* self)
{

	if ((self->bounds.x < i * 64) && (!((self->bounds.y + self->bounds.h > j * 64 + 64) || (self->bounds.y < j * 64))))
	{
		//slog("on left");

		//slog("%f x", i * 64);
		//self->position.x = i * 64+64;
		self->velocity.x = 0;
		self->position.x = i * 64 - 64;
		if (self->team == ETT_enemy)
		{
			self->directionX *= -1;
		}
		return;//NOTE: I want the collision to check for one then pass, because we can't be colliding on multiple sidesof the same tile,
		//if the player is colliding on the side, then they are definitely colliding with the top or bottommm

	}
	//if at wall of tile, and not ontop or below same tile
	//if that and is not there it  will push the player - use for treadmill tiles
}

void oneWayTop(int i, int j, World* world, Entity* self)
{

	if ((self->bounds.y < j * 64))
	{
		//self->position.y = j * 64;
		//slog("on top");

		self->velocity.y = 0;
		self->position.y = (j - 1) * 64;//What this does is set the y position of self to the tile bottom of the tile above j
		if (self->isFlying == 1)
		{
			self->directionY *= -1;//IF enemy flip their direction of movement
		}
		return;
	}
	//if at wall of tile, and not ontop or below same tile
	//if that and is not there it  will push the player - use for treadmill tiles
}

void oneWayBottom(int i, int j, World* world, Entity* self)
{

	if ((self->bounds.y + self->bounds.h > j * 64 + 64))
	{
		//slog("on bottom");
		//self->position.y = j * 64 + 64;
		self->velocity.y = 0;
		//Move to +1 below the tile so player does not stick and will fall affected by gravity
		self->position.y = j * 64 + 65;//What this does is set the y position of self to the tile bottom of the tile above j
		if (self->isFlying == 1)
		{
			self->directionY *= -1;//IF enemy flip their direction of movement
		}
		return;
	}
}

//TODO
// This might work.
//IF TILE ABOVE IS NOT 0 - collide using top collision

//IF TILE BELOW IS NOT 0 - collide using bottom collision

//IF TILE RIGHT IS NOT 0 - collide using right collision

//IF TILE LEFT IS NOT 0 - collide using left collision


int get_tile_right(float x, float y) 
{
	int xInt,yInt;
	//will get the top corner of tile  we are on
	xInt = (int)(x /64);
	yInt = (int)(y / 64);

	if ((xInt + (yInt * activeworld->tileWidth) + 1) < 0)return 0;
	if ((xInt + (yInt * activeworld->tileWidth) + 1) > activeworld->tileHeight * activeworld->tileWidth)return 0;
	return activeworld->tileMap[xInt + (yInt * activeworld->tileWidth)+1];
}

int get_tile_left(float x, float y)
{
	int xInt, yInt;
	//will get the top corner of tile  we are on
	xInt = (int)(x / 64);
	yInt = (int)(y / 64);

	if ((xInt + (yInt * activeworld->tileWidth) - 1) < 0)return 0;
	if ((xInt + (yInt * activeworld->tileWidth) - 1) > activeworld->tileHeight*activeworld->tileWidth)return 0;
	return activeworld->tileMap[xInt + (yInt * activeworld->tileWidth) - 1];
}

int get_tile_above(float x, float y)
{
	int xInt, yInt;
	//will get the top corner of tile  we are on
	xInt = (int)(x / 64);
	yInt = (int)(y / 64);

	if ((xInt + ((yInt - 1) * activeworld->tileWidth)) < 0)return 0;
	if ((xInt + ((yInt - 1) * activeworld->tileWidth)) > activeworld->tileHeight * activeworld->tileWidth)return 0;
	return activeworld->tileMap[xInt + ((yInt-1) * activeworld->tileWidth)];
}

int get_tile_below(float x, float y)
{
	int xInt, yInt;
	//will get the top corner of tile  we are on
	xInt = (int)(x /64);
	yInt = (int)(y /64);

	if ((xInt + ((yInt + 1) * activeworld->tileWidth)) < 0)return 0;
	if ((xInt + ((yInt + 1) * activeworld->tileWidth)) > activeworld->tileHeight * activeworld->tileWidth)return 0;
	return activeworld->tileMap[xInt + ((yInt+1) * activeworld->tileWidth)];
}

/*
int canGoThere(Entity* self, float x, float y) {
	int xInt, yInt;
	GFC_Vector2D posPredict;
	//will get the top corner of tile  we are on
	xInt = (int)(x / 64);
	yInt = (int)(y / 64);

	gfc_vector2d_add(posPredict, self->position, self->velocity);
	

}*/

int HollowTileCheckRight(int i)
{
	switch (i) {
	case 1:return 1; break;
	default:break;
	}
	return 0;
}

int HollowTileCheckLeft(int i) 
{
	switch (i) {
	case 1:
	case 3:return 1; break;
	default:break;
	}
	return 0;
}

int HollowTileCheckUp(int i) 
{
	switch (i) {
	case 1:return 1; break;
	default:break;
	}
	return 0;
}

int HollowTileCheckDown(int i)
{
	switch (i) {
	case 1:return 1; break;
	default:break;
	}
	return 0;
}


//****************
// These were originally specific functions for deliverables. I plan to make them all one way with a specific direction and rename these tiles to a new number later.
//******************

/*
void tile_4(int i, int j, World* world, Entity* self) {
	self->directionY = -1;
	self->acceleration.y = -.5;
}

void tile_5(int i, int j, World* world, Entity* self) {
	self->directionY = 1;
	self->acceleration.y = .5;//Default  Gravity
}
void tile_6(int i, int j, World* world, Entity* self) {
	slog("hazard hit");
	//entity_free(self);
	damageSelf(entity_player_get());
}

*/

void HighlightTile(float mx, float my, World *world)
{
	//int MouseX = (int)mx % 64;
	//int MouseY = (int)my % 64;
	const Uint8* keys;
	keys = SDL_GetKeyboardState(NULL);

		if (!activeworld) {
			slog("no active world to draw");
			return;
		}


		GFC_Vector2D position;
		GFC_Vector2D mouse;
		GFC_Vector2D offset;

		offset = camera_get_offset();
		//slog("It was poition");
		position.x = 0;
		position.y = 0;

		mouse.x = mx;
		mouse.y = my;

		//originally the draw imag function had  vector2D(0,0), that was giving me an error, so I initialized position to 0,0 and am using it once
		//to call draw at 0,0 and then for the for loops which will override the value.


		int i, j, index;
		GFC_Rect bounds;
		//NOTE refering to world crashes, but using activeworld works.

		/*
		if (self->bounds==NULL) {
			return;
		}
		*/
		for (j = 0; j < activeworld->tileHeight; j++)
		{
			for (i = 0; i < activeworld->tileWidth; i++)
			{

				index = i + (j * activeworld->tileWidth);//converts position on tilemap into position in array
				//if (activeworld->tileMap[index] == 0)continue;
				bounds = gfc_rect(i * 64 + offset.x, j * 64 + offset.y, 64, 64);
				if (gfc_point_in_rect(mouse, bounds)) 
				{
					gf2d_draw_rect(bounds, GFC_COLOR_GREEN);
					if (keys[SDL_SCANCODE_R]) {
						DrawTile(i * 64 + offset.x, j * 64 + offset.y, activeworld, index);
					}
					if (keys[SDL_SCANCODE_E]) {
						EraseTile(i * 64 + offset.x, j * 64 + offset.y, activeworld, index);
					}
				}
				
				
				/*
				*/
			}
		}


}

void DrawTile(float mx, float my, World* world,int index)
{
	//int MouseX = (int)mx % 64;
	//int MouseY = (int)my % 64;


	if (!activeworld) {
		slog("no active world to draw");
		return;
	}


	GFC_Vector2D position;
	GFC_Vector2D mouse;
	GFC_Vector2D offset;

	offset = camera_get_offset();
	//slog("It was poition");
	

	mouse.x = mx;
	mouse.y = my;

	//originally the draw imag function had  vector2D(0,0), that was giving me an error, so I initialized position to 0,0 and am using it once
	//to call draw at 0,0 and then for the for loops which will override the value.


	int i, j;
	Uint32  frame;
	GFC_Rect bounds;
	//NOTE refering to world crashes, but using activeworld works.

	/*
	if (self->bounds==NULL) {
		return;
	}
	*/
	//index = i + (j * world->tileWidth);//converts position on tilemap into position in array
	//if (world->tileMap[index] == 0) continue;//skip this cause its empty

	world->tileMap[index] = 1;
	//frame = world->tileMap[index] - 1;
	
	gf2d_sprite_free(world->tileLayer);
	world_tile_layer_build(world);
	//world_draw_active(activeworld);
}



void EraseTile(float mx, float my, World* world, int index)
{
	//int MouseX = (int)mx % 64;
	//int MouseY = (int)my % 64;


	if (!activeworld) {
		slog("no active world to draw");
		return;
	}


	GFC_Vector2D position;
	GFC_Vector2D mouse;
	GFC_Vector2D offset;

	offset = camera_get_offset();
	//slog("It was poition");


	mouse.x = mx;
	mouse.y = my;

	//originally the draw imag function had  vector2D(0,0), that was giving me an error, so I initialized position to 0,0 and am using it once
	//to call draw at 0,0 and then for the for loops which will override the value.


	int i, j;
	Uint32  frame;
	GFC_Rect bounds;
	//NOTE refering to world crashes, but using activeworld works.

	/*
	if (self->bounds==NULL) {
		return;
	}
	*/
	//index = i + (j * world->tileWidth);//converts position on tilemap into position in array
	//if (world->tileMap[index] == 0) continue;//skip this cause its empty

	world->tileMap[index] = 0;
	//frame = world->tileMap[index] - 1;

	gf2d_sprite_free(world->tileLayer);
	world_tile_layer_build(world);
	//world_draw_active(activeworld);
}

