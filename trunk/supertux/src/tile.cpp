//  $Id$
// 
//  SuperTux
//  Copyright (C) 2004 Tobias Glaesser <tobi.web@gmx.de>
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

#include <config.h>

#include <cmath>
#include <cassert>
#include <iostream>
#include <stdexcept>

#include "app/globals.h"
#include "tile.h"
#include "scene.h"
#include "utils/lispreader.h"
#include "math/vector.h"
#include "video/drawing_context.h"

/** Dirty little helper to create a surface from a snipped of lisp:
 *
 *  "filename"
 *  (region "filename" x y w h)
 */
static
Surface* create_surface(lisp_object_t* cur)
{
  if (lisp_string_p(cur))
    {
      return new Surface(datadir + "/images/tilesets/" + lisp_string(cur),
                         true);
    }
  else if (lisp_cons_p(cur) && lisp_symbol_p(lisp_car(cur)))
    {
      lisp_object_t* sym  = lisp_car(cur);
      lisp_object_t* data = lisp_cdr(cur);
      
      if (strcmp(lisp_symbol(sym), "region") == 0)
        {
          if (lisp_list_length(data) == 5) // (image-region filename x y w h)
            {
              return new Surface(datadir + "/images/tilesets/" + lisp_string(lisp_car(data)), 
                                 lisp_integer(lisp_list_nth(data, 1)),
                                 lisp_integer(lisp_list_nth(data, 2)),
                                 lisp_integer(lisp_list_nth(data, 3)),
                                 lisp_integer(lisp_list_nth(data, 4)),
                                 true);
            }
          else
            {
              std::cout << "Tile: Type mispatch, should be '(region \"somestring\" x y w h)'" << std::endl;
              return 0;
            }
        }
      else
        {
          std::cout << "Tile: Unhandled tag: " << lisp_symbol(sym) << std::endl;
          return 0;
        }
    }

  std::cout << "Tile: unhandled element" << std::endl;
  return 0;  
}

/** Create a vector of surfaces (aka Sprite) from a piece of lisp:
    ((image "bla.png") (image-region "bla.png") ...)
 */
static 
std::vector<Surface*> create_surfaces(lisp_object_t* cur)
{
  std::vector<Surface*> surfs;

  while(cur)
    {
      Surface* surface = create_surface(lisp_car(cur));
      if (surface)
        surfs.push_back(surface); 
      else
        std::cout << "Tile: Couldn't create image" << std::endl;
        
      cur = lisp_cdr(cur);
    }
  
  return surfs;
}

Tile::Tile()
  : id(0), attributes(0), data(0), next_tile(0), anim_speed(25)
{
}

Tile::~Tile()
{
  for(std::vector<Surface*>::iterator i = images.begin(); i != images.end();
      ++i) {
    delete *i;
  }
  for(std::vector<Surface*>::iterator i = editor_images.begin();
      i != editor_images.end(); ++i) {
    delete *i;                                                                
  }
}

void
Tile::read(LispReader& reader)
{
  if(!reader.read_uint("id", id)) {
    throw std::runtime_error("Missing tile-id.");
  }
  
  bool value;
  if(reader.read_bool("solid", value) && value)
    attributes |= SOLID;
  if(reader.read_bool("unisolid", value) && value)
    attributes |= UNISOLID | SOLID;
  if(reader.read_bool("brick", value) && value)
    attributes |= BRICK;
  if(reader.read_bool("ice", value) && value)
    attributes |= ICE;
  if(reader.read_bool("water", value) && value)
    attributes |= WATER;
  if(reader.read_bool("spike", value) && value)
    attributes |= SPIKE;
  if(reader.read_bool("fullbox", value) && value)
    attributes |= FULLBOX;
  if(reader.read_bool("distro", value) && value)
    attributes |= COIN;
  if(reader.read_bool("coin", value) && value)
    attributes |= COIN;
  if(reader.read_bool("goal", value) && value)
    attributes |= GOAL;

  reader.read_int("data", data);
  reader.read_int("anim-speed", anim_speed);
  reader.read_int("next-tile", next_tile);

  if(reader.read_int("slope-type", data)) {
    attributes |= SOLID | SLOPE;
  }

  images        = create_surfaces(reader.read_lisp("images"));
  editor_images = create_surfaces(reader.read_lisp("editor-images"));
}

void
Tile::draw(DrawingContext& context, const Vector& pos, int layer) const
{
  if(images.size() > 1) {
    size_t frame = ((global_frame_counter*25) / anim_speed) % images.size();
    context.draw_surface(images[frame], pos, layer);
  } else if (images.size() == 1) {
    context.draw_surface(images[0], pos, layer);
  }
}

