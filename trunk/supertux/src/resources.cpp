//  $Id$
//
//  SuperTux -  A Jump'n Run
//  Copyright (C) 2003 Tobias Glaesser <tobi.web@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "app/globals.h"
#include "scene.h"
#include "player.h"
#include "badguy.h"
#include "gameobjs.h"
#include "special.h"
#include "resources.h"
#include "special/sprite_manager.h"
#include "audio/sound_manager.h"
#include "app/setup.h"
#include "door.h"
#include "gui/button.h"

Surface* img_waves[3]; 
Surface* img_water;
Surface* img_pole;
Surface* img_poletop;
Surface* img_flag[2];
Surface* img_cloud[2][4];
Surface* img_distro[4];

Menu* main_menu      = 0;
Menu* game_menu      = 0;
Menu* options_menu   = 0;
Menu* options_keys_menu     = 0;
Menu* options_joystick_menu = 0;
Menu* highscore_menu = 0;
Menu* load_game_menu = 0;
Menu* save_game_menu = 0;
Menu* contrib_menu   = 0;
Menu* contrib_subset_menu   = 0;

MusicRef herring_song;
MusicRef level_end_song;

SpriteManager* sprite_manager = 0;
SoundManager* sound_manager = 0;

char * soundfilenames[NUM_SOUNDS] = {
                                       "/sounds/jump.wav",
                                       "/sounds/bigjump.wav",
                                       "/sounds/skid.wav",
                                       "/sounds/coin.wav",
                                       "/sounds/invincible.wav",
                                       "/sounds/brick.wav",
                                       "/sounds/hurt.wav",
                                       "/sounds/squish.wav",
                                       "/sounds/fall.wav",
                                       "/sounds/ricochet.wav",
                                       "/sounds/bump-upgrade.wav",
                                       "/sounds/upgrade.wav",
                                       "/sounds/grow.wav",
                                       "/sounds/fire-flower.wav",
                                       "/sounds/shoot.wav",
                                       "/sounds/lifeup.wav",
                                       "/sounds/stomp.wav",
                                       "/sounds/kick.wav",
                                       "/sounds/explosion.wav"
                                    };


Font* gold_text;
Font* blue_text;
Font* gray_text;
Font* yellow_nums;
Font* white_text;
Font* white_small_text;
Font* white_big_text;				    
				    
