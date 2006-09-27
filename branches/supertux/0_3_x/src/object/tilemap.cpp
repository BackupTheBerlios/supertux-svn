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

#include <cassert>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <cmath>

#include "tilemap.hpp"
#include "video/drawing_context.hpp"
#include "level.hpp"
#include "tile.hpp"
#include "resources.hpp"
#include "tile_manager.hpp"
#include "lisp/lisp.hpp"
#include "lisp/writer.hpp"
#include "object_factory.hpp"
#include "main.hpp"
#include "log.hpp"
#include "scripting/tilemap.hpp"
#include "scripting/squirrel_util.hpp"

TileMap::TileMap()
  : solid(false), speed_x(1), speed_y(1), width(0), height(0), z_pos(0), x_offset(0), y_offset(0),
    drawing_effect(NO_EFFECT), alpha(1.0), current_alpha(1.0), remaining_fade_time(0),
    draw_target(DrawingContext::NORMAL)
{
  tilemanager = tile_manager;
}

TileMap::TileMap(const lisp::Lisp& reader, TileManager* new_tile_manager)
  : solid(false), speed_x(1), speed_y(1), width(-1), height(-1), z_pos(0),
    x_offset(0), y_offset(0),
    drawing_effect(NO_EFFECT), alpha(1.0), current_alpha(1.0),
    remaining_fade_time(0),
    draw_target(DrawingContext::NORMAL)
{
  tilemanager = new_tile_manager;
  if(tilemanager == 0)
    tilemanager = tile_manager;

  reader.get("name", name);
  reader.get("z-pos", z_pos);
  reader.get("solid", solid);
  reader.get("speed", speed_x);
  reader.get("speed-y", speed_y);

  if(solid && ((speed_x != 1) || (speed_y != 1))) {
    log_warning << "Speed of solid tilemap is not 1. fixing" << std::endl;
    speed_x = 1;
    speed_y = 1;
  }

  const lisp::Lisp* pathLisp = reader.get_lisp("path");
  if (pathLisp) {
    path.reset(new Path());
    path->read(*pathLisp);
    walker.reset(new PathWalker(path.get(), /*running*/false));
    Vector v = path->get_base();
    set_x_offset(v.x);
    set_y_offset(v.y);
  }
  
  std::string draw_target_s = "normal";
  reader.get("draw-target", draw_target_s);
  if (draw_target_s == "normal") draw_target = DrawingContext::NORMAL;
  if (draw_target_s == "lightmap") draw_target = DrawingContext::LIGHTMAP;

  reader.get("width", width);
  reader.get("height", height);
  if(width < 0 || height < 0)
    throw std::runtime_error("Invalid/No width/height specified in tilemap.");

  if(!reader.get_vector("tiles", tiles))
    throw std::runtime_error("No tiles in tilemap.");

  if(int(tiles.size()) != width*height) {
    throw std::runtime_error("wrong number of tiles in tilemap.");
  }

  // make sure all tiles are loaded
  for(Tiles::iterator i = tiles.begin(); i != tiles.end(); ++i)
    tilemanager->get(*i);
}

TileMap::TileMap(std::string name, int z_pos, bool solid, size_t width, size_t height)
  : solid(solid), speed_x(1), speed_y(1), width(0), height(0), z_pos(z_pos),
    x_offset(0), y_offset(0), drawing_effect(NO_EFFECT), alpha(1.0),
    current_alpha(1.0), remaining_fade_time(0),
    draw_target(DrawingContext::NORMAL)
{
  this->name = name;
  tilemanager = tile_manager;

  resize(width, height);
}

TileMap::~TileMap()
{
}

void
TileMap::write(lisp::Writer& writer)
{
  writer.start_list("tilemap");

  writer.write_int("z-pos", z_pos);

  writer.write_bool("solid", solid);
  writer.write_float("speed", speed_x);
  writer.write_float("speed-y", speed_y);
  writer.write_int("width", width);
  writer.write_int("height", height);
  writer.write_int_vector("tiles", tiles);

  writer.end_list("tilemap");
}

void
TileMap::update(float elapsed_time)
{
  // handle tilemap fading
  if (current_alpha != alpha) {
    remaining_fade_time = std::max(0.0f, remaining_fade_time - elapsed_time);
    if (remaining_fade_time == 0.0f) {
      current_alpha = alpha;
    } else {
      float amt = (alpha - current_alpha) / (remaining_fade_time / elapsed_time);
      if (amt > 0) current_alpha = std::min(current_alpha + amt, alpha);
      if (amt < 0) current_alpha = std::max(current_alpha + amt, alpha);
    }
  }

  // if we have a path to follow, follow it
  if (walker.get()) {
    Vector v = walker->advance(elapsed_time);
    set_x_offset(v.x);
    set_y_offset(v.y);
  }
}

