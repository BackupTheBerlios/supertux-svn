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
#include <config.h>

#include "lisp/writer.hpp"
#include "lisp/lisp.hpp"
#include "player_status.hpp"
#include "resources.hpp"
#include "gettext.hpp"
#include "video/drawing_context.hpp"
#include "audio/sound_manager.hpp"
#include "sprite/sprite_manager.hpp"
#include "math/vector.hpp"
#include "main.hpp"
#include "log.hpp"

static const int START_COINS = 100;
static const int MAX_COINS = 99999;

PlayerStatus* player_status = 0;

PlayerStatus::PlayerStatus()
  : coins(START_COINS),
    bonus(NO_BONUS),
    max_fire_bullets(0),
    max_ice_bullets(0),
    score_multiplier(1),
    max_score_multiplier(1)
{
  reset();

  coin_surface.reset(new Surface("images/engine/hud/coins-0.png"));

  Console::instance->registerCommand("coins", this);
}

PlayerStatus::~PlayerStatus()
{
}

void PlayerStatus::reset()
{
  coins = START_COINS;
  bonus = NO_BONUS;
  score_multiplier = 1;
  max_score_multiplier = 1;
}

void
PlayerStatus::add_coins(int count, bool play_sound)
{
  coins = std::min(coins + count, MAX_COINS);
  if(play_sound) {
    if(count >= 100)
      sound_manager->play("sounds/lifeup.wav");
    else
      sound_manager->play("sounds/coin.wav");
  }
}

void
PlayerStatus::write(lisp::Writer& writer)
{
  switch(bonus) {
    case NO_BONUS:
      writer.write_string("bonus", "none");
      break;
    case GROWUP_BONUS:
      writer.write_string("bonus", "growup");
      break;
    case FIRE_BONUS:
      writer.write_string("bonus", "fireflower");
      break;
    case ICE_BONUS:
      writer.write_string("bonus", "iceflower");
      break;
    default:
      log_warning << "Unknown bonus type." << std::endl;
      writer.write_string("bonus", "none");
  }
  writer.write_int("fireflowers", max_fire_bullets);
  writer.write_int("iceflowers", max_ice_bullets);

  writer.write_int("coins", coins);
  writer.write_int("max-score-multiplier", max_score_multiplier);
}

void
PlayerStatus::read(const lisp::Lisp& lisp)
{
  reset();

  std::string bonusname;
  if(lisp.get("bonus", bonusname)) {
    if(bonusname == "none") {
      bonus = NO_BONUS;
    } else if(bonusname == "growup") {
      bonus = GROWUP_BONUS;
    } else if(bonusname == "fireflower") {
      bonus = FIRE_BONUS;
    } else if(bonusname == "iceflower") {
      bonus = ICE_BONUS;
    } else {
      log_warning << "Unknown bonus '" << bonusname << "' in savefile" << std::endl;
      bonus = NO_BONUS;
    }
  }
  lisp.get("fireflowers", max_fire_bullets);
  lisp.get("iceflowers", max_ice_bullets);

  lisp.get("coins", coins);
  lisp.get("max-score-multiplier", max_score_multiplier);
}

void
PlayerStatus::draw(DrawingContext& context)
{
  static int displayed_coins = -1;
  static int next_count = 0;

  if (displayed_coins == -1) displayed_coins = coins;
  if (++next_count > 2) {
    next_count = 0;
    if (displayed_coins < coins) displayed_coins++;
    if (displayed_coins > coins) displayed_coins--;
  }
  displayed_coins = std::min(std::max(displayed_coins, 0), 9999);

  std::stringstream ss;
  ss << displayed_coins;
  std::string coins_text = ss.str();

  context.push_transform();
  context.set_translation(Vector(0, 0));

  Surface* coin_surf = coin_surface.get();
  if (coin_surf) {
    context.draw_surface(coin_surf, Vector(SCREEN_WIDTH - BORDER_X - coin_surf->get_width() - gold_text->get_text_width(coins_text), BORDER_Y + 1), LAYER_FOREGROUND1); 
  }
  context.draw_text(gold_text, coins_text, Vector(SCREEN_WIDTH - BORDER_X, BORDER_Y), RIGHT_ALLIGN, LAYER_FOREGROUND1);

  context.pop_transform();
}

void
PlayerStatus::operator= (const PlayerStatus& other)
{
  coins = other.coins;
  bonus = other.bonus;
  score_multiplier = other.score_multiplier;
  max_score_multiplier = other.max_score_multiplier;
}

bool
PlayerStatus::consoleCommand(std::string command, std::vector<std::string> arguments)
{
  if (command == "coins") {
    if ((arguments.size() < 1) || (!Console::string_is<int>(arguments[0]))) {
      log_info << "Usage: coins <number>" << std::endl;
    } else {
      coins = Console::string_to<int>(arguments[0]);
    }
    return true;
  }
  return false;
}
