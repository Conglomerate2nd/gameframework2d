#include <SDL.h>
#include "simple_logger.h"
#include "gfc_input.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "gfc_shape.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include "gfc_audio.h"
#include "SDL_mixer.h"

#include "gfc_string.h"
#include "entity.h"
#include "player.h"
#include "flyer.h"
#include "walker.h"
#include "world.h"
#include "camera.h"
#include "attack.h"

int main(int argc, char * argv[])
{
    GFC_Vector2D screen;
    screen.x = 1200;
    screen.y = 720;
    /*variable declarations*/
    int done = 0;

    const Uint8 * keys;
    Sprite *sprite;
    //World *world;
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    GFC_Color mouseGFC_Color = gfc_color8(0,100,255,200);
    GFC_Rect rectBar;
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
    gfc_input_init("input.cfg");
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    entity_system_init(1000);//Very low ball number
    SDL_ShowCursor(SDL_DISABLE);
    camera_set_size(screen);
    player_new_pos(2 * 64, 4 * 64);//Hard code for something later and world draw testing
    //attack_new_pos1(4 * 64, 4 * 64,1);//Hard code for something later and world draw testing
    walker_new_pos(4 * 64, 4 * 64);//Hard code for something later and world draw testing


    gfc_audio_init(32, 8, 1, 1, true, false);
    Mix_VolumeMusic(32);

    //Feature Debug Flags
    int drawFlag = 0;


    Mix_Music *music;
    music = Mix_LoadMUS("audio/platform-shoes-8-bit-chiptune-instrumental-336417.mp3");
    Mix_PlayMusic(music, -1);
    //slog(music);
    //slog(Mix_PlayMusic(music, -1));

    //I dont know why this works gfc_sound_init is private
    //gfc_sound_init(99);
    //music = gfc_sound_load("audio/platform-shoes-8-bit-chiptune-instrumental-336417.mp3", 1, -1);
    //slog(SDL_GetError());

    //gfc_sound_play(music, 99, 10, -1, -1);
    //flag end 

    /*demo setup*/
   // sprite = gf2d_sprite_load_image("images/backgrounds/test.png");

    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    
    //player = player_new_pos(384,384);
    activeworld = world_load("maps/menu.map");
    //Hardcode Test
    //Entity* walk = walker_new_pos(128, 128);
    //Entity* fly = flyer_new();
    //GFC_Rect rect = gfc_rect(0,100,100,5);
    //world_setup_camera(activeworld);

    
    for (int i = 0; i < argc; i++)
    {
        slog(argv[i]);
        if (gfc_strlcmp("draw", argv[i])==0) {
            drawFlag = 1;
            done = 1;//skip main menu loop
        }
        else  drawFlag = 0;
    }

    slog("press [escape] to quit");

   // gf2d_sprite_load_image("images/backgrounds/test-export2.png");

    //MAIN MENU ATTEMPTS
    while (!done)
    {
        
        gfc_input_update();
        //SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx, &my);
        mf += 0.1;
        if (mf >= 16.0)mf = 0;

        
        gf2d_graphics_clear_screen();// clears drawing buffers
        
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
        //    //gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));
        //world_draw_active(activeworld);

        //Between Background and UI
        

        //UI elements last
        gf2d_sprite_draw(
            mouse,
            gfc_vector2d(mx, my),
            NULL,
            NULL,
            NULL,
            NULL,
            &mouseGFC_Color,
            (int)mf);

       //if (drawFlag == 1) { HighlightTile(mx, my, activeworld); }
        


        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame


        /*
        test demo
        */
        // entity_obj_collision_poc(player,rect,);

        if (keys[SDL_SCANCODE_RETURN])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }

    world_free(activeworld);
    activeworld = world_load("maps/world.map");
    done = 0;//ready for loop two
    //Mix_HaltMusic();
    /*main game loop*/
    slog(music);
    while(!done)
    {
        //Mix_PlayMusic(music, -1);
        //slog("music address is ", music);
        //slog(Mix_PlayMusic(music, -1));
        if (activeworld->music != NULL) {
            //Mix_PlayMusic(activeworld->music, -1);
            //slog("Music should be playing");
        }
       
        //gfc_sound_play(music, 99, 10, -1, -1);
        gfc_input_update();
        //SDL_PumpEvents();   // update SDL's internal event structures
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
            world_draw_active(activeworld);

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
            
            if(entity_player_get())
            {
                if (entity_player_get()->health <= 0) {
                    // slog("no health");
                }
                for (int i = 0; i < entity_player_get()->health; i++) {
                    rectBar = gfc_rect(i * 32 + 32, 32, 32, 32);
                    gf2d_draw_rect_filled(rectBar, GFC_COLOR_RED);
                }
            }

            //still no
            if (drawFlag == 1) { HighlightTile(mx, my, activeworld); }
          
            
        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        

        /*
        test demo
        */
       // entity_obj_collision_poc(player,rect,);

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }

    //not necessary but just in case
    entity_free_all(NULL);
    //entity_free(walk);
    //entity_free(fly);
    world_free(activeworld);
    slog("---==== END ====---");
    return 0;
}

void mainMenu()
{

}
/*eol@eof*/
