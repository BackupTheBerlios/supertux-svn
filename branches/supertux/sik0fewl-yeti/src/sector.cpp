//  $Id$
//
//  SuperTux -  A Jump'n Run
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

#include <memory>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <float.h>

#include "sector.hpp"
#include "player_status.hpp"
#include "object/gameobjs.hpp"
#include "object/camera.hpp"
#include "object/background.hpp"
#include "object/gradient.hpp"
#include "object/particlesystem.hpp"
#include "object/particlesystem_interactive.hpp"
#include "object/tilemap.hpp"
#include "lisp/parser.hpp"
#include "lisp/lisp.hpp"
#include "lisp/writer.hpp"
#include "lisp/list_iterator.hpp"
#include "tile.hpp"
#include "audio/sound_manager.hpp"
#include "game_session.hpp"
#include "resources.hpp"
#include "statistics.hpp"
#include "collision_grid.hpp"
#include "collision_grid_iterator.hpp"
#include "object_factory.hpp"
#include "collision.hpp"
#include "spawn_point.hpp"
#include "math/rect.hpp"
#include "math/aatriangle.hpp"
#include "object/coin.hpp"
#include "object/block.hpp"
#include "object/invisible_block.hpp"
#include "object/bullet.hpp"
#include "object/text_object.hpp"
#include "badguy/jumpy.hpp"
#include "trigger/sequence_trigger.hpp"
#include "player_status.hpp"
#include "scripting/squirrel_util.hpp"
#include "script_interface.hpp"
#include "log.hpp"

Sector* Sector::_current = 0;

bool Sector::show_collrects = false;
bool Sector::draw_solids_only = false;

Sector::Sector(Level* parent)
  : level(parent), currentmusic(LEVEL_MUSIC), gravity(10),
    player(0), solids(0), camera(0)
{
  add_object(new Player(player_status));
  add_object(new DisplayEffect());
  add_object(new TextObject());

#ifdef USE_GRID
  grid.reset(new CollisionGrid(32000, 32000));
#endif

  // create a new squirrel table for the sector
  using namespace Scripting;

  sq_collectgarbage(global_vm);

  sq_newtable(global_vm);
  sq_pushroottable(global_vm);
  if(SQ_FAILED(sq_setdelegate(global_vm, -2)))
    throw Scripting::SquirrelError(global_vm, "Couldn't set sector_table delegate");

  sq_resetobject(&sector_table);
  if(SQ_FAILED(sq_getstackobj(global_vm, -1, &sector_table)))
    throw Scripting::SquirrelError(global_vm, "Couldn't get sector table");
  sq_addref(global_vm, &sector_table);
  sq_pop(global_vm, 1);
}

Sector::~Sector()
{
  using namespace Scripting;
  
  deactivate();

  for(ScriptList::iterator i = scripts.begin();
      i != scripts.end(); ++i) {
    HSQOBJECT& object = *i;
    sq_release(global_vm, &object);
  }
  sq_release(global_vm, &sector_table);
  sq_collectgarbage(global_vm);
 
  update_game_objects();
  assert(gameobjects_new.size() == 0);

  for(GameObjects::iterator i = gameobjects.begin();
      i != gameobjects.end(); ++i) {
    GameObject* object = *i;
    before_object_remove(object);
    object->unref();
  }

  for(SpawnPoints::iterator i = spawnpoints.begin(); i != spawnpoints.end();
      ++i)
    delete *i;
}

Level*
Sector::get_level()
{
  return level;
}

GameObject*
Sector::parse_object(const std::string& name, const lisp::Lisp& reader)
{
  if(name == "camera") {
    Camera* camera = new Camera(this);
    camera->parse(reader);
    return camera;
  } else if(name == "particles-snow") {
    SnowParticleSystem* partsys = new SnowParticleSystem();
    partsys->parse(reader);
    return partsys;
  } else if(name == "particles-rain") {
    RainParticleSystem* partsys = new RainParticleSystem();
    partsys->parse(reader);
    return partsys;
  } else if(name == "particles-comets") {
    CometParticleSystem* partsys = new CometParticleSystem();
    partsys->parse(reader);
    return partsys;
  } else if(name == "particles-ghosts") {
    GhostParticleSystem* partsys = new GhostParticleSystem();
    partsys->parse(reader);
    return partsys;
  } else if(name == "particles-clouds") {
    CloudParticleSystem* partsys = new CloudParticleSystem();
    partsys->parse(reader);
    return partsys;
  } else if(name == "money") { // for compatibility with old maps
    return new Jumpy(reader);
  } 

  try {
    return create_object(name, reader);
  } catch(std::exception& e) {
    log_warning << e.what() << "" << std::endl;
  }
  
  return 0;
}

