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

#ifndef __SCRIPTED_OBJECT_H__
#define __SCRIPTED_OBJECT_H__

#include <string>
#include "physic.hpp"
#include "sprite/sprite.hpp"
#include "lisp/lisp.hpp"
#include "moving_object.hpp"
#include "script_interface.hpp"
#include "scripting/scripted_object.hpp"

class ScriptedObject : public MovingObject, public Scripting::ScriptedObject,
                       public ScriptInterface
{
public:
  ScriptedObject(const lisp::Lisp& lisp);
  virtual ~ScriptedObject();

  virtual void expose(HSQUIRRELVM vm, int table_idx);
  virtual void unexpose(HSQUIRRELVM vm, int table_idx);

  void update(float elapsed_time);
  void draw(DrawingContext& context);
  HitResponse collision(GameObject& other, const CollisionHit& hit);

  // --- Scripting Interface stuff ---

  void set_action(const std::string& animation);
  std::string get_action();

  void move(float x, float y);
  void set_pos(float x, float y);
  float get_pos_x();
  float get_pos_y();
  void set_velocity(float x, float y);
  float get_velocity_x();
  float get_velocity_y();
  void set_visible(bool visible);
  bool is_visible();

  std::string get_name();

private:
  std::string name;
  bool solid;
  bool physic_enabled;
  bool visible;
  bool new_vel_set;
  int layer;
  Vector new_vel;
  Physic physic;
  Sprite* sprite;
};

#endif

