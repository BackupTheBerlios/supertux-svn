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

#include "SDL.h"

#include "bitmask.h"
#include "special/timer.h"
#include "special/base.h"
#include "video/surface.h"
#include "collision.h"
#include "special/moving_object.h"
#include "math/physic.h"
#include "defines.h"

using namespace SuperTux;

class BadGuy;

/* Times: */

#define TUX_SAFE_TIME 1250
#define TUX_INVINCIBLE_TIME 10000
#define TUX_INVINCIBLE_TIME_WARNING 2000
#define TUX_FLAPPING_TIME 1000 /* How long Tux can flap his wings to gain additional jump height */
#define TIME_WARNING 20000     /* When to alert player they're low on time! */

/* One-ups... */

#define DISTROS_LIFEUP 100

/* Scores: */

#define SCORE_BRICK 5
#define SCORE_DISTRO 20

/* Sizes: */

#define SMALL 0
#define BIG 1

#include <vector>

struct PlayerKeymap
{
public:
  int jump;
  int activate;
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
  int old_up;
  int down;
  int fire;
  int old_fire;
  int activate;
};

void player_input_init(player_input_type* pplayer_input);

namespace SuperTux {
class Sprite;
}
class Camera;

extern Surface* tux_life;

extern Sprite* smalltux_gameover;
extern Sprite* smalltux_star;
extern Sprite* bigtux_star;

#define GROWING_TIME 1000
#define GROWING_FRAMES 7
extern Surface* growingtux_left[GROWING_FRAMES];
extern Surface* growingtux_right[GROWING_FRAMES];

class TuxBodyParts
{
public:
  TuxBodyParts() { };
  ~TuxBodyParts() { };

  void set_action(std::string action);
  void one_time_animation();
  void draw(DrawingContext& context, const Vector& pos, int layer,
                Uint32 drawing_effect = NONE_EFFECT);

  Sprite* head;
  Sprite* body;
  Sprite* arms;
  Sprite* feet;
};

extern TuxBodyParts* small_tux;
extern TuxBodyParts* big_tux;
extern TuxBodyParts* fire_tux;
extern TuxBodyParts* ice_tux;

class Player : public MovingObject
{
public:
  enum HurtMode { KILL, SHRINK };
  enum Power { NONE_POWER, FIRE_POWER, ICE_POWER };
  enum FallMode { ON_GROUND, JUMPING, TRAMPOLINE_JUMP, FALLING };

  player_input_type  input;
  int got_power;
  int size;
  bool duck;
  bool holding_something;
  bool dead;
  DyingType dying;

  Direction dir;
  Direction old_dir;

  float last_ground_y;
  FallMode fall_mode;

  bool jumping;
  bool flapping;
  bool can_jump;
  bool can_flap;
  bool enable_hover;
  bool butt_jump;
  int frame_;
  int frame_main;

  base_type  previous_base;
  Timer invincible_timer;
  Timer skidding_timer;
  Timer safe_timer;
  Timer frame_timer;
  Timer kick_timer;
  Timer shooting_timer;   // used to show the arm when Tux is shooting
  Timer dying_timer;
  Timer growing_timer;
  Timer idle_timer;
  Timer flapping_timer;
  Physic physic;
  
public:
  Player();
  virtual ~Player();
  
  int  key_event(SDLKey key, int state);
  void level_begin();
  void handle_input();
  void grabdistros();

  virtual void action(float elapsed_time);
  virtual void draw(DrawingContext& context);
  virtual void collision(const MovingObject& other_object,
      int collision_type);

  void collision(void* p_c_object, int c_object);
  void kill(HurtMode mode);
  void player_remove_powerups();
  void check_bounds(Camera* camera);
  bool on_ground();
  bool under_solid();
  bool tiles_on_air(int tiles);
  void grow(bool animate);
  void move(const Vector& vector);

  /** let the player jump a bit or more if jump button is hold down
      (used when you hit a badguy) */
  void bounce(BadGuy* badguy);

  bool is_dead() const
  { return dead; }
  
private:
  void init();
  
  void handle_horizontal_input();
  void handle_vertical_input();
  void remove_powerups();
};

#endif /*SUPERTUX_PLAYER_H*/

/* Local Variables: */
/* mode:c++ */
/* End: */
