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

#ifndef SUPERTUX_TIMER_H
#define SUPERTUX_TIMER_H

#include <cstdlib>

namespace SuperTux
  {

  class Ticks
    {
    public:
      /// Time a game is running. (Non-pause mode, etc.)
      static unsigned int get();

      static void pause_init();
      static void pause_start();
      static void pause_stop();
      static bool pause_started();

    private:
      static unsigned int pause_ticks;
      static unsigned int pause_count;

    };

  /// Timer
  /** This class can be used as stop watch
      for example. It's also possible to calculate
      frames per seconds and things like that with it.
      It's a general timing class, but it
      can esspecially be used together with Ticks::get(). */
  class Timer
    {
    public:
      unsigned int period;
      unsigned int time;
      unsigned int (*get_ticks) (void);

    public:
      Timer();

      /// Initialize the timer.
      /** @param st_ticks: If true internally Ticks::get() is used, else SDL_GetTicks() is used. */
      void init(bool game_ticks);

      /// Start the timer with the given period (in ms).
      void start(unsigned int period);

      /// Stop the timer.
      void stop();

      /// Check if the timer is started and within its period.
      /** If one of these requirements isn't the case the timer
          is automatically reseted. */
      int check();

      /// Is the timer started?
      int started();

      /// Get time left until the last timing period is finished.
      /** The returned value can be negative. */
      int get_left();

      /// Get the gone time, since last timer start.
      /** The returned value can be negative. */
      int  get_gone();

      /// Write the timer value to a file (For save games in example).
      void fwrite(FILE* fi);
      /// Read a timer value from a file (From save games in example).
      void fread(FILE* fi);
    };

} //namespace SuperTux

#endif /*SUPERTUX_TIMER_H*/

/* Local Variables: */
/* mode:c++ */
/* End: */