void
TileMap::draw(DrawingContext& context)
{
  context.push_transform();
  context.push_target();
  context.set_target(draw_target);

  if(drawing_effect != 0) context.set_drawing_effect(drawing_effect);
  if(current_alpha != 1.0) context.set_alpha(current_alpha);

  float trans_x = roundf(context.get_translation().x);
  float trans_y = roundf(context.get_translation().y);
  context.set_translation(Vector(trans_x * speed_x, trans_y * speed_y));

  /** if we don't round here, we'll have a 1 pixel gap on screen sometimes.
   * I have no idea why */
  float start_x = int((roundf(context.get_translation().x) - roundf(x_offset)) / 32) * 32 + roundf(x_offset);
  float start_y = int((roundf(context.get_translation().y) - roundf(y_offset)) / 32) * 32 + roundf(y_offset);
  float end_x = std::min(start_x + SCREEN_WIDTH + 32, float(width * 32 + roundf(x_offset)));
  float end_y = std::min(start_y + SCREEN_HEIGHT + 32, float(height * 32 + roundf(y_offset)));
  int tsx = int((start_x - roundf(x_offset)) / 32); // tilestartindex x
  int tsy = int((start_y - roundf(y_offset)) / 32); // tilestartindex y

  Vector pos;
  int tx, ty;
  for(pos.x = start_x, tx = tsx; pos.x < end_x; pos.x += 32, ++tx) {
    for(pos.y = start_y, ty = tsy; pos.y < end_y; pos.y += 32, ++ty) {
      if ((tx < 0) || (ty < 0)) continue;
      const Tile* tile = tilemanager->get(tiles[ty*width + tx]);
      assert(tile != 0);
      tile->draw(context, pos, z_pos);
    }
  }

  context.pop_target();
  context.pop_transform();
}

void
TileMap::goto_node(int node_no)
{
  if (!walker.get()) return;
  walker->goto_node(node_no);
}

void
TileMap::start_moving()
{
  if (!walker.get()) return;
  walker->start_moving();
}

void
TileMap::stop_moving()
{
  if (!walker.get()) return;
  walker->stop_moving();
}

void
TileMap::expose(HSQUIRRELVM vm, SQInteger table_idx)
{
  if (name.empty()) return;
  if (!walker.get()) return;
  Scripting::TileMap* interface = new Scripting::TileMap(this);
  expose_object(vm, table_idx, interface, name, true);
}

void
TileMap::unexpose(HSQUIRRELVM vm, SQInteger table_idx)
{
  if (name.empty()) return;
  if (!walker.get()) return;
  Scripting::unexpose_object(vm, table_idx, name);
}

void
TileMap::set(int newwidth, int newheight, const std::vector<unsigned int>&newt,
    int new_z_pos, bool newsolid)
{
  if(int(newt.size()) != newwidth * newheight)
    throw std::runtime_error("Wrong tilecount count.");

  width  = newwidth;
  height = newheight;

  tiles.resize(newt.size());
  tiles = newt;

  z_pos  = new_z_pos;
  solid  = newsolid;

  // make sure all tiles are loaded
  for(Tiles::iterator i = tiles.begin(); i != tiles.end(); ++i)
    tilemanager->get(*i);
}

void
TileMap::resize(int new_width, int new_height)
{
  if(new_width < width) {
    // remap tiles for new width
    for(int y = 0; y < height && y < new_height; ++y) {
      for(int x = 0; x < new_width; ++x) {
        tiles[y * new_width + x] = tiles[y * width + x];
      }
    }
  }

  tiles.resize(new_width * new_height);

  if(new_width > width) {
    // remap tiles
    for(int y = std::min(height, new_height)-1; y >= 0; --y) {
      for(int x = new_width-1; x >= 0; --x) {
        if(x >= width) {
          tiles[y * new_width + x] = 0;
          continue;
        }

        tiles[y * new_width + x] = tiles[y * width + x];
      }
    }
  }

  height = new_height;
  width = new_width;
}

const Tile*
TileMap::get_tile(int x, int y) const
{
  if(x < 0 || x >= width || y < 0 || y >= height) {
    //log_warning << "tile outside tilemap requested" << std::endl;
    return tilemanager->get(0);
  }

  return tilemanager->get(tiles[y*width + x]);
}

const Tile*
TileMap::get_tile_at(const Vector& pos) const
{
  return get_tile(int(pos.x - x_offset)/32, int(pos.y - y_offset)/32);
}

void
TileMap::change(int x, int y, uint32_t newtile)
{
  assert(x >= 0 && x < width && y >= 0 && y < height);
  tiles[y*width + x] = newtile;
}

void
TileMap::change_at(const Vector& pos, uint32_t newtile)
{
  change(int(pos.x - x_offset)/32, int(pos.y - y_offset)/32, newtile);
}

void
TileMap::change_all(uint32_t oldtile, uint32_t newtile)
{
  for (size_t x = 0; x < get_width(); x++)
    for (size_t y = 0; y < get_height(); y++) {
      if (get_tile(x,y)->getID() == oldtile) change(x,y,newtile);
    }
}

void 
TileMap::fade(float alpha, float seconds)
{
  this->alpha = alpha;
  this->remaining_fade_time = seconds;
}

IMPLEMENT_FACTORY(TileMap, "tilemap");
