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

#include "text_object.hpp"

#include <iostream>
#include "resources.hpp"
#include "video/drawing_context.hpp"
#include "scripting/squirrel_util.hpp"
#include "log.hpp"

TextObject::TextObject()
  : fading(0), fadetime(0), visible(false)
{
  font = blue_text;
  centered = false;
}

TextObject::~TextObject()
{
}

void
TextObject::expose(HSQUIRRELVM vm, SQInteger table_idx)
{
  Scripting::Text* interface = static_cast<Scripting::Text*> (this);
  Scripting::expose_object(vm, table_idx, interface, "Text", false);
}

void
TextObject::unexpose(HSQUIRRELVM vm, SQInteger table_idx)
{
  Scripting::unexpose_object(vm, table_idx, "Text");
}

void
TextObject::set_font(const std::string& name)
{
  if(name == "gold") {
    font = gold_text;
  } else if(name == "white") {
    font = white_text;
  } else if(name == "blue") {
    font = blue_text;
  } else if(name == "gray") {
    font = gray_text;
  } else if(name == "big") {
    font = white_big_text;
  } else if(name == "small") {
    font = white_small_text;
  } else {
    log_warning << "Unknown font '" << name << "'." << std::endl;
  }
}

void
TextObject::set_text(const std::string& text)
{
  this->text = text;
}

void
TextObject::fade_in(float fadetime)
{
  this->fadetime = fadetime;
  fading = fadetime;
}

void
TextObject::fade_out(float fadetime)
{
  this->fadetime = fadetime;
  fading = -fadetime;
}

void
TextObject::set_visible(bool visible)
{
  this->visible = visible;
  fading = 0;
}

void
TextObject::set_centered(bool centered)
{
  this->centered = centered;
}

void
TextObject::draw(DrawingContext& context)
{
  context.push_transform();
  context.set_translation(Vector(0, 0));
  if(fading > 0) {
    context.set_alpha((fadetime-fading) / fadetime);
  } else if(fading < 0) {
    context.set_alpha(-fading / fadetime);
  } else if(!visible) {
    context.pop_transform();
    return;
  }

  context.draw_filled_rect(Vector(125, 50), Vector(550, 120),
      Color(0.6, 0.7, 0.8, 0.5), LAYER_GUI-50);
  if (centered) {
    context.draw_center_text(font, text, Vector(0, 50+35), LAYER_GUI-40);
  }
  else context.draw_text(font, text, Vector(125+35, 50+35), LEFT_ALLIGN, LAYER_GUI-40);

  context.pop_transform();
}

void
TextObject::update(float elapsed_time)
{
  if(fading > 0) {
    fading -= elapsed_time;
    if(fading <= 0) {
      fading = 0;
      visible = true;
    }
  } else if(fading < 0) {
    fading += elapsed_time;
    if(fading >= 0) {
      fading = 0;
      visible = false;
    }
  }
}

