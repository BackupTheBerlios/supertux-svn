/*
  gameloop.c
  
  Super Tux - Game Loop!
  
  by Bill Kendrick & Tobias Glaesser
  bill@newbreedsoftware.com
  http://www.newbreedsoftware.com/supertux/
  
  April 11, 2000 - March 15, 2004
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <SDL.h>

#ifndef WIN32
#include <sys/types.h>
#include <ctype.h>
#endif

#include "defines.h"
#include "globals.h"
#include "gameloop.h"
#include "screen.h"
#include "setup.h"
#include "high_scores.h"
#include "menu.h"
#include "badguy.h"
#include "world.h"
#include "special.h"
#include "player.h"
#include "level.h"
#include "scene.h"
#include "collision.h"

/* extern variables */

st_level current_level;
int game_started = false;

/* Local variables: */

static texture_type img_waves[3], img_water, img_pole, img_poletop, img_flag[2];
static texture_type img_cloud[2][4];
static SDL_Event event;
static SDLKey key;
static char level_subset[100];
static float fps_fps;
static int st_gl_mode;
static unsigned int last_update_time;
static unsigned int update_time;
static int pause_menu_frame;
static int debug_fps;

/* Local function prototypes: */

void levelintro(void);
void loadshared(void);
void unloadshared(void);
void drawstatus(void);
void drawendscreen(void);
void drawresultscreen(void);

void levelintro(void)
{
  char str[60];
  /* Level Intro: */
  clearscreen(0, 0, 0);

  sprintf(str, "LEVEL %d", level);
  text_drawf(&blue_text, str, 0, 200, A_HMIDDLE, A_TOP, 1);

  sprintf(str, "%s", current_level.name.c_str());
  text_drawf(&gold_text, str, 0, 224, A_HMIDDLE, A_TOP, 1);

  sprintf(str, "TUX x %d", tux.lives);
  text_drawf(&white_text, str, 0, 256, A_HMIDDLE, A_TOP, 1);

  flipscreen();

  SDL_Delay(1000);
}

/* Reset Timers */
void start_timers(void)
{
  timer_start(&time_left,current_level.time_left*1000);
  st_pause_ticks_init();
  update_time = st_get_ticks();
}

void activate_bad_guys(void)
{
  int x,y;

  /* Activate bad guys: */

  for (y = 0; y < 15; y++)
    {
      for (x = 0; x < current_level.width; x++)
        {
          if (current_level.tiles[y][x] >= '0' && current_level.tiles[y][x] <= '9')
            {
              add_bad_guy(x * 32, y * 32, static_cast<BadGuyKind>(current_level.tiles[y][x] - '0'));
              current_level.tiles[y][x] = '.';
            }
        }
    }

}

/* --- GAME EVENT! --- */

void game_event(void)
{
  while (SDL_PollEvent(&event))
    {
      switch(event.type)
        {
        case SDL_QUIT:        /* Quit event - quit: */
          quit = 1;
          break;
        case SDL_KEYDOWN:     /* A keypress! */
          key = event.key.keysym.sym;

          /* Check for menu-events, if the menu is shown */
          if(show_menu)
            menu_event(&event.key.keysym);

          if(tux.key_event(key,DOWN))
            break;

          switch(key)
            {
            case SDLK_ESCAPE:    /* Escape: Open/Close the menu: */
              if(!game_pause)
                {
                  if(st_gl_mode == ST_GL_TEST)
                    quit = 1;
                  else if(show_menu)
                    {
                      Menu::set_current(game_menu);
                      show_menu = 0;
                      st_pause_ticks_stop();
                    }
                  else
                    {
                      Menu::set_current(game_menu);
                      show_menu = 1;
                      st_pause_ticks_start();
                    }
                }
              break;
            default:
              break;
            }
          break;
        case SDL_KEYUP:      /* A keyrelease! */
          key = event.key.keysym.sym;

          if(tux.key_event(key, UP))
            break;

          switch(key)
            {
            case SDLK_p:
              if(!show_menu)
                {
                  if(game_pause)
                    {
                      game_pause = 0;
                      st_pause_ticks_stop();
                    }
                  else
                    {
                      game_pause = 1;
                      st_pause_ticks_start();
                    }
                }
              break;
            case SDLK_TAB:
              if(debug_mode)
                {
                  tux.size = !tux.size;
                  if(tux.size == BIG)
                    {
                      tux.base.height = 64;
                    }
                  else
                    tux.base.height = 32;
                }
              break;
            case SDLK_END:
              if(debug_mode)
                distros += 50;
              break;
            case SDLK_SPACE:
              if(debug_mode)
                next_level = 1;
              break;
            case SDLK_DELETE:
              if(debug_mode)
                tux.got_coffee = 1;
              break;
            case SDLK_INSERT:
              if(debug_mode)
                timer_start(&tux.invincible_timer,TUX_INVINCIBLE_TIME);
              break;
            case SDLK_l:
              if(debug_mode)
                --tux.lives;
              break;
            case SDLK_s:
              if(debug_mode)
                score += 1000;
            case SDLK_f:
              if(debug_fps)
                debug_fps = false;
	      else
                debug_fps = true;
              break;	      
            default:
              break;
            }
          break;

        case SDL_JOYAXISMOTION:
          switch(event.jaxis.axis)
            {
            case JOY_X:
              if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
                {
                  tux.input.left  = DOWN;
                  tux.input.right = UP;
                }
              else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
                {
                  tux.input.left  = UP;
                  tux.input.right = DOWN;
                }
              else
                {
                  tux.input.left  = DOWN;
                  tux.input.right = DOWN;
                }
              break;
            case JOY_Y:
              if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
                tux.input.down = DOWN;
              else if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
                tux.input.down = UP;
              else
                tux.input.down = UP;

              /* Handle joystick for the menu */
              if(show_menu)
                {
                  if(tux.input.down == DOWN)
                    menuaction = MENU_ACTION_DOWN;
                  else
                    menuaction = MENU_ACTION_UP;
                }
              break;
            default:
              break;
            }
          break;
        case SDL_JOYBUTTONDOWN:
          if (event.jbutton.button == JOY_A)
            tux.input.up = DOWN;
          else if (event.jbutton.button == JOY_B)
            tux.input.fire = DOWN;
          break;
        case SDL_JOYBUTTONUP:
          if (event.jbutton.button == JOY_A)
            tux.input.up = UP;
          else if (event.jbutton.button == JOY_B)
            tux.input.fire = UP;

          if(show_menu)
            menuaction = MENU_ACTION_HIT;
          break;

        default:
          break;

        }  /* switch */

    } /* while */
}