void
Sector::parse(const lisp::Lisp& sector)
{  
  lisp::ListIterator iter(&sector);
  while(iter.next()) {
    const std::string& token = iter.item();
    if(token == "name") {
      iter.value()->get(name);
    } else if(token == "gravity") {
      iter.value()->get(gravity);
    } else if(token == "music") {
      iter.value()->get(music);
    } else if(token == "spawnpoint") {
      SpawnPoint* sp = new SpawnPoint(iter.lisp());
      spawnpoints.push_back(sp);
    } else if(token == "init-script") {
      iter.value()->get(init_script);
    } else {
      GameObject* object = parse_object(token, *(iter.lisp()));
      if(object) {
        add_object(object);
      }
    }
  }

  update_game_objects();

  if(!solids)
    throw std::runtime_error("sector does not contain a solid tile layer.");

  fix_old_tiles();
  if(!camera) {
    log_warning << "sector '" << name << "' does not contain a camera." << std::endl;
    update_game_objects();
    add_object(new Camera(this));
  }

  update_game_objects();
}

void
Sector::parse_old_format(const lisp::Lisp& reader)
{
  name = "main";
  reader.get("gravity", gravity);

  std::string backgroundimage;
  reader.get("background", backgroundimage);
  float bgspeed = .5;
  reader.get("bkgd_speed", bgspeed);
  bgspeed /= 100;

  Color bkgd_top, bkgd_bottom;
  int r = 0, g = 0, b = 128;
  reader.get("bkgd_red_top", r);
  reader.get("bkgd_green_top",  g);
  reader.get("bkgd_blue_top",  b);
  bkgd_top.red = static_cast<float> (r) / 255.0f;
  bkgd_top.green = static_cast<float> (g) / 255.0f;
  bkgd_top.blue = static_cast<float> (b) / 255.0f;
  
  reader.get("bkgd_red_bottom",  r);
  reader.get("bkgd_green_bottom", g);
  reader.get("bkgd_blue_bottom", b);
  bkgd_bottom.red = static_cast<float> (r) / 255.0f;
  bkgd_bottom.green = static_cast<float> (g) / 255.0f;
  bkgd_bottom.blue = static_cast<float> (b) / 255.0f;
  
  if(backgroundimage != "") {
    Background* background = new Background();
    background->set_image(
            std::string("images/background/") + backgroundimage, bgspeed);
    add_object(background);
  } else {
    Gradient* gradient = new Gradient();
    gradient->set_gradient(bkgd_top, bkgd_bottom);
    add_object(gradient);
  }

  std::string particlesystem;
  reader.get("particle_system", particlesystem);
  if(particlesystem == "clouds")
    add_object(new CloudParticleSystem());
  else if(particlesystem == "snow")
    add_object(new SnowParticleSystem());
  else if(particlesystem == "rain")
    add_object(new RainParticleSystem());

  Vector startpos(100, 170);
  reader.get("start_pos_x", startpos.x);
  reader.get("start_pos_y", startpos.y);

  SpawnPoint* spawn = new SpawnPoint;
  spawn->pos = startpos;
  spawn->name = "main";
  spawnpoints.push_back(spawn);

  music = "chipdisko.ogg";
  reader.get("music", music);
  music = "music/" + music;

  int width = 30, height = 15;
  reader.get("width", width);
  reader.get("height", height);
  
  std::vector<unsigned int> tiles;
  if(reader.get_vector("interactive-tm", tiles)
      || reader.get_vector("tilemap", tiles)) {
    TileMap* tilemap = new TileMap();
    tilemap->set(width, height, tiles, LAYER_TILES, true);
    add_object(tilemap);
  }

  if(reader.get_vector("background-tm", tiles)) {
    TileMap* tilemap = new TileMap();
    tilemap->set(width, height, tiles, LAYER_BACKGROUNDTILES, false);
    add_object(tilemap);
  }

  if(reader.get_vector("foreground-tm", tiles)) {
    TileMap* tilemap = new TileMap();
    tilemap->set(width, height, tiles, LAYER_FOREGROUNDTILES, false);
    add_object(tilemap);
  }

  // read reset-points (now spawn-points)
  const lisp::Lisp* resetpoints = reader.get_lisp("reset-points");
  if(resetpoints) {
    lisp::ListIterator iter(resetpoints);
    while(iter.next()) {
      if(iter.item() == "point") {
        Vector sp_pos;
        if(reader.get("x", sp_pos.x) && reader.get("y", sp_pos.y))
          {
          SpawnPoint* sp = new SpawnPoint;
          sp->name = "main";
          sp->pos = sp_pos;
          spawnpoints.push_back(sp);
          }
      } else {
        log_warning << "Unknown token '" << iter.item() << "' in reset-points." << std::endl;
      }
    }
  }

  // read objects
  const lisp::Lisp* objects = reader.get_lisp("objects");
  if(objects) {
    lisp::ListIterator iter(objects);
    while(iter.next()) {
      GameObject* object = parse_object(iter.item(), *(iter.lisp()));
      if(object) {
        add_object(object);
      } else {
        log_warning << "Unknown object '" << iter.item() << "' in level." << std::endl;
      }
    }
  }

  // add a camera
  Camera* camera = new Camera(this);
  add_object(camera);

  update_game_objects();

  if(solids == 0)
    throw std::runtime_error("sector does not contain a solid tile layer.");

  fix_old_tiles();
  update_game_objects();
}

