//  $Id$
//
//  SuperTux -  A Jump'n Run
//  Copyright (C) 2004 Matthias Braun <matze@braunis.de
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

#include "tilemap.h"
#include "video/drawing_context.h"
#include "level.h"
#include "tile.h"
#include "tile_manager.h"
#include "app/globals.h"
#include "utils/lispreader.h"
#include "utils/lispwriter.h"

TileMap::TileMap()
  : solid(false), speed(1), width(0), height(0), layer(LAYER_TILES),
    vertical_flip(false)
{
  tilemanager = TileManager::instance();

  if(solid)
    flags |= FLAG_SOLID;
}

TileMap::TileMap(LispReader& reader)
  : solid(false), speed(1), width(0), height(0), layer(LAYER_TILES),
    vertical_flip(false)
{
  tilemanager = TileManager::instance();

  std::string layer_str;
  if(reader.read_string("layer", layer_str)) {
    if(layer_str == "background")
      layer = LAYER_BACKGROUNDTILES;
    else if(layer_str == "interactive")
      layer = LAYER_TILES;
    else if(layer_str == "foreground")
      layer = LAYER_FOREGROUNDTILES;
    else
      std::cerr << "Unknown layer '" << layer_str << "' in tilemap.\n";
  }

  reader.read_bool("solid", solid);
  reader.read_float("speed", speed);

  if(solid && speed != 1) {
    std::cout << "Speed of solid tilemap is not 1. fixing.\n";
    speed = 1;
  }
  if(solid)
    flags |= FLAG_SOLID;
  
  if(!reader.read_int("width", width) ||
     !reader.read_int("height", height))
    throw std::runtime_error("No width or height specified in tilemap.");

  if(!reader.read_int_vector("tiles", tiles))
    throw std::runtime_error("No tiles in tilemap.");

  if(int(tiles.size()) != width*height)
    throw std::runtime_error("wrong number of tiles in tilemap.");
}

TileMap::TileMap(int layer_, bool solid_, size_t width_, size_t height_)
  : solid(solid_), speed(1), width(0), height(0), layer(layer_),
    vertical_flip(false)
{
  tilemanager = TileManager::instance();
  
  resize(width_, height_);

  if(solid)
    flags |= FLAG_SOLID;  
}

TileMap::~TileMap()
{
}

void
TileMap::write(LispWriter& writer)
{
  writer.start_list("tilemap");

  if(layer == LAYER_BACKGROUNDTILES)
    writer.write_string("layer", "background");
  else if(layer == LAYER_TILES)
    writer.write_string("layer", "interactive");
  else if(layer == LAYER_FOREGROUNDTILES)
    writer.write_string("layer", "foreground");
  else {
    writer.write_string("layer", "unknown");
    std::cerr << "Warning unknown layer in tilemap.\n";
  }

  writer.write_bool("solid", solid);
  writer.write_float("speed", speed);
  writer.write_int("width", width);
  writer.write_int("height", height);
  writer.write_int_vector("tiles", tiles);
  
  writer.end_list("tilemap");
}

void
TileMap::action(float )
{
}

void
TileMap::draw(DrawingContext& context)
{
  context.push_transform();

  if(vertical_flip)
    context.set_drawing_effect(VERTICAL_FLIP); 
  float trans_x = roundf(context.get_translation().x);
  float trans_y = roundf(context.get_translation().y);
  context.set_translation(Vector(trans_x * speed, trans_y * speed));

  /** if we don't round here, we'll have a 1 pixel gap on screen sometimes.
   * I have no idea why */
  float start_x = roundf(context.get_translation().x);
  float start_y = roundf(context.get_translation().y);
  float end_x = std::min(start_x + screen->w, float(width * 32));
  float end_y = std::min(start_y + screen->h, float(height * 32));
  start_x -= int(start_x) % 32;
  start_y -= int(start_y) % 32;  
  int tsx = int(start_x / 32); // tilestartindex x
  int tsy = int(start_y / 32); // tilestartindex y

  Vector pos;
  int tx, ty;
  for(pos.x = start_x, tx = tsx; pos.x < end_x; pos.x += 32, ++tx) {
    for(pos.y = start_y, ty = tsy; pos.y < end_y; pos.y += 32, ++ty) {
      const Tile* tile = tilemanager->get(tiles[ty*width + tx]);
      assert(tile != 0);
      tile->draw(context, pos, layer);
    }
  }

  context.pop_transform();
}

void
TileMap::set(int newwidth, int newheight, const std::vector<unsigned int>&newt,
    int newlayer, bool newsolid)
{
  assert(int(newt.size()) == newwidth * newheight);

  width  = newwidth;
  height = newheight;

  tiles.resize(newt.size());
  tiles = newt;

  layer  = newlayer;
  solid  = newsolid;
  if(solid)
    flags |= FLAG_SOLID;
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

void
TileMap::do_vertical_flip()
{
  // remap tiles vertically flipped
  for(int y = 0; y < height / 2; ++y) {
    for(int x = 0; x < width; ++x) {
      std::swap(tiles[y*width + x], tiles[(((height-1)*width) - (y*width)) + x]);
    }
  }

  vertical_flip = true;
}

const Tile*
TileMap::get_tile(int x, int y) const
{
  if(x < 0 || x >= width || y < 0 || y >= height) {
#ifdef DEBUG
    std::cout << "Warning: tile outside tilemap requested!\n";
#endif
    return tilemanager->get(0);
  }

  return tilemanager->get(tiles[y*width + x]);
}

const Tile*
TileMap::get_tile_at(const Vector& pos) const
{
  return get_tile(int(pos.x)/32, int(pos.y)/32);
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
  change(int(pos.x)/32, int(pos.y)/32, newtile);
}
