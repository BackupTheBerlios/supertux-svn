//  $Id$
//
//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
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

#include "fish.hpp"
#include "tile.hpp"
#include "object/tilemap.hpp"

static const float FISH_JUMP_POWER = 600;
static const float FISH_WAIT_TIME = 1;

Fish::Fish(const lisp::Lisp& reader)
{
  reader.get("x", start_position.x);
  reader.get("y", start_position.y);
  bbox.set_size(31.8, 31.8);
  sprite = sprite_manager->create("images/creatures/fish/fish.sprite");
  physic.enable_gravity(true);
}

Fish::Fish(float pos_x, float pos_y)
{
  start_position.x = pos_x;
  start_position.y = pos_y;
  bbox.set_size(31.8, 31.8);
  sprite = sprite_manager->create("images/creatures/fish/fish.sprite");
  physic.enable_gravity(true);
}

void
Fish::write(lisp::Writer& writer)
{
  writer.start_list("fish");

  writer.write_float("x", start_position.x);
  writer.write_float("y", start_position.y);

  writer.end_list("fish");
}

HitResponse
Fish::collision_solid(GameObject& , const CollisionHit& chit)
{
  return hit(chit);
}

HitResponse
Fish::collision_badguy(BadGuy& , const CollisionHit& chit)
{
  return hit(chit);
}

void
Fish::draw(DrawingContext& context)
{
  if(waiting.started())
    return;

  BadGuy::draw(context);
}

HitResponse
Fish::hit(const CollisionHit& chit)
{
  if(chit.normal.y < .5) { // hit ceiling
    physic.set_velocity_y(0);
  }

  return CONTINUE;
}

void
Fish::collision_tile(uint32_t tile_attributes)
{
  if(tile_attributes & Tile::WATER) {
    start_waiting();
    movement = Vector(0, 0);
  }
}

void
Fish::active_update(float elapsed_time)
{
  BadGuy::active_update(elapsed_time);

  // waited long enough?
  if(waiting.check()) {
    jump();
  }
  
  // set sprite
  sprite->set_action(physic.get_velocity_y() > 0 ? "normal" : "down");
  
  // we can't afford flying out of the tilemap, 'cause the engine would remove us.
  if ((get_pos().y - 31.8) < 0) // too high, let us fall
  {
    physic.set_velocity_y(0);
    physic.enable_gravity(true);
  }
}

void
Fish::start_waiting()
{
  waiting.start(FISH_WAIT_TIME);
  set_group(COLGROUP_DISABLED);
  physic.enable_gravity(false);
  physic.set_velocity_y(0);
}

void
Fish::jump()
{
  physic.set_velocity_y(FISH_JUMP_POWER);
  physic.enable_gravity(true);
  set_group(COLGROUP_MOVING);
}

IMPLEMENT_FACTORY(Fish, "fish")
