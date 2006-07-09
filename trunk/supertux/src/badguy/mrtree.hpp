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

#ifndef __MRTREE_H__
#define __MRTREE_H__

#include "badguy.hpp"

class MrTree : public BadGuy
{
public:
  MrTree(const lisp::Lisp& reader);

  void activate();
  void active_update(float elapsed_time);
  void write(lisp::Writer& writer);
  void collision_solid(const CollisionHit& hit);
  HitResponse collision_badguy(BadGuy& badguy, const CollisionHit& hit);

  virtual MrTree* clone() const { return new MrTree(*this); }

protected:
  enum MyState {
    STATE_BIG, STATE_INVINCIBLE, STATE_NORMAL
  };
  MyState mystate;

  Timer invincible_timer;

  bool collision_squished(Player& player);
};

#endif