void
Sector::fix_old_tiles()
{
  // hack for now...
  for(size_t x=0; x < solids->get_width(); ++x) {
    for(size_t y=0; y < solids->get_height(); ++y) {
      const Tile* tile = solids->get_tile(x, y);
      Vector pos(x*32, y*32);
      
      if(tile->getID() == 112) {
        add_object(new InvisibleBlock(pos));
        solids->change(x, y, 0);
      } else if(tile->getAttributes() & Tile::COIN) {
        add_object(new Coin(pos));
        solids->change(x, y, 0);
      } else if(tile->getAttributes() & Tile::FULLBOX) {
        add_object(new BonusBlock(pos, tile->getData()));
        solids->change(x, y, 0);
      } else if(tile->getAttributes() & Tile::BRICK) {
        add_object(new Brick(pos, tile->getData()));
        solids->change(x, y, 0);
      } else if(tile->getAttributes() & Tile::GOAL) {
        std::string sequence = tile->getData() == 0 ? "endsequence" : "stoptux";
        add_object(new SequenceTrigger(pos, sequence));
        solids->change(x, y, 0);
      }
    }
  }
}

void
Sector::write(lisp::Writer& writer)
{
  writer.write_string("name", name);
  writer.write_float("gravity", gravity);
  writer.write_string("music", music);

  // write spawnpoints
  for(SpawnPoints::iterator i = spawnpoints.begin(); i != spawnpoints.end();
      ++i) {
    SpawnPoint* spawn = *i;
    writer.start_list("spawn-points");
    writer.write_string("name", spawn->name);
    writer.write_float("x", spawn->pos.x);
    writer.write_float("y", spawn->pos.y);
    writer.end_list("spawn-points");
  }

  // write objects
  for(GameObjects::iterator i = gameobjects.begin();
      i != gameobjects.end(); ++i) {
    Serializable* serializable = dynamic_cast<Serializable*> (*i);
    if(serializable)
      serializable->write(writer);
  }
}

HSQUIRRELVM
Sector::run_script(std::istream& in, const std::string& sourcename)
{
  using namespace Scripting;

  // garbage collect thread list
  for(ScriptList::iterator i = scripts.begin();
      i != scripts.end(); ) {
    HSQOBJECT& object = *i;
    HSQUIRRELVM vm = object_to_vm(object);

    if(sq_getvmstate(vm) != SQ_VMSTATE_SUSPENDED) {
      sq_release(global_vm, &object);
      i = scripts.erase(i);
      continue;
    }
    
    ++i;
  }
  
  HSQOBJECT object = create_thread(global_vm);
  scripts.push_back(object);

  HSQUIRRELVM vm = object_to_vm(object);

  // set sector_table as roottable for the thread
  sq_pushobject(vm, sector_table);
  sq_setroottable(vm);

  compile_and_run(vm, in, sourcename);

  return vm;
}

