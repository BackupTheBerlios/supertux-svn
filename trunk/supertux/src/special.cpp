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

#include <cassert>
#include <iostream>

#include "SDL.h"

#include "app/defines.h"
#include "special.h"
#include "camera.h"
#include "gameloop.h"
#include "video/screen.h"
#include "audio/sound.h"
#include "scene.h"
#include "app/globals.h"
#include "player.h"
#include "sector.h"
#include "special/sprite_manager.h"
#include "resources.h"

Sprite* img_firebullet;
Sprite* img_icebullet;
Sprite* img_star;
Sprite* img_growup;
Sprite* img_iceflower;
Sprite* img_fireflower;
Sprite* img_1up;

#define GROWUP_SPEED 1.0f

#define BULLET_STARTING_YM 0
#define BULLET_XM 6

Bullet::Bullet(const Vector& pos, float xm, int dir, int kind_)
{
  life_count = 3;
  base.width = 4;
  base.height = 4;
  
  if (kind == ICE_BULLET)
    life_count = 6; //ice-bullets get "extra lives" for bumping off walls

  if (dir == RIGHT)
    {
      base.x = pos.x + 32;
      physic.set_velocity_x(BULLET_XM + xm);
    }
  else
    {
      base.x = pos.x;
      physic.set_velocity_x(-BULLET_XM + xm);
    }

  base.y = pos.y;
  physic.set_velocity_y(-BULLET_STARTING_YM);
  old_base = base;
  kind = kind_;
}

void
Bullet::action(float elapsed_time)
{
  elapsed_time *= 0.5f;

  float old_y = base.y;

  physic.apply(elapsed_time, base.x, base.y,  Sector::current()->gravity);
  collision_swept_object_map(&old_base,&base);
      
  if (issolid(base.x+2, base.y + 4) || issolid(base.x+2, base.y))
    {
      base.y  = old_y;
      physic.set_velocity_y(-physic.get_velocity_y());
      life_count -= 1;
    }

  if(kind == FIRE_BULLET)
    // @not framerate independant :-/
    physic.set_velocity_y(physic.get_velocity_y() - 0.5 * elapsed_time);
  if(physic.get_velocity_y() > 9)
    physic.set_velocity_y(9);
  else if(physic.get_velocity_y() < -9)
    physic.set_velocity_y(-9);

  float scroll_x =
    Sector::current()->camera->get_translation().x;
  float scroll_y =
    Sector::current()->camera->get_translation().y;
  if (base.x < scroll_x ||
      base.x > scroll_x + screen->w ||
      base.y < scroll_y ||
      base.y > scroll_y + screen->h ||
      life_count <= 0)
    {
      remove_me();
    }
  if (issolid(base.x + 4, base.y + 2) || 
      issolid(base.x, base.y + 2))
     {
       if (kind == FIRE_BULLET)
         remove_me();
       else if (kind == ICE_BULLET)
         {
	   physic.set_velocity_x(-physic.get_velocity_x());
	   //physic.set_velocity_y(-physic.get_velocity_y());
	 }
     }
}

void 
Bullet::draw(DrawingContext& context)
{
  Sprite* sprite = kind == FIRE_BULLET ? img_firebullet : img_icebullet;
 
  sprite->draw(context, Vector(base.x, base.y), LAYER_OBJECTS);
}

void
Bullet::collision(const MovingObject& , int)
{
  // later
}

void
Bullet::collision(int c_object)
{
  if(c_object == CO_BADGUY) {
    remove_me();
  }
}

//---------------------------------------------------------------------------

Upgrade::Upgrade(const Vector& pos, Direction dir_, UpgradeKind kind_)
{
  kind = kind_;
  dir = dir_;

  base.width = 32;
  base.height = 0;
  base.x = pos.x;
  base.y = pos.y;
  old_base = base;

  physic.reset();
  physic.enable_gravity(false);

  if(kind == UPGRADE_1UP || kind == UPGRADE_HERRING) {
    physic.set_velocity(dir == LEFT ? -1 : 1, 4);
    physic.enable_gravity(true);
    base.height = 32;
  } else if (kind == UPGRADE_ICEFLOWER || kind == UPGRADE_FIREFLOWER) {
    // nothing
  } else if (kind == UPGRADE_GROWUP) {
    physic.set_velocity(dir == LEFT ? -GROWUP_SPEED : GROWUP_SPEED, 0);
  } else {
    physic.set_velocity(dir == LEFT ? -2 : 2, 0);
  }
}

Upgrade::~Upgrade()
{
}