/* --- GAME ACTION! --- */

int game_action(void)
{
  unsigned int i;

  /* (tux_dying || next_level) */
  if (tux.dying || next_level)
    {
      /* Tux either died, or reached the end of a level! */

      halt_music();


      if (next_level)
        {
          /* End of a level! */
          level++;
          next_level = 0;
          if(st_gl_mode != ST_GL_TEST)
            {
              drawresultscreen();
            }
          else
            {
              level_free_gfx();
              level_free(&current_level);
              level_free_song();
              unloadshared();
              arrays_free();
              return(0);
            }
          tux.level_begin();
        }
      else
        {
          tux.is_dying();

          /* No more lives!? */

          if (tux.lives < 0)
            {
              if(st_gl_mode != ST_GL_TEST)
                drawendscreen();

              if(st_gl_mode != ST_GL_TEST)
                {
                  if (score > hs_score)
                    save_hs(score);
                }
              level_free_gfx();
              level_free(&current_level);
              level_free_song();
              unloadshared();
              arrays_free();
              return(0);
            } /* if (lives < 0) */
        }

      /* Either way, (re-)load the (next) level... */

      tux.level_begin();
      set_defaults();
      level_free(&current_level);

      if (st_gl_mode == ST_GL_LOAD_LEVEL_FILE)
        {
          if(level_load(&current_level, level_subset) != 0)
            return 0;
        }
      else
        {
          if(level_load(&current_level,level_subset,level) != 0)
            return 0;
        }

      arrays_free();
      arrays_init();
      activate_bad_guys();
      level_free_gfx();
      level_load_gfx(&current_level);
      level_free_song();
      level_load_song(&current_level);
      if(st_gl_mode != ST_GL_TEST)
        levelintro();
      start_timers();
      /* Play music: */
      play_current_music();
    }

  tux.action();

  /* Handle bouncy distros: */
  for (i = 0; i < bouncy_distros.size(); i++)
    {
      bouncy_distro_action(&bouncy_distros[i]);
    }


  /* Handle broken bricks: */
  for (i = 0; i < broken_bricks.size(); i++)
    {
      broken_brick_action(&broken_bricks[i]);
    }


  /* Handle distro counting: */

  if (counting_distros)
    {
      distro_counter--;

      if (distro_counter <= 0)
        counting_distros = -1;
    }


  /* Handle bouncy bricks: */

  for (i = 0; i < bouncy_bricks.size(); i++)
    {
      bouncy_brick_action(&bouncy_bricks[i]);
    }


  /* Handle floating scores: */

  for (i = 0; i < floating_scores.size(); i++)
    {
      floating_score_action(&floating_scores[i]);
    }


  /* Handle bullets: */

  for (i = 0; i < bullets.size(); ++i)
    {
      bullet_action(&bullets[i]);
    }

  /* Handle upgrades: */

  for (i = 0; i < upgrades.size(); i++)
    {
      upgrade_action(&upgrades[i]);
    }


  /* Handle bad guys: */

  for (i = 0; i < bad_guys.size(); i++)
    {
      bad_guys[i].action();
    }

  /* Handle all possible collisions. */
  collision_handler();

  return -1;
}

/* --- GAME DRAW! --- */

void game_draw(void)
{
  /* Draw screen: */
  if (tux.dying && (global_frame_counter % 4) == 0)
    clearscreen(255, 255, 255);
  else if(timer_check(&super_bkgd_timer))
    texture_draw(&img_super_bkgd, 0, 0);
  else
    {
      /* Draw the real background */
      if(current_level.bkgd_image[0] != '\0')
        {
          int s = (int)scroll_x / 30;
          texture_draw_part(&img_bkgd,s,0,0,0,img_bkgd.w - s, img_bkgd.h);
          texture_draw_part(&img_bkgd,0,0,screen->w - s ,0,s,img_bkgd.h);
        }
      else
        {
          clearscreen(current_level.bkgd_red, current_level.bkgd_green, current_level.bkgd_blue);
        }
    }

  // Draw background:
  for (int y = 0; y < 15; ++y)
    for (int x = 0; x < 21; ++x)
      drawshape(32*x - fmodf(scroll_x, 32), y * 32,
                current_level.tiles[y][x + (int)(scroll_x / 32)]);

  for (unsigned int i = 0; i < bouncy_bricks.size(); ++i)
    bouncy_brick_draw(&bouncy_bricks[i]);

  for (unsigned int i = 0; i < bad_guys.size(); ++i)
    bad_guys[i].draw();

  tux.draw();

  for (unsigned int i = 0; i < bullets.size(); ++i)
    bullet_draw(&bullets[i]);

  for (unsigned int i = 0; i < floating_scores.size(); ++i)
    floating_score_draw(&floating_scores[i]);

  for (unsigned int i = 0; i < upgrades.size(); ++i)
    upgrade_draw(&upgrades[i]);

  for (unsigned int i = 0; i < bouncy_distros.size(); ++i)
      bouncy_distro_draw(&bouncy_distros[i]);

  for (unsigned int i = 0; i < broken_bricks.size(); ++i)
    broken_brick_draw(&broken_bricks[i]);

  drawstatus();

  if(game_pause)
    {
      int x = screen->h / 20;
      for(int i = 0; i < x; ++i)
        {
          fillrect(i % 2 ? (pause_menu_frame * i)%screen->w : -((pause_menu_frame * i)%screen->w) ,(i*20+pause_menu_frame)%screen->h,screen->w,10,20,20,20, rand() % 20 + 1);
        }
      fillrect(0,0,screen->w,screen->h,rand() % 50, rand() % 50, rand() % 50, 128);
      text_drawf(&blue_text, "PAUSE - Press 'P' To Play", 0, 230, A_HMIDDLE, A_TOP, 1);
    }

  if(show_menu)
    menu_process_current();

  /* (Update it all!) */

  updatescreen();


}

