//  $Id$
//
//  SuperTux - Lantern
//  Copyright (C) 2006 Wolfgang Becker <uafr@gmx.de>
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

#ifndef __SUPERTUX_LANTERN_H__
#define __SUPERTUX_LANTERN_H___

#include "object/moving_sprite.hpp"
#include "object/rock.hpp"

/**
 * Lantern. A portable Light Source.
 */
class Lantern : public Rock 
{
public:
  Lantern(const lisp::Lisp& reader);
  void draw(DrawingContext& context);
  ~Lantern();

private:
  Color lightcolor;
  Sprite* lightsprite;
  void updateColor();
};

#endif