/* Load graphics/sounds shared between all levels: */
void loadshared()
{

  /* Load global images: */
  gold_text = new Font(datadir + "/images/fonts/gold.png", Font::TEXT, 16,18);
  blue_text = new Font(datadir + "/images/fonts/blue.png", Font::TEXT, 16,18,3);
  white_text  = new Font(datadir + "/images/fonts/white.png",
                         Font::TEXT, 16,18);
  gray_text  = new Font(datadir + "/images/fonts/gray.png",
                        Font::TEXT, 16,18);
  white_small_text = new Font(datadir + "/images/fonts/white-small.png",
                              Font::TEXT, 8,9, 1);
  white_big_text   = new Font(datadir + "/images/fonts/white-big.png",
                              Font::TEXT, 20,22, 3);
  yellow_nums = new Font(datadir + "/images/fonts/numbers.png",
                         Font::NUM, 32,32);

  Menu::default_font = white_text;
  Menu::active_font = blue_text;
  Menu::deactive_font = gray_text;
  Menu::label_font = white_big_text;
  Menu::field_font = gold_text;
  
  Button::info_font = white_small_text;

  int i;

  sprite_manager = new SpriteManager(datadir + "/supertux.strf");

  /* Tuxes: */
  smalltux_star = sprite_manager->load("smalltux-star");
  largetux_star = sprite_manager->load("largetux-star");
  smalltux_gameover = sprite_manager->load("smalltux-gameover");

  char img_name[1024];
  for (int i = 0; i < GROWING_FRAMES; i++)
    {
      sprintf(img_name, "%s/images/shared/tux-grow-left-%i.png", datadir.c_str(), i+1);
      growingtux_left[i] = new Surface(img_name, true);

      sprintf(img_name, "%s/images/shared/tux-grow-right-%i.png", datadir.c_str(), i+1);
      growingtux_right[i] = new Surface(img_name, true);
    }

  smalltux.stand_left  = sprite_manager->load("smalltux-stand-left");
  smalltux.stand_right = sprite_manager->load("smalltux-stand-right");
  smalltux.walk_left   = sprite_manager->load("smalltux-walk-left");
  smalltux.walk_right  = sprite_manager->load("smalltux-walk-right");
  smalltux.jump_left   = sprite_manager->load("smalltux-jump-left");
  smalltux.jump_right  = sprite_manager->load("smalltux-jump-right");
  smalltux.kick_left   = sprite_manager->load("smalltux-kick-left");
  smalltux.kick_right  = sprite_manager->load("smalltux-kick-right");
  smalltux.skid_left   = sprite_manager->load("smalltux-skid-left");
  smalltux.skid_right  = sprite_manager->load("smalltux-skid-right");
  smalltux.grab_left   = sprite_manager->load("smalltux-grab-left");
  smalltux.grab_right  = sprite_manager->load("smalltux-grab-right");

  largetux.stand_left  = sprite_manager->load("largetux-stand-left");
  largetux.stand_right = sprite_manager->load("largetux-stand-right");
  largetux.walk_left   = sprite_manager->load("largetux-walk-left");
  largetux.walk_right  = sprite_manager->load("largetux-walk-right");
  largetux.jump_left   = sprite_manager->load("largetux-jump-left");
  largetux.jump_right  = sprite_manager->load("largetux-jump-right");
  largetux.kick_left   = sprite_manager->load("largetux-kick-left");
  largetux.kick_right  = sprite_manager->load("largetux-kick-right");
  largetux.skid_right  = sprite_manager->load("largetux-skid-right");
  largetux.skid_left   = sprite_manager->load("largetux-skid-left");
  largetux.grab_left   = sprite_manager->load("largetux-grab-left");
  largetux.grab_right  = sprite_manager->load("largetux-grab-right");
  largetux.duck_left   = sprite_manager->load("largetux-duck-left");
  largetux.duck_right  = sprite_manager->load("largetux-duck-right");

  firetux.stand_left  = sprite_manager->load("firetux-stand-left");
  firetux.stand_right = sprite_manager->load("firetux-stand-right");
  firetux.walk_left   = sprite_manager->load("firetux-walk-left");
  firetux.walk_right  = sprite_manager->load("firetux-walk-right");
  firetux.jump_left   = sprite_manager->load("firetux-jump-left");
  firetux.jump_right  = sprite_manager->load("firetux-jump-right");
  firetux.kick_left   = sprite_manager->load("firetux-kick-left");
  firetux.kick_right  = sprite_manager->load("firetux-kick-right");
  firetux.skid_right  = sprite_manager->load("firetux-skid-right");
  firetux.skid_left   = sprite_manager->load("firetux-skid-left");
  firetux.grab_left   = sprite_manager->load("firetux-grab-left");
  firetux.grab_right  = sprite_manager->load("firetux-grab-right");
  firetux.duck_left   = sprite_manager->load("firetux-duck-left");
  firetux.duck_right  = sprite_manager->load("firetux-duck-right");

  icetux.stand_left  = sprite_manager->load("icetux-stand-left");
  icetux.stand_right = sprite_manager->load("icetux-stand-right");
  icetux.walk_left   = sprite_manager->load("icetux-walk-left");
  icetux.walk_right  = sprite_manager->load("icetux-walk-right");
  icetux.jump_left   = sprite_manager->load("icetux-jump-left");
  icetux.jump_right  = sprite_manager->load("icetux-jump-right");
  icetux.kick_left   = sprite_manager->load("icetux-kick-left");
  icetux.kick_right  = sprite_manager->load("icetux-kick-right");
  icetux.skid_right  = sprite_manager->load("icetux-skid-right");
  icetux.skid_left   = sprite_manager->load("icetux-skid-left");
  icetux.grab_left   = sprite_manager->load("icetux-grab-left");
  icetux.grab_right  = sprite_manager->load("icetux-grab-right");
  icetux.duck_left   = sprite_manager->load("icetux-duck-left");
  icetux.duck_right  = sprite_manager->load("icetux-duck-right");

  /* Water: */
  img_water = new Surface(datadir + "/images/shared/water.png", false);

  img_waves[0] = new Surface(datadir + "/images/shared/waves-0.png",
                             true);

  img_waves[1] = new Surface(datadir + "/images/shared/waves-1.png",
                             true);

  img_waves[2] = new Surface(datadir + "/images/shared/waves-2.png",
                             true);


  /* Pole: */

  img_pole = new Surface(datadir + "/images/shared/pole.png", true);
  img_poletop = new Surface(datadir + "/images/shared/poletop.png",
                            true);


  /* Flag: */

  img_flag[0] = new Surface(datadir + "/images/shared/flag-0.png",
                            true);
  img_flag[1] = new Surface(datadir + "/images/shared/flag-1.png",
                            true);


  /* Cloud: */

  img_cloud[0][0] = new Surface(datadir + "/images/shared/cloud-00.png",
                                true);

  img_cloud[0][1] = new Surface(datadir + "/images/shared/cloud-01.png",
                                true);

  img_cloud[0][2] = new Surface(datadir + "/images/shared/cloud-02.png",
                                true);

  img_cloud[0][3] = new Surface(datadir + "/images/shared/cloud-03.png",
                                true);


  img_cloud[1][0] = new Surface(datadir + "/images/shared/cloud-10.png",
                                true);

  img_cloud[1][1] = new Surface(datadir + "/images/shared/cloud-11.png",
                                true);

  img_cloud[1][2] = new Surface(datadir + "/images/shared/cloud-12.png",
                                true);

  img_cloud[1][3] = new Surface(datadir + "/images/shared/cloud-13.png",
                                true);


  /* Bad guys: */
  load_badguy_gfx();

  /* Upgrades: */
  load_special_gfx();

  /* Objects */
  load_object_gfx();

  // load the door object graphics:
  door = sprite_manager->load("door");
  for (int i = 0; i < DOOR_OPENING_FRAMES; i++)
    {
      sprintf(img_name, "%s/images/shared/door-%i.png", datadir.c_str(), i+1);
      door_opening[i] = new Surface(img_name, true);
    }

  /* Distros: */
  img_distro[0] = new Surface(datadir + "/images/tilesets/coin1.png",
                              true);

  img_distro[1] = new Surface(datadir + "/images/tilesets/coin2.png",
                              true);

  img_distro[2] = new Surface(datadir + "/images/tilesets/coin3.png",
                              true);

  img_distro[3] = new Surface(datadir + "/images/tilesets/coin2.png",
                              true);


  /* Tux life: */

  tux_life = new Surface(datadir + "/images/shared/tux-life.png",
                         true);

  /* Sound effects: */

  /* if (use_sound) // this will introduce SERIOUS bugs here ! because "load_sound"
                    // initialize sounds[i] with the correct pointer's value:
                    // NULL or something else. And it will be dangerous to
                    // play with not-initialized pointers.
                    // This is also true with if (use_music)
                    Send a mail to me: neoneurone@users.sf.net, if you have another opinion. :)
  */
  for (i = 0; i < NUM_SOUNDS; i++)
    SoundManager::get
      ()->add_sound(SoundManager::get
                      ()->load_sound(datadir + soundfilenames[i]),i);

  /* Herring song */
  herring_song = SoundManager::get
                   ()->load_music(datadir + "/music/SALCON.MOD");
  level_end_song = SoundManager::get
                     ()->load_music(datadir + "/music/leveldone.mod");
}

/* Free shared data: */
void unloadshared(void)
{
  /* Free global images: */
  delete gold_text;
  delete white_text;
  delete blue_text;
  delete gray_text;
  delete white_small_text;
  delete white_big_text;
  delete yellow_nums;
  
  int i;

  free_special_gfx();
  free_badguy_gfx();

  delete img_water;
  for (i = 0; i < 3; i++)
    delete img_waves[i];

  delete img_pole;
  delete img_poletop;

  for (i = 0; i < 2; i++)
    delete img_flag[i];

  for (i = 0; i < 4; i++)
    {
      delete img_distro[i];
      delete img_cloud[0][i];
      delete img_cloud[1][i];
    }

  delete tux_life;

  for (int i = 0; i < GROWING_FRAMES; i++)
  {
    delete growingtux_left[i];
    delete growingtux_right[i];
  }

  // door game object:

  for (int i = 0; i < DOOR_OPENING_FRAMES; i++)
    delete door_opening[i];

  delete sprite_manager;
  sprite_manager = 0;
  sound_manager = 0;
}

/* EOF */
