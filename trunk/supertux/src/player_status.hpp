//  $Id$
//
//  SuperTux
//  Copyright (C) 2003 Tobias Glaesser <tobi.web@gmx.de>
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
#ifndef SUPERTUX_PLAYERSTATUS_H
#define SUPERTUX_PLAYERSTATUS_H

#include <assert.h>
#include <memory>
#include "lisp/lisp.hpp"
#include "timer.hpp"
#include "serializable.hpp"
#include "sprite/sprite.hpp"
#include "console.hpp"
#include "video/surface.hpp"

static const float BORDER_X = 10;
static const float BORDER_Y = 10;

enum BonusType {
  NO_BONUS, GROWUP_BONUS, FIRE_BONUS, ICE_BONUS
};
class DrawingContext;

/**
 * This class memorizes player status between different game sessions (for
 * example when switching maps in the worldmap)
 */
class PlayerStatus : public Serializable, public ConsoleCommandReceiver
{
public:
  PlayerStatus();
  ~PlayerStatus();
  void reset();
  void add_coins(int count, bool play_sound = true);

  void write(lisp::Writer& writer);
  void read(const lisp::Lisp& lisp);

  void draw(DrawingContext& context);

  bool consoleCommand(std::string command, std::vector<std::string> arguments); /**< callback from Console; return false if command was unknown, true otherwise */

  int  coins;
  BonusType bonus;
  int max_fire_bullets; /**< maximum number of fire bullets in play */
  int max_ice_bullets; /**< maximum number of ice bullets in play */

  int score_multiplier;
  int max_score_multiplier;

  void operator= (const PlayerStatus& other);

private:
  // don't use this
  PlayerStatus(const PlayerStatus& other);

  std::auto_ptr<Surface> coin_surface;
};

// global player state
extern PlayerStatus* player_status;

#endif
