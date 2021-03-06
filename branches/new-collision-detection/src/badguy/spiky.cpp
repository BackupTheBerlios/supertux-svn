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

#include "spiky.hpp"

static const float WALKSPEED = 80;

Spiky::Spiky(const lisp::Lisp& reader)
	: BadGuy(reader, "images/creatures/spiky/spiky.sprite")
{
}

void
Spiky::write(lisp::Writer& writer)
{
  writer.start_list("spiky");

  writer.write_float("x", start_position.x);
  writer.write_float("y", start_position.y);

  writer.end_list("spiky");
}

void
Spiky::activate()
{
  physic.set_velocity_x(dir == LEFT ? -WALKSPEED : WALKSPEED);
  sprite->set_action(dir == LEFT ? "left" : "right");
}

void
Spiky::active_update(float elapsed_time)
{
  BadGuy::active_update(elapsed_time);

  if (might_fall(601))
  {
    dir = (dir == LEFT ? RIGHT : LEFT);
    sprite->set_action(dir == LEFT ? "left" : "right");
    physic.set_velocity_x(-physic.get_velocity_x());
  }
}

void
Spiky::collision_solid(const CollisionHit& hit)
{
  if(hit.top || hit.bottom) { // hit floor or roof?
    physic.set_velocity_y(0);
  } else { // hit right or left
    dir = dir == LEFT ? RIGHT : LEFT;
    sprite->set_action(dir == LEFT ? "left" : "right");
    physic.set_velocity_x(-physic.get_velocity_x());
  }
}

HitResponse
Spiky::collision_badguy(BadGuy& , const CollisionHit& hit)
{
  if(hit.left || hit.right) {
    dir = dir == LEFT ? RIGHT : LEFT;
    sprite->set_action(dir == LEFT ? "left" : "right");
    physic.set_velocity_x(-physic.get_velocity_x());
  }

  return CONTINUE;
}

IMPLEMENT_FACTORY(Spiky, "spiky")
