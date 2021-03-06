//  $Id$
//
//  SuperTux (Statistics module)
//  Copyright (C) 2004 Ricardo Cruz <rick2@aeiou.pt>
//  Copyright (C) 2006 Ondrej Hosek <ondra.hosek@gmail.com>
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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef SUPERTUX_STATISTICS_H
#define SUPERTUX_STATISTICS_H

#include "timer.hpp"
#include "lisp/lisp.hpp"
#include "lisp/writer.hpp"
#include "video/surface.hpp"
#include "video/drawing_context.hpp"

/** This class is a layer between level and worldmap to keep
 *  track of stuff like scores, and minor, but funny things, like
 *  number of jumps and stuff */
class Statistics
{ 
public:
  int coins; /**< coins collected */
  int total_coins; /**< coins in level */
  int badguys; /**< badguys actively killed */
  int total_badguys; /**< (vincible) badguys in level */
  float time; /**< seconds needed */
  int secrets; /**< secret areas found */
  int total_secrets; /**< secret areas in level */

public:
  Statistics(); /**< Creates new statistics, call reset() before counting */
  ~Statistics();

  /// read statistics from lisp file
  void parse(const lisp::Lisp& lisp);
  /// write statistics to lisp file
  void write(lisp::Writer& writer);

  void draw_worldmap_info(DrawingContext& context); /**< draw worldmap stat HUD */
  void draw_message_info(DrawingContext& context, std::string title); /**< draw stats at level start */
  void draw_endseq_panel(DrawingContext& context, Statistics* best_stats, Surface* backdrop); /**< draw panel shown during level's end sequence */

  void reset(); /**< Set stats (but not totals) to zero */
  void merge(Statistics& stats); /**< Given another Statistics object finds the best of each one */
  void operator+=(const Statistics& o); /**< Add two Statistics objects */

private:
  Timer timer; /**< for draw_worldmap_info: time until switching to next stat */
  int display_stat; /**< for draw_worldmap_info: which stat is currently displayed */
};

#endif /*SUPERTUX_STATISTICS_H*/