/* --- GAME LOOP! --- */

int gameloop(const char * subset, int levelnb, int mode)
{
  int fps_cnt, jump, done;
  timer_type fps_timer, frame_timer;
  timer_init(&fps_timer, true);
  timer_init(&frame_timer, true);

  game_started = true;

  st_gl_mode = mode;
  level = levelnb;

  /* Init the game: */
  arrays_init();
  set_defaults();

  strcpy(level_subset,subset);

  if (st_gl_mode == ST_GL_LOAD_LEVEL_FILE)
    {
      if (level_load(&current_level, level_subset))
        exit(1);
    }
  else
    {
      if(level_load(&current_level, level_subset, level) != 0)
        exit(1);
    }

  level_load_gfx(&current_level);
  activate_bad_guys();
  level_load_song(&current_level);

  tux.init();

  if(st_gl_mode != ST_GL_TEST)
    load_hs();

  loadshared();

  if(st_gl_mode == ST_GL_PLAY || st_gl_mode == ST_GL_LOAD_LEVEL_FILE)
    levelintro();

  timer_init(&time_left,true);
  start_timers();

  if(st_gl_mode == ST_GL_LOAD_GAME)
    loadgame(levelnb);


  /* --- MAIN GAME LOOP!!! --- */

  jump = false;
  done = 0;
  quit = 0;
  global_frame_counter = 0;
  game_pause = 0;
  timer_init(&fps_timer,true);
  timer_init(&frame_timer,true);
  fps_cnt = 0;

  /* Clear screen: */

  clearscreen(0, 0, 0);
  updatescreen();

  /* Play music: */
  play_current_music();


  while (SDL_PollEvent(&event))
    {}

  game_draw();
  do
    {
      jump = false;

      /* Calculate the movement-factor */
      frame_ratio = ((double)(update_time-last_update_time))/((double)FRAME_RATE);
      if(frame_ratio > 1.5) /* Quick hack to correct the unprecise CPU clocks a little bit. */
        frame_ratio = 1.5 + (frame_ratio - 1.5) * 0.85;
      
      if(!timer_check(&frame_timer))
        {
          timer_start(&frame_timer,25);
          ++global_frame_counter;
        }

      /* Handle events: */

      tux.input.old_fire = tux.input.fire;

      game_event();

      if(show_menu)
        {
          if(current_menu == game_menu)
            {
              switch (game_menu->check())
                {
                case 2:
                  st_pause_ticks_stop();
                  break;
                case 3:
                  update_load_save_game_menu(save_game_menu, false);
                  break;
                case 4:
                  update_load_save_game_menu(load_game_menu, true);
                  break;
                case 7:
                  st_pause_ticks_stop();
                  done = 1;
                  break;
                }
            }
          else if(current_menu == options_menu)
            {
              process_options_menu();
            }
          else if(current_menu == save_game_menu )
            {
              process_save_load_game_menu(true);
            }
          else if(current_menu == load_game_menu )
            {
              process_save_load_game_menu(false);
            }
        }


      /* Handle actions: */

      if(!game_pause && !show_menu)
        {
          /*float z = frame_ratio;
            frame_ratio = 1;
            while(z >= 1)
            {*/
          if (game_action() == 0)
            {
              /* == 0: no more lives */
              /* == -1: continues */
              return 0;
            }
	  /*  --z;
              }*/
        }
      else
        {
          ++pause_menu_frame;
          SDL_Delay(50);
        }

      if(debug_mode && debug_fps)
        SDL_Delay(60);

      /*Draw the current scene to the screen */
      /*If the machine running the game is too slow
        skip the drawing of the frame (so the calculations are more precise and
        the FPS aren't affected).*/
      /*if( ! fps_fps < 50.0 )
        game_draw();
        else
        jump = true;*/ /*FIXME: Implement this tweak right.*/
      game_draw();

      /* Time stops in pause mode */
      if(game_pause || show_menu )
        {
          continue;
        }

      /* Set the time of the last update and the time of the current update */
      last_update_time = update_time;
      update_time = st_get_ticks();


      /* Pause till next frame, if the machine running the game is too fast: */
      /* FIXME: Works great for in OpenGl mode, where the CPU doesn't have to do that much. But
         the results in SDL mode aren't perfect (thought the 100 FPS are reached), even on an AMD2500+. */
      if(last_update_time >= update_time - 12 && !jump)
        SDL_Delay(10);
      /*if((update_time - last_update_time) < 10)
        SDL_Delay((11 - (update_time - last_update_time))/2);*/



      /* Handle time: */

      if (timer_check(&time_left))
        {
          /* are we low on time ? */
          if ((timer_get_left(&time_left) < TIME_WARNING)
              && (get_current_music() != HURRYUP_MUSIC))     /* play the fast music */
            {
              set_current_music(HURRYUP_MUSIC);
              play_current_music();
            }

        }
      else
        tux.kill(KILL);


      /* Calculate frames per second */
      if(show_fps)
        {
          ++fps_cnt;
          fps_fps = (1000.0 / (float)timer_get_gone(&fps_timer)) * (float)fps_cnt;

          if(!timer_check(&fps_timer))
            {
              timer_start(&fps_timer,1000);
              fps_cnt = 0;
            }
        }

    }
  while (!done && !quit);

  halt_music();

  level_free_gfx();
  level_free(&current_level);
  level_free_song();
  unloadshared();
  arrays_free();

  game_started = false;

  return(quit);
}


/* Load graphics/sounds shared between all levels: */