void
Upgrade::action(float elapsed_time)
{
  if (kind == UPGRADE_ICEFLOWER || kind == UPGRADE_FIREFLOWER
      || kind == UPGRADE_GROWUP) {
    if (base.height < 32) {
      /* Rise up! */
      base.height = base.height + 0.7 * elapsed_time;
      if(base.height > 32)
        base.height = 32;

      return;
    }
  }

  /* Away from the screen? Kill it! */
  float scroll_x =
    Sector::current()->camera->get_translation().x;
  float scroll_y =                                                        
    Sector::current()->camera->get_translation().y;
  
  if(base.x < scroll_x - X_OFFSCREEN_DISTANCE ||
      base.x > scroll_x + screen->w + X_OFFSCREEN_DISTANCE ||
      base.y < scroll_y - Y_OFFSCREEN_DISTANCE ||
      base.y > scroll_y + screen->h + Y_OFFSCREEN_DISTANCE)
    {
    remove_me();
    return;
    }

  /* Move around? */
  physic.apply(elapsed_time, base.x, base.y, Sector::current()->gravity);
  if(kind == UPGRADE_GROWUP) {
    collision_swept_object_map(&old_base, &base);
  }

  // fall down?
  if(kind == UPGRADE_GROWUP || kind == UPGRADE_HERRING) {
    // falling?
    if(physic.get_velocity_y() != 0) {
      if(issolid(base.x, base.y + base.height)) {
        base.y = int(base.y / 32) * 32;
        old_base = base;                         
        if(kind == UPGRADE_GROWUP) {
          physic.enable_gravity(false);
          physic.set_velocity(dir == LEFT ? -GROWUP_SPEED : GROWUP_SPEED, 0);
        } else if(kind == UPGRADE_HERRING) {
          physic.set_velocity(dir == LEFT ? -2 : 2, 3);
        }
      }
    } else {
      if((physic.get_velocity_x() < 0
            && !issolid(base.x+base.width, base.y + base.height))
        || (physic.get_velocity_x() > 0
            && !issolid(base.x, base.y + base.height))) {
        physic.enable_gravity(true);
      }
    }
  }

  // horizontal bounce?
  if(kind == UPGRADE_GROWUP || kind == UPGRADE_HERRING) {
    if (  (physic.get_velocity_x() < 0
          && issolid(base.x, (int) base.y + base.height/2)) 
        ||  (physic.get_velocity_x() > 0
          && issolid(base.x + base.width, (int) base.y + base.height/2))) {
        physic.set_velocity(-physic.get_velocity_x(),physic.get_velocity_y());
        dir = dir == LEFT ? RIGHT : LEFT;
    }
  }
}

void
Upgrade::draw(DrawingContext& context)
{
  Sprite* sprite;
  switch(kind) {
    case UPGRADE_GROWUP: sprite = img_growup; break;
    case UPGRADE_ICEFLOWER: sprite = img_iceflower; break;
    case UPGRADE_FIREFLOWER: sprite = img_fireflower; break;
    case UPGRADE_HERRING: sprite = img_star; break;
    case UPGRADE_1UP: sprite = img_1up; break;
    default:
      assert(!"wrong type in Powerup::draw()");
      sprite = NULL;		// added by neoneurone, g++ likes this !
  }

  if(base.height < 32) // still raising up?
    sprite->draw(context, Vector(base.x, base.y + (32 - base.height)),
        LAYER_TILES - 10);
  else
    sprite->draw(context, Vector(base.x, base.y), LAYER_OBJECTS);
}

void
Upgrade::bump(Player* player)
{
  // these can't be bumped
  if(kind != UPGRADE_GROWUP)
    return;

  sound_manager->play_sound(sounds[SND_BUMP_UPGRADE], Vector(base.x, base.y), Sector::current()->player->get_pos());
  
  // determine new direction
  Direction old_dir = dir;
  if (player->base.x + player->base.width/2 > base.x + base.width/2)
    dir = LEFT;
  else
    dir = RIGHT;

  // do a little jump and change direction (if necessary)
  if (dir != old_dir)
    physic.set_velocity(-physic.get_velocity_x(), 3);
  else
    physic.set_velocity_y(3);

  physic.enable_gravity(true);
}

void
Upgrade::collision(const MovingObject& , int)
{
  // later
}

void
Upgrade::collision(void* p_c_object, int c_object, CollisionType type)
{
  Player* pplayer = NULL;

  if(type == COLLISION_BUMP) {
    if(c_object == CO_PLAYER)
      pplayer = (Player*) p_c_object;
    bump(pplayer);
    return;
  }

  switch (c_object)
    {
    case CO_PLAYER:
      /* Remove the upgrade: */

      /* p_c_object is CO_PLAYER, so assign it to pplayer */
      pplayer = (Player*) p_c_object;

      /* Affect the player: */

      if (kind == UPGRADE_GROWUP)
        {
          sound_manager->play_sound(sounds[SND_EXCELLENT]);
          pplayer->grow(true);
        }
      else if (kind == UPGRADE_FIREFLOWER)
        {
          sound_manager->play_sound(sounds[SND_COFFEE]);
          pplayer->grow(true);
          pplayer->got_power = pplayer->FIRE_POWER;
        }
      else if (kind == UPGRADE_ICEFLOWER)
        {
          sound_manager->play_sound(sounds[SND_COFFEE]);
          pplayer->grow(true);
          pplayer->got_power = pplayer->ICE_POWER;
        }
      else if (kind == UPGRADE_FIREFLOWER)
        {
          sound_manager->play_sound(sounds[SND_COFFEE]);
          pplayer->grow(true);
          pplayer->got_power = pplayer->FIRE_POWER;
        }
      else if (kind == UPGRADE_HERRING)
        {
          sound_manager->play_sound(sounds[SND_HERRING]);
          pplayer->invincible_timer.start(TUX_INVINCIBLE_TIME);
          Sector::current()->play_music(HERRING_MUSIC);
        }
      else if (kind == UPGRADE_1UP)
        {
          if(player_status.lives < MAX_LIVES) {
            player_status.lives++;
            sound_manager->play_sound(sounds[SND_LIFEUP]);
          }
        }

      remove_me();
      return;
    }
}

void load_special_gfx()
{
  img_growup    = sprite_manager->load("egg");
  img_iceflower = sprite_manager->load("iceflower");
  img_fireflower = sprite_manager->load("fireflower");
  img_star      = sprite_manager->load("star");
  img_1up       = sprite_manager->load("1up");

  img_firebullet = sprite_manager->load("firebullet");
  img_icebullet  = sprite_manager->load("icebullet");
}

void free_special_gfx()
{
}

