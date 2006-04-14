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
#include <config.h>

#include <stddef.h>
#include <physfs.h>
#include "level.hpp"
#include "sprite/sprite_manager.hpp"
#include "sprite/sprite.hpp"
#include "video/drawing_context.hpp"

namespace WorldMapNS
{

Level::Level(const std::string& basedir, const lisp::Lisp* lisp)
  : solved(false), auto_path(true)
{
  lisp->get("x", pos.x);
  lisp->get("y", pos.y);
  
  std::string spritefile = "images/worldmap/common/leveldot.sprite";
  lisp->get("sprite", spritefile);
  sprite.reset(sprite_manager->create(spritefile));

  lisp->get("extro-script", extro_script);
  lisp->get("name", name);
  
  if (!PHYSFS_exists((basedir + name).c_str()))
  {
    log_warning << "level file '" << name 
      << "' does not exist and will not be added to the worldmap" << std::endl;
    return;
  }
}

Level::~Level()
{
}

void
Level::draw(DrawingContext& context)
{
  sprite->draw(context, pos*32 + Vector(16, 16), LAYER_OBJECTS - 1);
}

void
Level::update(float )
{
}

}
