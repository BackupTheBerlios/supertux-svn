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

#ifndef SUPERTUX_PLAYER_H
#define SUPERTUX_PLAYER_H

#include <SDL.h>
#include "bitmask.h"
#include "type.h"
#include "timer.h"
#include "texture.h"
#include "collision.h"
#include "sound.h"
#include "physic.h"

/* Times: */

#define TUX_SAFE_TIME 750
#define TUX_INVINCIBLE_TIME 10000
#define TIME_WARNING 20000     /* When to alert player they're low on time! */

/* One-ups... */

#define DISTROS_LIFEUP 100

/* Scores: */

#define SCORE_BRICK 5
#define SCORE_DISTRO 25

#include <vector>

struct PlayerKeymap
{
public:
  int jump;
  int duck;
  int left;
  int right;
  int fire;
  
  PlayerKeymap();
};

extern PlayerKeymap keymap;

struct player_input_type
{
  int right;
  int left;
  int up;
  int down;
  int fire;
  int old_fire;
};

void player_input_init(player_input_type* pplayer_input);

class Sprite;

extern Surface* tux_life;

extern Sprite* smalltux_gameover;
extern Sprite* smalltux_star;
extern Sprite* largetux_star;

struct PlayerSprite
{
  Sprite* stand_left;
  Sprite* stand_right;
  Sprite* walk_right;
  Sprite* walk_left;
  Sprite* jump_right;
  Sprite* jump_left;
  Sprite* kick_left;
  Sprite* kick_right;
  Sprite* skid_right;
  Sprite* skid_left;
  Sprite* grab_left;
  Sprite* grab_right;
  Sprite* duck_right;
  Sprite* duck_left;
};

extern PlayerSprite smalltux;
extern PlayerSprite largetux;
extern PlayerSprite firetux;

class Player
{
public:
  enum HurtMode { KILL, SHRINK };

  player_input_type  input;
  bool got_coffee;
  int size;
  bool duck;
  bool holding_something;
  DyingType dying;

  Direction dir;

  bool jumping;
  int frame_;
  int frame_main;

  base_type  base;
  base_type  old_base;
  base_type  previous_base;
  Timer invincible_timer;
  Timer skidding_timer;
  Timer safe_timer;
  Timer frame_timer;
  Timer kick_timer;
  Physic physic;

public:
  void init();
  int  key_event(SDLKey key, int state);
  void level_begin();
  void action(double frame_ratio);
  void handle_input();
  void grabdistros();
  void draw();
  void collision(void* p_c_object, int c_object);
  void kill(HurtMode mode);
  void is_dying();
  bool is_dead();
  void player_remove_powerups();
  void keep_in_bounds();
  bool on_ground();
  bool under_solid();
  void grow();
  
private:
  void handle_horizontal_input();
  void handle_vertical_input();
  void remove_powerups();
};

#endif /*SUPERTUX_PLAYER_H*/

/* Local Variables: */
/* mode:c++ */
/* End: */