void
Sector::add_object(GameObject* object)
{
  // make sure the object isn't already in the list
#ifdef DEBUG
  for(GameObjects::iterator i = gameobjects.begin(); i != gameobjects.end();
      ++i) {
    if(*i == object) {
      assert("object already added to sector" == 0);
    }
  }
  for(GameObjects::iterator i = gameobjects_new.begin();
      i != gameobjects_new.end(); ++i) {
    if(*i == object) {
      assert("object already added to sector" == 0);
    }
  }
#endif

  object->ref();
  gameobjects_new.push_back(object);
}

void
Sector::activate(const std::string& spawnpoint)
{
  SpawnPoint* sp = 0;
  for(SpawnPoints::iterator i = spawnpoints.begin(); i != spawnpoints.end();
      ++i) {
    if((*i)->name == spawnpoint) {
      sp = *i;
      break;
    }
  }                                                                           
  if(!sp) {
    log_warning << "Spawnpoint '" << spawnpoint << "' not found." << std::endl;
    if(spawnpoint != "main") {
      activate("main");
    } else {
      activate(Vector(0, 0));
    }
  } else {
    activate(sp->pos);
  }
}

void
Sector::activate(const Vector& player_pos)
{
  if(_current != this) {
    if(_current != NULL)
      _current->deactivate();
    _current = this;

    // register sectortable as sector in scripting
    HSQUIRRELVM vm = Scripting::global_vm;
    sq_pushroottable(vm);
    sq_pushstring(vm, "sector", -1);
    sq_pushobject(vm, sector_table);
    if(SQ_FAILED(sq_createslot(vm, -3)))
      throw Scripting::SquirrelError(vm, "Couldn't set sector in roottable");
    sq_pop(vm, 1);

    for(GameObjects::iterator i = gameobjects.begin();
        i != gameobjects.end(); ++i) {
      GameObject* object = *i;

      try_expose(object);
    }
  }

  player->move(player_pos);
  camera->reset(player->get_pos());
  update_game_objects();

  // Run init script
  if(init_script != "") {
    std::istringstream in(init_script);
    run_script(in, std::string("Sector(") + name + ") - init");
  }
}

void
Sector::deactivate()
{
  if(_current != this)
    return;

  // remove sector entry from global vm
  HSQUIRRELVM vm = Scripting::global_vm;
  sq_pushroottable(vm);
  sq_pushstring(vm, "sector", -1);
  if(SQ_FAILED(sq_deleteslot(vm, -2, SQFalse)))
    throw Scripting::SquirrelError(vm, "Couldn't unset sector in roottable");
  sq_pop(vm, 1);
  
  for(GameObjects::iterator i = gameobjects.begin();
      i != gameobjects.end(); ++i) {
    GameObject* object = *i;
    
    try_unexpose(object);
  }

  _current = NULL;
}

Rect
Sector::get_active_region()
{
  return Rect(
    camera->get_translation() - Vector(1600, 1200),
    camera->get_translation() + Vector(1600, 1200));
}

void
Sector::update(float elapsed_time)
{
  player->check_bounds(camera);

#if 0
  CollisionGridIterator iter(*grid, get_active_region());
  while(MovingObject* object = iter.next()) {
    if(!object->is_valid())
      continue;

    object->update(elapsed_time);
  }
#else
  /* update objects */
  for(GameObjects::iterator i = gameobjects.begin();
          i != gameobjects.end(); ++i) {
    GameObject* object = *i;
    if(!object->is_valid())
      continue;
    
    object->update(elapsed_time);
  }
#endif
  
  /* Handle all possible collisions. */
  handle_collisions();
  update_game_objects();
}

void
Sector::update_game_objects()
{
  /** cleanup marked objects */
  for(std::vector<Bullet*>::iterator i = bullets.begin();
      i != bullets.end(); /* nothing */) {
    Bullet* bullet = *i;
    if(bullet->is_valid()) {
      ++i;
      continue;
    }

    i = bullets.erase(i);
  }
  for(MovingObjects::iterator i = moving_objects.begin();
      i != moving_objects.end(); /* nothing */) {
    MovingObject* moving_object = *i;
    if(moving_object->is_valid()) {
      ++i;
      continue;
    }

#ifdef USE_GRID
    grid->remove_object(moving_object);
#endif
    
    i = moving_objects.erase(i);
  }
  for(std::vector<GameObject*>::iterator i = gameobjects.begin();
      i != gameobjects.end(); /* nothing */) {
    GameObject* object = *i;
    
    if(object->is_valid()) {
      ++i;
      continue;
    }

    before_object_remove(object);
    
    object->unref();
    i = gameobjects.erase(i);
  }

  /* add newly created objects */
  for(std::vector<GameObject*>::iterator i = gameobjects_new.begin();
      i != gameobjects_new.end(); ++i)
  {
    GameObject* object = *i;

    before_object_add(object);
    
    gameobjects.push_back(object);
  }
  gameobjects_new.clear();
}