void loadshared(void)
{
  int i;

  /* Tuxes: */
  texture_load(&tux_right[0], datadir + "/images/shared/tux-right-0.png", USE_ALPHA);
  texture_load(&tux_right[1], datadir + "/images/shared/tux-right-1.png", USE_ALPHA);
  texture_load(&tux_right[2], datadir + "/images/shared/tux-right-2.png", USE_ALPHA);

  texture_load(&tux_left[0], datadir + "/images/shared/tux-left-0.png", USE_ALPHA);
  texture_load(&tux_left[1], datadir + "/images/shared/tux-left-1.png", USE_ALPHA);
  texture_load(&tux_left[2], datadir + "/images/shared/tux-left-2.png", USE_ALPHA);

  texture_load(&firetux_right[0], datadir + "/images/shared/firetux-right-0.png", USE_ALPHA);
  texture_load(&firetux_right[1], datadir + "/images/shared/firetux-right-1.png", USE_ALPHA);
  texture_load(&firetux_right[2], datadir + "/images/shared/firetux-right-2.png", USE_ALPHA);

  texture_load(&firetux_left[0], datadir + "/images/shared/firetux-left-0.png", USE_ALPHA);
  texture_load(&firetux_left[1], datadir + "/images/shared/firetux-left-1.png", USE_ALPHA);
  texture_load(&firetux_left[2], datadir + "/images/shared/firetux-left-2.png", USE_ALPHA);


  texture_load(&cape_right[0], datadir + "/images/shared/cape-right-0.png",
               USE_ALPHA);

  texture_load(&cape_right[1], datadir + "/images/shared/cape-right-1.png",
               USE_ALPHA);

  texture_load(&cape_left[0], datadir + "/images/shared/cape-left-0.png",
               USE_ALPHA);

  texture_load(&cape_left[1], datadir + "/images/shared/cape-left-1.png",
               USE_ALPHA);

  texture_load(&bigtux_right[0], datadir + "/images/shared/bigtux-right-0.png",
               USE_ALPHA);

  texture_load(&bigtux_right[1], datadir + "/images/shared/bigtux-right-1.png",
               USE_ALPHA);

  texture_load(&bigtux_right[2], datadir + "/images/shared/bigtux-right-2.png",
               USE_ALPHA);

  texture_load(&bigtux_right_jump, datadir + "/images/shared/bigtux-right-jump.png", USE_ALPHA);

  texture_load(&bigtux_left[0], datadir + "/images/shared/bigtux-left-0.png",
               USE_ALPHA);

  texture_load(&bigtux_left[1], datadir + "/images/shared/bigtux-left-1.png",
               USE_ALPHA);

  texture_load(&bigtux_left[2], datadir + "/images/shared/bigtux-left-2.png",
               USE_ALPHA);

  texture_load(&bigtux_left_jump, datadir + "/images/shared/bigtux-left-jump.png", USE_ALPHA);

  texture_load(&bigcape_right[0], datadir + "/images/shared/bigcape-right-0.png",
               USE_ALPHA);

  texture_load(&bigcape_right[1], datadir + "/images/shared/bigcape-right-1.png",
               USE_ALPHA);

  texture_load(&bigcape_left[0], datadir + "/images/shared/bigcape-left-0.png",
               USE_ALPHA);

  texture_load(&bigcape_left[1], datadir + "/images/shared/bigcape-left-1.png",
               USE_ALPHA);

  texture_load(&bigfiretux_right[0], datadir + "/images/shared/bigfiretux-right-0.png",
               USE_ALPHA);

  texture_load(&bigfiretux_right[1], datadir + "/images/shared/bigfiretux-right-1.png",
               USE_ALPHA);

  texture_load(&bigfiretux_right[2], datadir + "/images/shared/bigfiretux-right-2.png",
               USE_ALPHA);

  texture_load(&bigfiretux_right_jump, datadir + "/images/shared/bigfiretux-right-jump.png", USE_ALPHA);

  texture_load(&bigfiretux_left[0], datadir + "/images/shared/bigfiretux-left-0.png",
               USE_ALPHA);

  texture_load(&bigfiretux_left[1], datadir + "/images/shared/bigfiretux-left-1.png",
               USE_ALPHA);

  texture_load(&bigfiretux_left[2], datadir + "/images/shared/bigfiretux-left-2.png",
               USE_ALPHA);

  texture_load(&bigfiretux_left_jump, datadir + "/images/shared/bigfiretux-left-jump.png", USE_ALPHA);

  texture_load(&bigcape_right[0], datadir + "/images/shared/bigcape-right-0.png",
               USE_ALPHA);

  texture_load(&bigcape_right[1], datadir + "/images/shared/bigcape-right-1.png",
               USE_ALPHA);

  texture_load(&bigcape_left[0], datadir + "/images/shared/bigcape-left-0.png",
               USE_ALPHA);

  texture_load(&bigcape_left[1], datadir + "/images/shared/bigcape-left-1.png",
               USE_ALPHA);


  texture_load(&ducktux_right, datadir +
               "/images/shared/ducktux-right.png",
               USE_ALPHA);

  texture_load(&ducktux_left, datadir +
               "/images/shared/ducktux-left.png",
               USE_ALPHA);

  texture_load(&skidtux_right, datadir +
               "/images/shared/skidtux-right.png",
               USE_ALPHA);

  texture_load(&skidtux_left, datadir +
               "/images/shared/skidtux-left.png",
               USE_ALPHA);

  texture_load(&duckfiretux_right, datadir +
               "/images/shared/duckfiretux-right.png",
               USE_ALPHA);

  texture_load(&duckfiretux_left, datadir +
               "/images/shared/duckfiretux-left.png",
               USE_ALPHA);

  texture_load(&skidfiretux_right, datadir +
               "/images/shared/skidfiretux-right.png",
               USE_ALPHA);

  texture_load(&skidfiretux_left, datadir +
               "/images/shared/skidfiretux-left.png",
               USE_ALPHA);


  /* Boxes: */

  texture_load(&img_box_full, datadir + "/images/shared/box-full.png",
               IGNORE_ALPHA);
  texture_load(&img_box_empty, datadir + "/images/shared/box-empty.png",
               IGNORE_ALPHA);


  /* Water: */


  texture_load(&img_water, datadir + "/images/shared/water.png", IGNORE_ALPHA);

  texture_load(&img_waves[0], datadir + "/images/shared/waves-0.png",
               USE_ALPHA);

  texture_load(&img_waves[1], datadir + "/images/shared/waves-1.png",
               USE_ALPHA);

  texture_load(&img_waves[2], datadir + "/images/shared/waves-2.png",
               USE_ALPHA);


  /* Pole: */

  texture_load(&img_pole, datadir + "/images/shared/pole.png", USE_ALPHA);
  texture_load(&img_poletop, datadir + "/images/shared/poletop.png",
               USE_ALPHA);


  /* Flag: */

  texture_load(&img_flag[0], datadir + "/images/shared/flag-0.png",
               USE_ALPHA);
  texture_load(&img_flag[1], datadir + "/images/shared/flag-1.png",
               USE_ALPHA);


  /* Cloud: */

  texture_load(&img_cloud[0][0], datadir + "/images/shared/cloud-00.png",
               USE_ALPHA);

  texture_load(&img_cloud[0][1], datadir + "/images/shared/cloud-01.png",
               USE_ALPHA);

  texture_load(&img_cloud[0][2], datadir + "/images/shared/cloud-02.png",
               USE_ALPHA);

  texture_load(&img_cloud[0][3], datadir + "/images/shared/cloud-03.png",
               USE_ALPHA);


  texture_load(&img_cloud[1][0], datadir + "/images/shared/cloud-10.png",
               USE_ALPHA);

  texture_load(&img_cloud[1][1], datadir + "/images/shared/cloud-11.png",
               USE_ALPHA);

  texture_load(&img_cloud[1][2], datadir + "/images/shared/cloud-12.png",
               USE_ALPHA);

  texture_load(&img_cloud[1][3], datadir + "/images/shared/cloud-13.png",
               USE_ALPHA);


  /* Bad guys: */

  /* (BSOD) */

  texture_load(&img_bsod_left[0], datadir +
               "/images/shared/bsod-left-0.png",
               USE_ALPHA);

  texture_load(&img_bsod_left[1], datadir +
               "/images/shared/bsod-left-1.png",
               USE_ALPHA);

  texture_load(&img_bsod_left[2], datadir +
               "/images/shared/bsod-left-2.png",
               USE_ALPHA);

  texture_load(&img_bsod_left[3], datadir +
               "/images/shared/bsod-left-3.png",
               USE_ALPHA);

  texture_load(&img_bsod_right[0], datadir +
               "/images/shared/bsod-right-0.png",
               USE_ALPHA);

  texture_load(&img_bsod_right[1], datadir +
               "/images/shared/bsod-right-1.png",
               USE_ALPHA);

  texture_load(&img_bsod_right[2], datadir +
               "/images/shared/bsod-right-2.png",
               USE_ALPHA);

  texture_load(&img_bsod_right[3], datadir +
               "/images/shared/bsod-right-3.png",
               USE_ALPHA);

  texture_load(&img_bsod_squished_left, datadir +
               "/images/shared/bsod-squished-left.png",
               USE_ALPHA);

  texture_load(&img_bsod_squished_right, datadir +
               "/images/shared/bsod-squished-right.png",
               USE_ALPHA);

  texture_load(&img_bsod_falling_left, datadir +
               "/images/shared/bsod-falling-left.png",
               USE_ALPHA);

  texture_load(&img_bsod_falling_right, datadir +
               "/images/shared/bsod-falling-right.png",
               USE_ALPHA);


  /* (Laptop) */

  texture_load(&img_laptop_left[0], datadir +
               "/images/shared/laptop-left-0.png",
               USE_ALPHA);

  texture_load(&img_laptop_left[1], datadir +
               "/images/shared/laptop-left-1.png",
               USE_ALPHA);

  texture_load(&img_laptop_left[2], datadir +
               "/images/shared/laptop-left-2.png",
               USE_ALPHA);

  texture_load(&img_laptop_right[0], datadir +
               "/images/shared/laptop-right-0.png",
               USE_ALPHA);

  texture_load(&img_laptop_right[1], datadir +
               "/images/shared/laptop-right-1.png",
               USE_ALPHA);

  texture_load(&img_laptop_right[2], datadir +
               "/images/shared/laptop-right-2.png",
               USE_ALPHA);

  texture_load(&img_laptop_flat_left, datadir +
               "/images/shared/laptop-flat-left.png",
               USE_ALPHA);

  texture_load(&img_laptop_flat_right, datadir +
               "/images/shared/laptop-flat-right.png",
               USE_ALPHA);

  texture_load(&img_laptop_falling_left, datadir +
               "/images/shared/laptop-falling-left.png",
               USE_ALPHA);

  texture_load(&img_laptop_falling_right, datadir +
               "/images/shared/laptop-falling-right.png",
               USE_ALPHA);


  /* (Money) */

  texture_load(&img_money_left[0], datadir +
               "/images/shared/bag-left-0.png",
               USE_ALPHA);

  texture_load(&img_money_left[1], datadir +
               "/images/shared/bag-left-1.png",
               USE_ALPHA);

  texture_load(&img_money_right[0], datadir +
               "/images/shared/bag-right-0.png",
               USE_ALPHA);

  texture_load(&img_money_right[1], datadir +
               "/images/shared/bag-right-1.png",
               USE_ALPHA);



  /* Upgrades: */

  texture_load(&img_mints, datadir + "/images/shared/mints.png", USE_ALPHA);
  texture_load(&img_coffee, datadir + "/images/shared/coffee.png", USE_ALPHA);


  /* Weapons: */

  texture_load(&img_bullet, datadir + "/images/shared/bullet.png", USE_ALPHA);

  texture_load(&img_red_glow, datadir + "/images/shared/red-glow.png",
               USE_ALPHA);



  /* Distros: */

  texture_load(&img_distro[0], datadir + "/images/shared/distro-0.png",
               USE_ALPHA);

  texture_load(&img_distro[1], datadir + "/images/shared/distro-1.png",
               USE_ALPHA);

  texture_load(&img_distro[2], datadir + "/images/shared/distro-2.png",
               USE_ALPHA);

  texture_load(&img_distro[3], datadir + "/images/shared/distro-3.png",
               USE_ALPHA);


  /* Tux life: */

  texture_load(&tux_life, datadir + "/images/shared/tux-life.png",
               USE_ALPHA);

  /* Herring: */

  texture_load(&img_golden_herring, datadir + "/images/shared/golden-herring.png",
               USE_ALPHA);


  /* Super background: */

  texture_load(&img_super_bkgd, datadir + "/images/shared/super-bkgd.png",
               IGNORE_ALPHA);


  /* Sound effects: */

  /* if (use_sound) // this will introduce SERIOUS bugs here ! because "load_sound"
                    // initialize sounds[i] with the correct pointer's value:
                    // NULL or something else. And it will be dangerous to
                    // play with not-initialized pointers.
                    // This is also true with if (use_music)
                    Send a mail to me: neoneurone@users.sf.net, if you have another opinion. :)
  */
  for (i = 0; i < NUM_SOUNDS; i++)
    sounds[i] = load_sound(datadir + soundfilenames[i]);

  /* Herring song */
  herring_song = load_song(datadir + "/music/SALCON.MOD");
}


