//  $Id$
//
//  SuperTux - Mole Badguy
//  Copyright (C) 2006 Christoph Sommer <christoph.sommer@2006.expires.deltadevelopment.de>
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

#include <config.h>

#include "mole.hpp"
#include "mole_rock.hpp"
#include "tile.hpp"
#include "object/tilemap.hpp"
#include "random_generator.hpp"
#include "log.hpp"
#include "level.hpp"

static const float IDLE_TIME = 0.2; /**< time to wait before and after throwing */
static const float THROW_TIME = 4.6; /**< time to spend throwing */
static const float THROW_INTERVAL = 1; /**< time between two thrown rocks */
static const float THROW_VELOCITY = 400; /**< initial velocity of thrown rocks */

Mole::Mole(const lisp::Lisp& reader)
	: BadGuy(reader, "images/creatures/mole/mole.sprite", LAYER_TILES-1), state(PRE_THROWING)
{
  physic.enable_gravity(false);
}

Mole::Mole(const Vector& pos)
	: BadGuy(pos, "images/creatures/mole/mole.sprite", LAYER_TILES-1), state(PRE_THROWING)
{
  physic.enable_gravity(false);
}

void
Mole::write(lisp::Writer& writer)
{
  writer.start_list("mole");
  writer.write_float("x", start_position.x);
  writer.write_float("y", start_position.y);
  writer.end_list("mole");
}

void
Mole::activate()
{
  if (state != DEAD) set_state(PRE_THROWING);
}

void
Mole::kill_fall()
{
  set_state(DEAD);
  sound_manager->play("sounds/fall.wav", get_pos());
  if (countMe) Sector::current()->get_level()->stats.badguys++;
}

HitResponse
Mole::collision_badguy(BadGuy& , const CollisionHit& )
{
  return FORCE_MOVE;
}

bool
Mole::collision_squished(GameObject& )
{
  set_state(DEAD);
  sound_manager->play("sounds/squish.wav", get_pos());
  if (countMe) Sector::current()->get_level()->stats.badguys++;
  return true;
}

void
Mole::throw_rock()
{
  float px = get_bbox().get_middle().x;
  float py = get_bbox().get_middle().y;

  float angle = systemRandom.rand(90 - 15, 90 + 15) * (M_PI / 180);
  float vx = cos(angle) * THROW_VELOCITY;
  float vy = -sin(angle) * THROW_VELOCITY;

  sound_manager->play("sounds/dartfire.wav", get_pos());
  Sector::current()->add_object(new MoleRock(Vector(px, py), Vector(vx, vy), this));
}

void
Mole::active_update(float elapsed_time)
{
  BadGuy::active_update(elapsed_time);

  switch (state) {
    case PRE_THROWING:
      if (timer.check()) {
	set_state(THROWING);
      }
      break;
    case THROWING:
      if (throw_timer.check()) {
        throw_rock();
	throw_timer.start(THROW_INTERVAL);
      }
      if (timer.check()) {
	set_state(POST_THROWING);
      }
      break;
    case POST_THROWING:
      if (timer.check()) {
	set_state(PEEKING);
      }
      break;
    case PEEKING:
      if (sprite->animation_done()) {
	set_state(PRE_THROWING);
      }
      break;
    case DEAD:
      break;
  }

}

void 
Mole::set_state(MoleState new_state)
{
  switch (new_state) {
    case PRE_THROWING:
      sprite->set_action("idle");
      set_group(COLGROUP_DISABLED);
      timer.start(IDLE_TIME);
      break;
    case THROWING:
      sprite->set_action("idle");
      set_group(COLGROUP_DISABLED);
      timer.start(THROW_TIME);
      throw_timer.start(THROW_INTERVAL);
      break;
    case POST_THROWING:
      sprite->set_action("idle");
      set_group(COLGROUP_DISABLED);
      timer.start(IDLE_TIME);
      break;
    case PEEKING:
      sprite->set_action("peeking", 1);
      set_group(COLGROUP_STATIC);
      break;
    case DEAD:
      sprite->set_action("idle");
      set_group(COLGROUP_DISABLED);
      break;
  }

  state = new_state;
}

IMPLEMENT_FACTORY(Mole, "mole")

