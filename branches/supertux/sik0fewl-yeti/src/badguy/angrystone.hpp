//  $Id$
//
//  AngryStone - A spiked block that charges towards the player
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

#ifndef __ANGRYSTONE_H__
#define __ANGRYSTONE_H__

#include "badguy.hpp"

class AngryStone : public BadGuy
{
public:
  AngryStone(const lisp::Lisp& reader);

  void activate();
  void write(lisp::Writer& writer);
  HitResponse collision_solid(GameObject& other, const CollisionHit& hit);
  HitResponse collision_badguy(BadGuy& badguy, const CollisionHit& hit);
  void active_update(float elapsed_time);
  void kill_fall();

protected:
  Vector attackDirection;  /**< 1-normalized vector of current attack direction */
  Vector oldWallDirection; /**< if wall was hit during last attack: 1-normalized vector of last attack direction, (0,0) otherwise */

  Timer timer;

  enum AngryStoneState {
    IDLE,
    CHARGING,
    ATTACKING,
    RECOVERING
  };
  AngryStoneState state;
  
};

#endif