/* Free shared data: */

void unloadshared(void)
{
  int i;

  for (i = 0; i < 3; i++)
    {
      texture_free(&tux_right[i]);
      texture_free(&tux_left[i]);
      texture_free(&bigtux_right[i]);
      texture_free(&bigtux_left[i]);
    }

  texture_free(&bigtux_right_jump);
  texture_free(&bigtux_left_jump);

  for (i = 0; i < 2; i++)
    {
      texture_free(&cape_right[i]);
      texture_free(&cape_left[i]);
      texture_free(&bigcape_right[i]);
      texture_free(&bigcape_left[i]);
    }

  texture_free(&ducktux_left);
  texture_free(&ducktux_right);

  texture_free(&skidtux_left);
  texture_free(&skidtux_right);

  for (i = 0; i < 4; i++)
    {
      texture_free(&img_bsod_left[i]);
      texture_free(&img_bsod_right[i]);
    }

  texture_free(&img_bsod_squished_left);
  texture_free(&img_bsod_squished_right);

  texture_free(&img_bsod_falling_left);
  texture_free(&img_bsod_falling_right);

  for (i = 0; i < 3; i++)
    {
      texture_free(&img_laptop_left[i]);
      texture_free(&img_laptop_right[i]);
    }

  texture_free(&img_laptop_flat_left);
  texture_free(&img_laptop_flat_right);

  texture_free(&img_laptop_falling_left);
  texture_free(&img_laptop_falling_right);

  for (i = 0; i < 2; i++)
    {
      texture_free(&img_money_left[i]);
      texture_free(&img_money_right[i]);
    }

  texture_free(&img_box_full);
  texture_free(&img_box_empty);

  texture_free(&img_water);
  for (i = 0; i < 3; i++)
    texture_free(&img_waves[i]);

  texture_free(&img_pole);
  texture_free(&img_poletop);

  for (i = 0; i < 2; i++)
    texture_free(&img_flag[i]);

  texture_free(&img_mints);
  texture_free(&img_coffee);

  for (i = 0; i < 4; i++)
    {
      texture_free(&img_distro[i]);
      texture_free(&img_cloud[0][i]);
      texture_free(&img_cloud[1][i]);
    }

  texture_free(&img_golden_herring);

  for (i = 0; i < NUM_SOUNDS; i++)
    free_chunk(sounds[i]);

  /* free the herring song */
  free_music( herring_song );
}


