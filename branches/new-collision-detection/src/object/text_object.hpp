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

#ifndef __TEXTOBJECT_H__
#define __TEXTOBJECT_H__

#include "game_object.hpp"
#include "scripting/text.hpp"
#include "script_interface.hpp"

class Font;

/** A text object intended for scripts that want to tell a story */
class TextObject : public GameObject, public Scripting::Text,
                   public ScriptInterface
{
public:
  TextObject();
  virtual ~TextObject();

  void expose(HSQUIRRELVM vm, SQInteger table_idx);
  void unexpose(HSQUIRRELVM vm, SQInteger table_idx);

  void set_text(const std::string& text);
  void set_font(const std::string& name);
  void fade_in(float fadetime);
  void fade_out(float fadetime);
  void set_visible(bool visible);
  void set_centered(bool centered);
  bool is_visible();

  void draw(DrawingContext& context);
  void update(float elapsed_time);

private:
  Font* font;
  std::string text;
  float fading;
  float fadetime;
  bool visible;
  bool centered;
};

#endif

