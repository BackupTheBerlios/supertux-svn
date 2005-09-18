//  SuperTux
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
#include "electrifier.hpp"
#include "sector.hpp"
#include "object/tilemap.hpp"
#include "tile.hpp"


Electrifier::Electrifier(uint32_t oldtile, uint32_t newtile, float seconds)
{
  duration.start(seconds);
  change_from = oldtile;
  change_to = newtile;
  Sector::current()->solids->change_all(change_from,change_to);
}
  
Electrifier::~Electrifier() {
  remove_me();
}

void
Electrifier::update(float ) 
{
  if (duration.check()) {
    Sector::current()->solids->change_all(change_to,change_from);
    remove_me();
  }
}

void
Electrifier::draw(DrawingContext& ) 
{
}