/* Draw a tile on the screen: */

void drawshape(float x, float y, unsigned char c)
{
  int z;

  if (c == 'X' || c == 'x')
    texture_draw(&img_brick[0], x, y);
  else if (c == 'Y' || c == 'y')
    texture_draw(&img_brick[1], x, y);
  else if (c == 'A' || c =='B' || c == '!')
    texture_draw(&img_box_full, x, y);
  else if (c == 'a')
    texture_draw(&img_box_empty, x, y);
  else if (c >= 'C' && c <= 'F')
    texture_draw(&img_cloud[0][c - 'C'], x, y);
  else if (c >= 'c' && c <= 'f')
    texture_draw(&img_cloud[1][c - 'c'], x, y);
  else if (c >= 'G' && c <= 'J')
    texture_draw(&img_bkgd_tile[0][c - 'G'], x, y);
  else if (c >= 'g' && c <= 'j')
    texture_draw(&img_bkgd_tile[1][c - 'g'], x, y);
  else if (c == '#')
    texture_draw(&img_solid[0], x, y);
  else if (c == '[')
    texture_draw(&img_solid[1], x, y);
  else if (c == '=')
    texture_draw(&img_solid[2], x, y);
  else if (c == ']')
    texture_draw(&img_solid[3], x, y);
  else if (c == '$')
    {
      z = (global_frame_counter / 2) % 6;

      if (z < 4)
        texture_draw(&img_distro[z], x, y);
      else if (z == 4)
        texture_draw(&img_distro[2], x, y);
      else if (z == 5)
        texture_draw(&img_distro[1], x, y);
    }
  else if (c == '^')
    {
      z = (global_frame_counter / 3) % 3;

      texture_draw(&img_waves[z], x, y);
    }
  else if (c == '*')
    texture_draw(&img_poletop, x, y);
  else if (c == '|')
    {
      texture_draw(&img_pole, x, y);

    }
  else if (c == '\\')
    {
      z = (global_frame_counter / 3) % 2;

      texture_draw(&img_flag[z], x + 16, y);
    }
  else if (c == '&')
    texture_draw(&img_water, x, y);
}


/* What shape is at some position? */

unsigned char shape(float x, float y)
{

  int xx, yy;
  unsigned char c;

  yy = ((int)y / 32);
  xx = ((int)x / 32);

  if (yy >= 0 && yy < 15 && xx >= 0 && xx <= current_level.width)
    {
      c = current_level.tiles[yy][xx];
    }
  else
    c = '.';

  return(c);
}

