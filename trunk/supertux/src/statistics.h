//  SuperTux
//  Copyright (C) 2004 SuperTux Development Team, see AUTHORS for details
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

#ifndef SUPERTUX_STATISTICS_H
#define SUPERTUX_STATISTICS_H

using namespace SuperTux;

namespace SuperTux {
class LispReader;
class LispWriter;
}

enum {
  SCORE_STAT,
  MAX_STATS
};

/** This class is a layer between level and worldmap to keep
 *  track of stuff like scores, and minor, but funny things, like
 *  number of jumps and stuff */

class Statistics
{
public:
  Statistics();
  ~Statistics();

  /// read statistics from lisp file
  void parse(LispReader& reader);
  /// write statistics to lisp file
  void write(LispWriter& writer);

  // TODO: add drawing functions to draw stats on WorldMap

  void add_points(int stat, int points);
  int get_points(int stat);

  void reset();

  /* Give another Statistics object, find the best of each one */
  void merge(Statistics& stats);

  /* Add two statistic objects */
  void operator+=(const Statistics& o);

private:
  int stats[MAX_STATS];
};

extern Statistics global_stats;

#endif /*SUPERTUX_STATISTICS_H*/
