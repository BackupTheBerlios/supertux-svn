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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.
#ifndef __COLLISION_GRID_H__
#define __COLLISION_GRID_H__

#include <vector>
#include "moving_object.hpp"

class CollisionGridIterator;

/**
 * A rectangular grid to keep track of all moving game objects. It allows fast
 * queries for all objects in a rectangular area.
 */
class CollisionGrid
{
public:
  CollisionGrid(float width, float height);
  ~CollisionGrid();

  void add_object(MovingObject* object);
  void remove_object(MovingObject* object);

  void check_collisions();

private:
  friend class CollisionGridIterator;
  
  struct ObjectWrapper
  {
    MovingObject* object;
    Rect dest;
    /** (pseudo) timestamp. When reading from the grid the timestamp is
     * changed so that you can easily avoid reading an object multiple times
     * when it is in several cells that you check.
     */
    int timestamp;
    /// index in the objects vector
    int id;
  };
 
  /** Element for the single linked list in each grid cell */
  struct GridEntry
  {
    GridEntry* next;
    ObjectWrapper* object_wrapper;
  };

  void remove_object_from_gridcell(int gridcell, ObjectWrapper* wrapper);
  void collide_object(ObjectWrapper* wrapper);
  void collide_object_object(ObjectWrapper* wrapper, ObjectWrapper* wrapper2);
  void move_object(ObjectWrapper* wrapper);
  
  typedef std::vector<GridEntry*> GridEntries;
  GridEntries grid;
  typedef std::vector<ObjectWrapper*> Objects;
  Objects objects;
  size_t cells_x, cells_y;
  float width;
  float height;
  float cell_width;
  float cell_height;
  int iterator_timestamp;
};

extern CollisionGrid* bla;

#endif