/* Is is ground? */


bool issolid(float x, float y)
{
  return (isbrick(x, y) ||
          isice(x, y) ||
          (shape(x, y) == '[') ||
          (shape(x, y) == '=') ||
          (shape(x, y) == ']') ||
          (shape(x, y) == 'A') ||
          (shape(x, y) == 'B') ||
          (shape(x, y) == '!') ||
          (shape(x, y) == 'a'));
}

/* Is it a brick? */

bool isbrick(float x, float y)
{
  return (shape(x, y) == 'X' ||
          shape(x, y) == 'x' ||
          shape(x, y) == 'Y' ||
          shape(x, y) == 'y');
}


/* Is it ice? */

bool isice(float x, float y)
{
  return (shape(x, y) == '#');
}

/* Is it a full box? */

bool isfullbox(float x, float y)
{
  return (shape(x, y) == 'A' ||
          shape(x, y) == 'B' ||
          shape(x, y) == '!');
}

/* Break a brick: */

void trybreakbrick(float x, float y)
{
  if (isbrick(x, y))
    {
      if (shape(x, y) == 'x' || shape(x, y) == 'y')
        {
          /* Get a distro from it: */

          add_bouncy_distro(((int)(x + 1) / 32) * 32,
                            (int)(y / 32) * 32);

          if (!counting_distros)
            {
              counting_distros = true;
              distro_counter = 50;
            }

          if (distro_counter <= 0)
            level_change(&current_level,x, y, 'a');

          play_sound(sounds[SND_DISTRO], SOUND_CENTER_SPEAKER);
          score = score + SCORE_DISTRO;
          distros++;
        }
      else
        {
          /* Get rid of it: */

          level_change(&current_level,x, y,'.');
        }


      /* Replace it with broken bits: */

      add_broken_brick(((int)(x + 1) / 32) * 32,
                       (int)(y / 32) * 32);


      /* Get some score: */

      play_sound(sounds[SND_BRICK], SOUND_CENTER_SPEAKER);
      score = score + SCORE_BRICK;
    }
}


/* Bounce a brick: */

void bumpbrick(float x, float y)
{
  add_bouncy_brick(((int)(x + 1) / 32) * 32,
                   (int)(y / 32) * 32);

  play_sound(sounds[SND_BRICK], SOUND_CENTER_SPEAKER);

}


/* Empty a box: */

void tryemptybox(float x, float y, int col_side)
{
  if (!isfullbox(x, y))
    return;

  // according to the collision side, set the upgrade direction

  if(col_side == LEFT)
    col_side = RIGHT;
  else
    col_side = LEFT;

  switch(shape(x,y))
    {
    case 'A':      /* Box with a distro! */
      add_bouncy_distro(((int)(x + 1) / 32) * 32, (int)(y / 32) * 32 - 32);
      play_sound(sounds[SND_DISTRO], SOUND_CENTER_SPEAKER);
      score = score + SCORE_DISTRO;
      distros++;
      break;
    case 'B':      /* Add an upgrade! */
      if (tux.size == SMALL)     /* Tux is small, add mints! */
        add_upgrade((int)((x + 1) / 32) * 32, (int)(y / 32) * 32 - 32, col_side, UPGRADE_MINTS);
      else     /* Tux is big, add coffee: */
        add_upgrade((int)((x + 1) / 32) * 32, (int)(y / 32) * 32 - 32, col_side, UPGRADE_COFFEE);
      play_sound(sounds[SND_UPGRADE], SOUND_CENTER_SPEAKER);
      break;
    case '!':     /* Add a golden herring */
      add_upgrade((int)((x + 1) / 32) * 32, (int)(y / 32) * 32 - 32, col_side, UPGRADE_HERRING);
      break;
    default:
      break;
    }

  /* Empty the box: */
  level_change(&current_level,x, y, 'a');
}


/* Try to grab a distro: */

void trygrabdistro(float x, float y, int bounciness)
{
  if (shape(x, y) == '$')
    {
      level_change(&current_level,x, y, '.');
      play_sound(sounds[SND_DISTRO], SOUND_CENTER_SPEAKER);

      if (bounciness == BOUNCE)
        {
          add_bouncy_distro(((int)(x + 1) / 32) * 32,
                            (int)(y / 32) * 32);
        }

      score = score + SCORE_DISTRO;
      distros++;
    }
}

/* Try to bump a bad guy from below: */

void trybumpbadguy(float x, float y)
{
  unsigned int i;

  /* Bad guys: */
  for (i = 0; i < bad_guys.size(); i++)
    {
      if (bad_guys[i].base.x >= x - 32 && bad_guys[i].base.x <= x + 32 &&
          bad_guys[i].base.y >= y - 16 && bad_guys[i].base.y <= y + 16)
        {
          if (bad_guys[i].kind == BAD_BSOD ||
              bad_guys[i].kind == BAD_LAPTOP)
            {
              bad_guys[i].dying = DYING_FALLING;
              bad_guys[i].base.ym = -8;
              play_sound(sounds[SND_FALL], SOUND_CENTER_SPEAKER);
            }
        }
    }


  /* Upgrades: */
  for (i = 0; i < upgrades.size(); i++)
    {
      if (upgrades[i].base.height == 32 &&
          upgrades[i].base.x >= x - 32 && upgrades[i].base.x <= x + 32 &&
          upgrades[i].base.y >= y - 16 && upgrades[i].base.y <= y + 16)
        {
          upgrades[i].base.xm = -upgrades[i].base.xm;
          upgrades[i].base.ym = -8;
          play_sound(sounds[SND_BUMP_UPGRADE], SOUND_CENTER_SPEAKER);
        }
    }
}

