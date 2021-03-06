//  $Id: dart.hpp 3327 2006-04-13 15:02:40Z sommer $
//
//  Dart - Your average poison dart
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

#ifndef __DART_H__
#define __DART_H__

#include "badguy.hpp"

/**
 * Badguy "Dart" - Your average poison dart
 */
class Dart : public BadGuy
{
public:
  Dart(const lisp::Lisp& reader);
  Dart(float pos_x, float pos_y, Direction d, const BadGuy* parent);
  ~Dart();

  void activate();
  void deactivate();
  void write(lisp::Writer& writer);

  void active_update(float elapsed_time);

  HitResponse collision_solid(GameObject& object, const CollisionHit& hit);
  HitResponse collision_badguy(BadGuy& badguy, const CollisionHit& hit);
  HitResponse collision_player(Player& player, const CollisionHit& hit);

protected:
  bool set_direction;
  Direction initial_direction;
  const BadGuy* parent; /**< collisions with this BadGuy will be ignored */
  SoundSource* soundSource; /**< SoundSource for ambient sound */
};

#endif

