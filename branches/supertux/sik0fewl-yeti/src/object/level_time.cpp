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

#include <config.h>

#include "level_time.hpp"

#include <stdexcept>
#include <iostream>
#include "main.hpp"
#include "resources.hpp"
#include "sector.hpp"
#include "gettext.hpp"
#include "object_factory.hpp"
#include "object/player.hpp"
#include "video/drawing_context.hpp"
#include "lisp/list_iterator.hpp"
#include "log.hpp"

/** When to alert player they're low on time! */
static const float TIME_WARNING = 20;

LevelTime::LevelTime(const lisp::Lisp& reader)
: final_level_time(0.f), final_remaining_time(0.f)
{
    float time = -1;
    lisp::ListIterator iter(&reader);
    while(iter.next()) {
        if(iter.item() == "time") {
            iter.value()->get(time);
            break;
        } else {
            log_warning << "Unknown token '" << iter.item() << "' in LevelTime object" << std::endl;
        }
    }
    if(time < 0)
      throw std::runtime_error("Invalid leveltime specified");
    time_left.start(time);
}

LevelTime::~LevelTime()
{}

void
LevelTime::update(float )
{
  if(time_left.check()) {
    Sector::current()->player->kill(Player::KILL);
  }
}

void
LevelTime::draw(DrawingContext& context)
{
  context.push_transform();
  context.set_translation(Vector(0, 0));

  char str[60];
    
  if(get_remaining_time() < 0) {
    context.draw_text(white_text, _("TIME's UP"), Vector(SCREEN_WIDTH/2, BORDER_Y),
        CENTER_ALLIGN, LAYER_FOREGROUND1);
  } else if (get_remaining_time() > TIME_WARNING
      || int(game_time * 2.5) % 2) {
    snprintf(str, sizeof(str), " %d", int(get_remaining_time()));
    context.draw_text(white_text, _("TIME"),
        Vector(SCREEN_WIDTH/2, BORDER_Y), CENTER_ALLIGN, LAYER_FOREGROUND1);
    context.draw_text(gold_text, str, Vector(SCREEN_WIDTH/2 + 4*16, BORDER_Y),
                      CENTER_ALLIGN, LAYER_FOREGROUND1);
  }

  context.pop_transform();
}

float
LevelTime::get_level_time()
{
  if (!time_left.started())
    return final_level_time;
  return time_left.get_period();
}

float
LevelTime::get_remaining_time()
{
  if (!time_left.started())
    return final_remaining_time;
  return time_left.get_timeleft();
}

void
LevelTime::stop()
{
  final_level_time = time_left.get_period();
  final_remaining_time = time_left.get_timeleft();
  time_left.stop();
}

IMPLEMENT_FACTORY(LevelTime, "leveltime");
