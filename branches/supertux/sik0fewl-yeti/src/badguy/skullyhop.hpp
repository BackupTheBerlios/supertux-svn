//  $Id$
//
//  SkullyHop - A Hopping Skull
//  Copyright (C) 2006 Christoph Sommer <supertux@2006.expires.deltadevelopment.de>
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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.

#ifndef __SKULLYHOP_H__
#define __SKULLYHOP_H__

#include "badguy.hpp"

/**
 * Badguy "SkullyHop" - A Hopping Skull
 */
class SkullyHop : public BadGuy
{
public:
  SkullyHop(const lisp::Lisp& reader);
  SkullyHop(float pos_x, float pos_y, Direction d);

  void activate();
  void write(lisp::Writer& writer);
  HitResponse collision_solid(GameObject& other, const CollisionHit& hit);
  HitResponse collision_badguy(BadGuy& badguy, const CollisionHit& hit);
  bool collision_squished(Player& player);
  void active_update(float elapsed_time);

protected:
  enum SkullyHopState {
    STANDING,
    CHARGING,
    JUMPING
  };

  bool has_initial_direction;
  Direction initial_direction;  

  Timer recover_timer;
  SkullyHopState state;

  void set_state(SkullyHopState newState);
};

#endif

