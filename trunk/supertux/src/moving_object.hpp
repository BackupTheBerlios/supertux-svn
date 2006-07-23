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
#ifndef SUPERTUX_MOVING_OBJECT_H
#define SUPERTUX_MOVING_OBJECT_H

#include <stdint.h>

#include "game_object.hpp"
#include "collision_hit.hpp"
#include "math/vector.hpp"
#include "math/rect.hpp"

class Sector;
class CollisionGrid;

enum CollisionGroup {
  /** Objects in DISABLED group are not tested for collisions */
  COLGROUP_DISABLED,
  /**
   * "default" is moving object. MovingObjects get tested against all other
   * objects and against other movingobjects
   */
  COLGROUP_MOVING,
  /**
   * a Moving object, that is not tested against other MovingObjects (or other
   * MovingOnlyStatic objects), but is tested against all other objects.
   */
  COLGROUP_MOVING_ONLY_STATIC,
  /**
   * Doesn't move and isn't explicitely checked for collisions with other
   * objects (but other objects might check with this)
   * The difference to COLGROUP_TOUCHABLE is that we can do multiple
   * collision response tests in a row which is needed for static object
   * that tux walks on. The results for collisions with STATIC objects
   * are also sorted by time (so that the first hit gets handled first).
   *
   * Use this for static obstacles
   */
  COLGROUP_STATIC,
  /**
   * Isn't explicitely checked for collisions with other objects. But other
   * objects might check with this object.
   * Difference to COLGROUP_STATIC is that collisions with this object are
   * only tested once and collision response is typically not handled
   *
   * Use this for touchable things like spikes/areas or collectibles like
   * coins
   */
  COLGROUP_TOUCHABLE,

  /**
   * Should be used for tilemaps
   */
  COLGROUP_TILEMAP
};

/**
 * Base class for all dynamic/moving game objects. This class contains things
 * for handling the bounding boxes and collision feedback.
 */
class MovingObject : public GameObject
{
public:
  MovingObject(std::string name = "");
  MovingObject(const lisp::Lisp& lisp);
  virtual ~MovingObject();

  /** this function is called when the object collided with something solid */
  virtual void collision_solid(const CollisionHit& hit)
  {
    (void) hit;
  }
  /** this function is called when the object collided with any other object */
  virtual HitResponse collision(GameObject& other, const CollisionHit& hit) = 0;
  /** called when tiles with special attributes have been touched */
  virtual void collision_tile(uint32_t tile_attributes)
  {
    (void) tile_attributes;
  }

  const Vector& get_pos() const
  {
    return bbox.p1;
  }

  /** returns the bounding box of the Object */
  const Rect& get_bbox() const
  {
    return bbox;
  }

  const Vector& get_movement() const
  {
    return movement;
  }

  /** places the moving object at a specific position. Be carefull when
   * using this function. There are no collision detection checks performed
   * here so bad things could happen.
   */
  virtual void set_pos(const Vector& pos)
  {
    dest.move(pos-get_pos());
    bbox.set_pos(pos);
  }

  /**
   * sets the moving object's bbox to a specific width. Be careful when
   * using this function. There are no collision detection checks performed
   * here so bad things could happen.
   */
  virtual void set_width(float w)
  {
    dest.set_width(w);
    bbox.set_width(w);
  }

  /**
   * sets the moving object's bbox to a specific size. Be careful when
   * using this function. There are no collision detection checks performed
   * here so bad things could happen.
   */
  /*virtual void set_size(float w, float h)
  {
    dest.set_size(w, h);
    bbox.set_size(w, h);
  }*/

  CollisionGroup get_group() const
  {
    return group;
  }

  void set_group(CollisionGroup group)
  {
    this->group = group;
  }

  // --- BEGIN METHODS TO EXPOSE TO SQUIRREL --- //
  void set_solid(bool solid)
  {
    this->solid = solid;
  }
  bool is_solid() const
  {
    return solid;
  }
  void move(float x, float y)
  {
    bbox.move(Vector(x, y));
  }
  void set_pos(float x, float y)
  {
    set_pos(Vector(x, y));
  }
  float get_pos_x() const
  {
    return bbox.get_left();
  }
  float get_pos_y() const
  {
    return bbox.get_top();
  }
  void set_size(float w, float h)
  {
    dest.set_size(w, h);
    bbox.set_size(w, h);
  }
  float get_width() const
  {
    return bbox.get_width();
  }
  float get_height() const
  {
    return bbox.get_height();
  }
  void set_velocity(float x, float y)
  {
    movement = Vector(x, y);
  }
  float get_velocity_x() const
  {
    return movement.x;
  }
  float get_velocity_y() const
  {
    return movement.y;
  }
  // --- END METHODS TO EXPOSE TO SQUIRREL --- //

protected:
  MovingObject(Rect bbox, CollisionGroup group, bool solid);
  friend class Sector;
  friend class CollisionGrid;
  friend class Platform;

  /** The bounding box of the object (as used for collision detection, this
   * isn't necessarily the bounding box for graphics)
   */
  Rect bbox;
  /** The movement that will happen till next frame
   */
  Vector movement;
  /** The collision group */
  CollisionGroup group;

private:
  /**
   * this is only here for internal collision detection use (don't touch this
   * from outside collision detection code)
   *
   * This field holds the currently anticipated destination of the object
   * during collision detection
   */
  Rect dest;
  
  bool solid; /**< true if this object should be considered when doing collision detection */
};

#endif
