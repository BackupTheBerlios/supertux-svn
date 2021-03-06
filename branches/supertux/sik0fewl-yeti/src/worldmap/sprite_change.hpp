//  $Id: worldmap.hpp 3327 2006-04-13 15:02:40Z ravu_al_hemio $
//
//  SuperTux
//  Copyright (C) 2004 Ingo Ruhnke <grumbel@gmx.de>
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
#ifndef __WORLDMAP_SPRITE_CHANGE_HPP__
#define __WORLDMAP_SPRITE_CHANGE_HPP__

#include <string>
#include <memory>
#include "game_object.hpp"
#include "lisp/lisp.hpp"
#include "math/vector.hpp"

class Sprite;

namespace WorldMapNS
{

class SpriteChange : public GameObject
{
public:
  SpriteChange(const lisp::Lisp* lisp);
  virtual ~SpriteChange();

  Vector pos;
  /**
   * should tuxs sprite change when the tile has been completely entered,
   * or already when the tile was just touched
   */
  bool change_on_touch;
  /// sprite to change tux image to
  std::auto_ptr<Sprite> sprite;
  /**
   * stay action can be used for objects like boats or cars, if it is
   * != "" then this sprite will be displayed when tux left the tile towards
   * another SpriteChange object.
   */
  std::string stay_action;
  /**
   * should the stayaction be displayed
   */
  bool in_stay_action;

  virtual void draw(DrawingContext& context);
  virtual void update(float elapsed_time);
};

}

#endif

