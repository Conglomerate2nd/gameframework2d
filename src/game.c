#include <SDL.h>
#include "simple_logger.h"
#include "gfc_input.h"
#include "gf2d_graphics.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"

#include "entity.h"
#include "player.h"
#include "flyer.h"
#include "walker.h"
#include "world.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    //Sprite *sprite;
    World *world;
    //gfc_input_init("input.cfg");
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    GFC_Color mouseGFC_Color = gfc_color8(0,100,255,200);
    Entity* player;// not necessary, can be declared at definition, but helps readability
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    entity_system_init(100);//Very low ball number
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    //sprite = gf2d_sprite_load_image("images/backgrounds/test.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);

    player = player_new();
    world = world_load("maps/world.map");
    //Hardcode Test
    Entity* walk = walker_new();
    //Entity* fly = flyer_new();
    //GFC_Rect rect = gfc_rect(0,100,100,5);
    


    slog("press [escape] to quit");
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;

        /*
        * Entity and Update Begin
        */
        entity_system_think();
        entity_system_update();
        entity_system_collision();



        /*
        * Entity and Update End
        */

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            //gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));
            world_draw(world);

            //Between Background and UI
            entity_system_draw();

            //UI elements last
            gf2d_sprite_draw(
                mouse,
                gfc_vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseGFC_Color,
                (int)mf);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        

        /*
        test demo
        */
       // entity_obj_collision_poc(player,rect,);

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }

    //not necessary but just in case
    entity_free(player);
    entity_free(walk);
    //entity_free(fly);
    world_free(world);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