bool
Sector::before_object_add(GameObject* object)
{
  Bullet* bullet = dynamic_cast<Bullet*> (object);
  if(bullet)
    bullets.push_back(bullet);

  MovingObject* movingobject = dynamic_cast<MovingObject*> (object);
  if(movingobject) {
    moving_objects.push_back(movingobject);
#ifdef USE_GRID
    grid->add_object(movingobject);
#endif
  }
  
  TileMap* tilemap = dynamic_cast<TileMap*> (object);
  if(tilemap && tilemap->is_solid()) {
    if(solids == 0) {
      solids = tilemap;
    } else {
      log_warning << "Another solid tilemaps added. Ignoring" << std::endl;
    }
  }

  Camera* camera = dynamic_cast<Camera*> (object);
  if(camera) {
    if(this->camera != 0) {
      log_warning << "Multiple cameras added. Ignoring" << std::endl;
      return false;
    }
    this->camera = camera;
  }

  Player* player = dynamic_cast<Player*> (object);
  if(player) {
    if(this->player != 0) {
      log_warning << "Multiple players added. Ignoring" << std::endl;
      return false;
    }
    this->player = player;
  }

  if(_current == this) {
    try_expose(object);
  }
  
  return true;
}

void
Sector::try_expose(GameObject* object)
{
  ScriptInterface* interface = dynamic_cast<ScriptInterface*> (object);
  if(interface != NULL) {
    HSQUIRRELVM vm = Scripting::global_vm;
    sq_pushobject(vm, sector_table);
    interface->expose(vm, -1);
    sq_pop(vm, 1);
  }
}

void
Sector::before_object_remove(GameObject* object)
{
  if(_current == this)
    try_unexpose(object);
}

void
Sector::try_unexpose(GameObject* object)
{
  ScriptInterface* interface = dynamic_cast<ScriptInterface*> (object);
  if(interface != NULL) {
    HSQUIRRELVM vm = Scripting::global_vm;
    int oldtop = sq_gettop(vm);
    sq_pushobject(vm, sector_table);
    try {
      interface->unexpose(vm, -1);
    } catch(std::exception& e) {
      log_warning << "Couldn't unregister object: " << e.what() << std::endl;
    }
    sq_settop(vm, oldtop);
  }
} 

void
Sector::draw(DrawingContext& context)
{
  context.push_transform();
  context.set_translation(camera->get_translation());

  for(GameObjects::iterator i = gameobjects.begin();
      i != gameobjects.end(); ++i) {
    GameObject* object = *i; 
    if(!object->is_valid())
      continue;

    if (draw_solids_only)
    {
      TileMap* tm = dynamic_cast<TileMap*>(object);
      if (tm && !tm->is_solid())
        continue;
    }

    object->draw(context);
  }

  if(show_collrects) {
    Color col(0.2, 0.2, 0.2, 0.7);
    for(MovingObjects::iterator i = moving_objects.begin();
            i != moving_objects.end(); ++i) {
      MovingObject* object = *i;
      const Rect& rect = object->get_bbox();

      context.draw_filled_rect(rect, col, LAYER_FOREGROUND1 + 10);
    }
  }

  context.pop_transform();
}

static const float DELTA = .001;

