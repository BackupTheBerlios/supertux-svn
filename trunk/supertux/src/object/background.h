//  $Id$
//
//  SuperTux -  A Jump'n Run
//  Copyright (C) 2004 Matthias Braun <matze@braunis.de
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

#ifndef SUPERTUX_BACKGROUND_H
#define SUPERTUX_BACKGROUND_H

#include "video/surface.h"
#include "video/drawing_context.h"
#include "special/game_object.h"
#include "serializable.h"

class DisplayManager;

namespace lisp {
class Lisp;
}

class Background : public GameObject, public Serializable
{
public:
  Background();
  Background(const lisp::Lisp& reader);
  virtual ~Background();

  virtual void write(lisp::Writer& writer);

  void set_image(const std::string& name, float bkgd_speed);

  void set_gradient(Color top, Color bottom);

  std::string get_image() const
        { return imagefile; }
  float get_speed() const
        { return speed; }
  Color get_gradient_top() const
        { return gradient_top; }
  Color get_gradient_bottom() const
        { return gradient_bottom; }

  virtual void action(float elapsed_time);

  virtual void draw(DrawingContext& context);

private:
  enum Type {
    INVALID, GRADIENT, IMAGE      
  };
  
  Type type;
  int layer;
  std::string imagefile;
  float speed;
  Surface* image;
  Color gradient_top, gradient_bottom;
};

#endif /*SUPERTUX_BACKGROUND_H*/