/* (Status): */
void drawstatus(void)
{
  char str[60];

  sprintf(str, "%d", score);
  text_draw(&white_text, "SCORE", 0, 0, 1);
  text_draw(&gold_text, str, 96, 0, 1);

  if(st_gl_mode != ST_GL_TEST)
    {
      sprintf(str, "%d", hs_score);
      text_draw(&white_text, "HIGH", 0, 20, 1);
      text_draw(&gold_text, str, 96, 20, 1);
    }
  else
    {
      text_draw(&white_text,"Press ESC To Return",0,20,1);
    }

  if (timer_get_left(&time_left) > TIME_WARNING || (global_frame_counter % 10) < 5)
    {
      sprintf(str, "%d", timer_get_left(&time_left) / 1000 );
      text_draw(&white_text, "TIME", 224, 0, 1);
      text_draw(&gold_text, str, 304, 0, 1);
    }

  sprintf(str, "%d", distros);
  text_draw(&white_text, "DISTROS", screen->h, 0, 1);
  text_draw(&gold_text, str, 608, 0, 1);

  text_draw(&white_text, "LIVES", screen->h, 20, 1);

  if(show_fps)
    {
      sprintf(str, "%2.1f", fps_fps);
      text_draw(&white_text, "FPS", screen->h, 40, 1);
      text_draw(&gold_text, str, screen->h + 60, 40, 1);
    }

  for(int i=0; i < tux.lives; ++i)
    {
      texture_draw(&tux_life,565+(18*i),20);
    }
}


void drawendscreen(void)
{
  char str[80];

  clearscreen(0, 0, 0);

  text_drawf(&blue_text, "GAMEOVER", 0, 200, A_HMIDDLE, A_TOP, 1);

  sprintf(str, "SCORE: %d", score);
  text_drawf(&gold_text, str, 0, 224, A_HMIDDLE, A_TOP, 1);

  sprintf(str, "DISTROS: %d", distros);
  text_drawf(&gold_text, str, 0, 256, A_HMIDDLE, A_TOP, 1);

  flipscreen();
  SDL_Delay(2000);
}

void drawresultscreen(void)
{
  char str[80];

  clearscreen(0, 0, 0);

  text_drawf(&blue_text, "Result:", 0, 200, A_HMIDDLE, A_TOP, 1);

  sprintf(str, "SCORE: %d", score);
  text_drawf(&gold_text, str, 0, 224, A_HMIDDLE, A_TOP, 1);

  sprintf(str, "DISTROS: %d", distros);
  text_drawf(&gold_text, str, 0, 256, A_HMIDDLE, A_TOP, 1);

  flipscreen();
  SDL_Delay(2000);
}

void savegame(int slot)
{
  char savefile[1024];
  FILE* fi;
  unsigned int ui;

  sprintf(savefile,"%s/slot%d.save",st_save_dir,slot);

  fi = fopen(savefile, "wb");

  if (fi == NULL)
    {
      fprintf(stderr, "Warning: I could not open the slot file ");

    }
  else
    {
      fputs(level_subset, fi);
      fputs("\n", fi);
      fwrite(&level,sizeof(int),1,fi);
      fwrite(&score,sizeof(int),1,fi);
      fwrite(&distros,sizeof(int),1,fi);
      fwrite(&scroll_x,sizeof(float),1,fi);
      fwrite(&tux,sizeof(Player),1,fi);
      timer_fwrite(&tux.invincible_timer,fi);
      timer_fwrite(&tux.skidding_timer,fi);
      timer_fwrite(&tux.safe_timer,fi);
      timer_fwrite(&tux.frame_timer,fi);
      timer_fwrite(&time_left,fi);
      ui = st_get_ticks();
      fwrite(&ui,sizeof(int),1,fi);
    }
  fclose(fi);

}

void loadgame(int slot)
{
  char savefile[1024];
  char str[100];
  FILE* fi;
  unsigned int ui;

  sprintf(savefile,"%s/slot%d.save",st_save_dir,slot);

  fi = fopen(savefile, "rb");

  if (fi == NULL)
    {
      fprintf(stderr, "Warning: I could not open the slot file ");

    }
  else
    {
      fgets(str, 100, fi);
      strcpy(level_subset, str);
      level_subset[strlen(level_subset)-1] = '\0';
      fread(&level,sizeof(int),1,fi);

      set_defaults();
      level_free(&current_level);
      if(level_load(&current_level,level_subset,level) != 0)
        exit(1);
      arrays_free();
      arrays_init();
      activate_bad_guys();
      level_free_gfx();
      level_load_gfx(&current_level);
      level_free_song();
      level_load_song(&current_level);
      levelintro();
      update_time = st_get_ticks();

      fread(&score,sizeof(int),1,fi);
      fread(&distros,sizeof(int),1,fi);
      fread(&scroll_x,sizeof(float),1,fi);
      fread(&tux, sizeof(Player), 1, fi);
      timer_fread(&tux.invincible_timer,fi);
      timer_fread(&tux.skidding_timer,fi);
      timer_fread(&tux.safe_timer,fi);
      timer_fread(&tux.frame_timer,fi);
      timer_fread(&time_left,fi);
      fread(&ui,sizeof(int),1,fi);
      tux.hphysic.start_time += st_get_ticks() - ui;
      tux.vphysic.start_time += st_get_ticks() - ui;
      fclose(fi);
    }

}

void slotinfo(char **pinfo, int slot)
{
  FILE* fi;
  char slotfile[1024];
  char tmp[200];
  char str[5];
  int slot_level;
  sprintf(slotfile,"%s/slot%d.save",st_save_dir,slot);

  fi = fopen(slotfile, "rb");

  sprintf(tmp,"Slot %d - ",slot);

  if (fi == NULL)
    {
      strcat(tmp,"Free");
    }
  else
    {
      fgets(str, 100, fi);
      str[strlen(str)-1] = '\0';
      strcat(tmp, str);
      strcat(tmp, " / Level:");
      fread(&slot_level,sizeof(int),1,fi);
      sprintf(str,"%d",slot_level);
      strcat(tmp,str);
      fclose(fi);
    }

  *pinfo = (char*) malloc(sizeof(char) * (strlen(tmp)+1));
  strcpy(*pinfo,tmp);
}