void
Sector::collision_tilemap(const Rect& dest, const Vector& movement,
                          CollisionHit& hit) const
{
  // calculate rectangle where the object will move
  float x1 = dest.get_left();
  float x2 = dest.get_right();
  float y1 = dest.get_top();
  float y2 = dest.get_bottom();

  // test with all tiles in this rectangle
  int starttilex = int(x1) / 32;
  int starttiley = int(y1) / 32;
  int max_x = int(x2 + (1 - DELTA));
  int max_y = int(y2 + (1 - DELTA));

  CollisionHit temphit;
  for(int x = starttilex; x*32 < max_x; ++x) {
    for(int y = starttiley; y*32 < max_y; ++y) {
      const Tile* tile = solids->get_tile(x, y);
      if(!tile)
        continue;
      // skip non-solid tiles
      if(tile->getAttributes() == 0)
        continue;
      // only handle unisolid when the player is falling down and when he was
      // above the tile before
      if(tile->getAttributes() & Tile::UNISOLID) {
        if(movement.y < 0 || dest.get_top() - movement.y > y*32)
          continue;
      }

      if(tile->getAttributes() & Tile::SLOPE) { // slope tile
        AATriangle triangle;
        Vector p1(x*32, y*32);
        Vector p2((x+1)*32, (y+1)*32);
        triangle = AATriangle(p1, p2, tile->getData());

        if(Collision::rectangle_aatriangle(temphit, dest, movement,
              triangle)) {
          if(temphit.time > hit.time && (tile->getAttributes() & Tile::SOLID)) {
            hit = temphit;
          }
        }
      } else { // normal rectangular tile
        Rect rect(x*32, y*32, (x+1)*32, (y+1)*32);
        if(Collision::rectangle_rectangle(temphit, dest, movement, rect)) {
          if(temphit.time > hit.time && (tile->getAttributes() & Tile::SOLID)) {
            hit = temphit;
          }
        }
      }
    }
  }
}

uint32_t
Sector::collision_tile_attributes(const Rect& dest) const
{
  /** XXX This function doesn't work correctly as it will check all tiles
   * in the bounding box of the object movement, this might include tiles
   * that have actually never been touched by the object
   * (though this only occures for very fast objects...)
   */
 
#if 0
  // calculate rectangle where the object will move
  float x1, x2;
  if(object->get_movement().x >= 0) {
    x1 = object->get_bbox().p1.x;
    x2 = object->get_bbox().p2.x + object->get_movement().x;
  } else {
    x1 = object->get_bbox().p1.x + object->get_movement().x;
    x2 = object->get_bbox().p2.x;
  }
  float y1, y2;
  if(object->get_movement().y >= 0) {
    y1 = object->get_bbox().p1.y;
    y2 = object->get_bbox().p2.y + object->get_movement().y;
  } else {
    y1 = object->get_bbox().p1.y + object->get_movement().y;
    y2 = object->get_bbox().p2.y;
  }
#endif
  float x1 = dest.p1.x;
  float y1 = dest.p1.y;
  float x2 = dest.p2.x;
  float y2 = dest.p2.y;

  // test with all tiles in this rectangle
  int starttilex = int(x1) / 32;
  int starttiley = int(y1) / 32;
  int max_x = int(x2);
  int max_y = int(y2);

  uint32_t result = 0;
  for(int x = starttilex; x*32 < max_x; ++x) {
    for(int y = starttiley; y*32 < max_y; ++y) {
      const Tile* tile = solids->get_tile(x, y);
      if(!tile)
        continue;
      result |= tile->getAttributes();
    }
  }

  return result;
}

void
Sector::collision_object(MovingObject* object1, MovingObject* object2) const
{
  CollisionHit hit;

  Vector movement = object1->get_movement() - object2->get_movement();
  if(Collision::rectangle_rectangle(hit, object1->dest, movement, object2->dest)) {
    HitResponse response1 = object1->collision(*object2, hit);
    hit.normal *= -1;
    HitResponse response2 = object2->collision(*object1, hit);

    if(response1 != CONTINUE) {
      if(response1 == ABORT_MOVE)
        object1->dest = object1->get_bbox();
      if(response2 == CONTINUE)
        object2->dest.move(hit.normal * (hit.depth + DELTA));
    } else if(response2 != CONTINUE) {
      if(response2 == ABORT_MOVE)
        object2->dest = object2->get_bbox();
      if(response1 == CONTINUE)
        object1->dest.move(-hit.normal * (hit.depth + DELTA));
    } else {
      object1->dest.move(-hit.normal * (hit.depth/2 + DELTA));
      object2->dest.move(hit.normal * (hit.depth/2 + DELTA));
    }
  }
}

