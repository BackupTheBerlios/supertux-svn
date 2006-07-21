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

#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <vector>
#include "log.hpp"

class Color
{
public:
  Color()
    : red(0), green(0), blue(0), alpha(1.0)
  { }
  Color(float red, float green, float blue, float alpha = 1.0)
    : red(red), green(green), blue(blue), alpha(alpha)
  {
#ifdef DEBUG
    check_color_ranges();
#endif
  }
  Color(const std::vector<float>& vals)
  {
    red = vals[0];
    green = vals[1];
    blue = vals[2];
    if(vals.size() > 3)
      alpha = vals[3];
    else
      alpha = 1.0;
#ifdef DEBUG
    check_color_ranges();
#endif
  }

  void check_color_ranges()
  {
    if(red < 0 || red > 1.0 || green < 0 || green > 1.0
            || blue < 0 || blue > 1.0
            || alpha < 0 || alpha > 1.0)
      log_warning << "color value out of range: " << red << ", " << green << ", " << blue << ", " << alpha << std::endl;
  }

  float red, green, blue, alpha;
};

#endif