bool
Sector::collision_static(MovingObject* object, const Vector& movement)
{
  GameObject* collided_with = solids;
  CollisionHit hit;
  hit.time = -1;

  collision_tilemap(object->dest, movement, hit);

  // collision with other (static) objects
  CollisionHit temphit;
  for(MovingObjects::iterator i2 = moving_objects.begin();
      i2 != moving_objects.end(); ++i2) {
    MovingObject* moving_object_2 = *i2;
    if(moving_object_2->get_group() != COLGROUP_STATIC
        || !moving_object_2->is_valid())
      continue;
        
    Rect dest = moving_object_2->dest;

    Vector rel_movement 
      = movement - moving_object_2->get_movement();

    if(Collision::rectangle_rectangle(temphit, object->dest, rel_movement, dest)
        && temphit.time > hit.time) {
      hit = temphit;
      collided_with = moving_object_2;
    }
  }

  if(hit.time < 0)
    return true;

  HitResponse response = object->collision(*collided_with, hit);
  hit.normal *= -1;
  if(collided_with != solids) {
    MovingObject* moving_object = (MovingObject*) collided_with;
    HitResponse other_response = moving_object->collision(*object, hit);
    if(other_response == ABORT_MOVE) {
      moving_object->dest = moving_object->get_bbox();
    } else if(other_response == FORCE_MOVE) {
      // the static object "wins" move tux out of the collision
      object->dest.move(-hit.normal * (hit.depth + DELTA));
      return false;
    } else if(other_response == PASS_MOVEMENT) {
      object->dest.move(moving_object->get_movement());
      //object->movement += moving_object->get_movement();
    }
  }

  if(response == CONTINUE) {
    object->dest.move(-hit.normal * (hit.depth + DELTA));
    return false;
  } else if(response == ABORT_MOVE) {
    object->dest = object->get_bbox();
    return true;
  }
  
  // force move
  return false;
}

void
Sector::handle_collisions()
{
  // calculate destination positions of the objects
  for(MovingObjects::iterator i = moving_objects.begin();
      i != moving_objects.end(); ++i) {
    MovingObject* moving_object = *i;

    moving_object->dest = moving_object->get_bbox();
    moving_object->dest.move(moving_object->get_movement());
  }
    
  // part1: COLGROUP_MOVING vs COLGROUP_STATIC and tilemap
  //   we do this up to 4 times and have to sort all results for the smallest
  //   one before we can continue here
  for(MovingObjects::iterator i = moving_objects.begin();
      i != moving_objects.end(); ++i) {
    MovingObject* moving_object = *i;
    if((moving_object->get_group() != COLGROUP_MOVING
          && moving_object->get_group() != COLGROUP_MOVING_ONLY_STATIC)
        || !moving_object->is_valid())
      continue;

    Vector movement = moving_object->get_movement();

    // test if x or y movement is dominant
    if(fabsf(moving_object->get_movement().x) < fabsf(moving_object->get_movement().y)) {

      // test in x direction first, then y direction
      moving_object->dest.move(Vector(0, -movement.y));
      for(int i = 0; i < 2; ++i) {
        bool res = collision_static(moving_object, Vector(movement.x, 0));
        if(res)
          break;
      }
      moving_object->dest.move(Vector(0, movement.y));
      for(int i = 0; i < 2; ++i) {
        bool res = collision_static(moving_object, Vector(0, movement.y));
        if(res)
          break;
      }
      
    } else {

      // test in y direction first, then x direction
      moving_object->dest.move(Vector(-movement.x, 0));
      for(int i = 0; i < 2; ++i) {
        bool res = collision_static(moving_object, Vector(0, movement.y));
        if(res)
          break;
      }
      moving_object->dest.move(Vector(movement.x, 0)); 
      for(int i = 0; i < 2; ++i) {
        bool res = collision_static(moving_object, Vector(movement.x, 0));
        if(res)
          break;
      }
    }
  }

  // part2: COLGROUP_MOVING vs tile attributes
  for(MovingObjects::iterator i = moving_objects.begin();
      i != moving_objects.end(); ++i) {
    MovingObject* moving_object = *i;
    if((moving_object->get_group() != COLGROUP_MOVING
          && moving_object->get_group() != COLGROUP_MOVING_ONLY_STATIC)
        || !moving_object->is_valid())
      continue;

    uint32_t tile_attributes = collision_tile_attributes(moving_object->dest);
    if(tile_attributes > Tile::FIRST_INTERESTING_FLAG) {
      moving_object->collision_tile(tile_attributes);
    }
  }

  // part2.5: COLGROUP_MOVING vs COLGROUP_TOUCHABLE
  for(MovingObjects::iterator i = moving_objects.begin();
      i != moving_objects.end(); ++i) {
    MovingObject* moving_object = *i;
    if(moving_object->get_group() != COLGROUP_MOVING
        || !moving_object->is_valid())
      continue;

    for(MovingObjects::iterator i2 = moving_objects.begin();
        i2 != moving_objects.end(); ++i2) {
      MovingObject* moving_object_2 = *i2;
      if(moving_object_2->get_group() != COLGROUP_TOUCHABLE
         || !moving_object_2->is_valid())
        continue;

      collision_object(moving_object, moving_object_2);
    } 
  }

  // part3: COLGROUP_MOVING vs COLGROUP_MOVING
  for(MovingObjects::iterator i = moving_objects.begin();
      i != moving_objects.end(); ++i) {
    MovingObject* moving_object = *i;

    if(moving_object->get_group() != COLGROUP_MOVING
        || !moving_object->is_valid())
      continue;

    for(MovingObjects::iterator i2 = i+1;
        i2 != moving_objects.end(); ++i2) {
      MovingObject* moving_object_2 = *i2;
      if(moving_object_2->get_group() != COLGROUP_MOVING
         || !moving_object_2->is_valid())
        continue;

      collision_object(moving_object, moving_object_2);
    }    
  }

  // apply object movement
  for(MovingObjects::iterator i = moving_objects.begin();
      i != moving_objects.end(); ++i) {
    MovingObject* moving_object = *i;

    moving_object->bbox = moving_object->dest;
    moving_object->movement = Vector(0, 0);
  }
}

bool
Sector::is_free_space(const Rect& rect) const
{
  // test with all tiles in this rectangle
  int starttilex = int(rect.p1.x) / 32;
  int starttiley = int(rect.p1.y) / 32;
  int max_x = int(rect.p2.x);
  int max_y = int(rect.p2.y);

  for(int x = starttilex; x*32 <= max_x; ++x) {
    for(int y = starttiley; y*32 <= max_y; ++y) {
      const Tile* tile = solids->get_tile(x, y);
      if(!tile)
        continue;
      if(tile->getAttributes() & Tile::SOLID)
        return false;
    }
  }

  for(MovingObjects::const_iterator i = moving_objects.begin();
      i != moving_objects.end(); ++i) {
    const MovingObject* moving_object = *i;
    if(moving_object->get_group() != COLGROUP_STATIC
        || !moving_object->is_valid())
      continue;

    if(Collision::intersects(rect, moving_object->get_bbox()))
      return false;
  }

  return true;
}

bool
Sector::add_bullet(const Vector& pos, float xm, Direction dir)
{
  // TODO remove this function and move these checks elsewhere...

  Bullet* new_bullet = 0;
  if(player_status->bonus == FIRE_BONUS) {
    if((int)bullets.size() >= player_status->max_fire_bullets)
      return false;
    new_bullet = new Bullet(pos, xm, dir, FIRE_BULLET);
  } else if(player_status->bonus == ICE_BONUS) {
    if((int)bullets.size() >= player_status->max_ice_bullets)
      return false;
    new_bullet = new Bullet(pos, xm, dir, ICE_BULLET);
  } else {
    return false;
  }
  add_object(new_bullet);

  sound_manager->play("sounds/shoot.wav");

  return true;
}

bool
Sector::add_smoke_cloud(const Vector& pos)
{
  add_object(new SmokeCloud(pos));
  return true;
}

void
Sector::add_floating_text(const Vector& pos, const std::string& text)
{
  add_object(new FloatingText(pos, text));
}

void
Sector::play_music(MusicType type)
{
  currentmusic = type;
  switch(currentmusic) {
    case LEVEL_MUSIC:
      sound_manager->play_music(music);
      break;
    case HERRING_MUSIC:
      sound_manager->play_music("music/salcon.ogg");
      break;
    case HERRING_WARNING_MUSIC:
      sound_manager->stop_music(TUX_INVINCIBLE_TIME_WARNING);
      break;
    default:
      sound_manager->play_music("");
      break;
  }
}

MusicType
Sector::get_music_type()
{
  return currentmusic;
}

int
Sector::get_total_badguys()
{
  int total_badguys = 0;
  for(GameObjects::iterator i = gameobjects.begin();
      i != gameobjects.end(); ++i) {
    BadGuy* badguy = dynamic_cast<BadGuy*> (*i);
    if (badguy && badguy->countMe)
      total_badguys++;
  }

  return total_badguys;
}

bool
Sector::inside(const Rect& rect) const
{
  if(rect.p1.x > solids->get_width() * 32 
      || rect.p1.y > solids->get_height() * 32
      || rect.p2.x < 0)
    return false;

  return true;
}
