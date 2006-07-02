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

#include <math.h>
#include "flower.hpp"
#include "resources.hpp"
#include "camera.hpp"
#include "sector.hpp"
#include "player.hpp"
#include "audio/sound_manager.hpp"
#include "sprite/sprite_manager.hpp"

Flower::Flower(Type _type)
  : type(_type)
{
  bbox.set_size(32, 32);

  if(_type == FIREFLOWER){
    sprite = sprite_manager->create("images/powerups/fireflower/fireflower.sprite");
    sound_manager->preload("sounds/fire-flower.wav");
  }
  else
    sprite = sprite_manager->create("images/powerups/iceflower/iceflower.sprite"); 

  set_group(COLGROUP_TOUCHABLE);
}

Flower::~Flower()
{
  delete sprite;
}

void
Flower::update(float )
{
}

void
Flower::draw(DrawingContext& context)
{
  sprite->draw(context, get_pos(), LAYER_OBJECTS);
}

HitResponse
Flower::collision(GameObject& other, const CollisionHit& )
{
  Player* player = dynamic_cast<Player*>(&other);
  if(!player)
    return ABORT_MOVE;

  if(type == FIREFLOWER)
    player->add_bonus(FIRE_BONUS, true);
  else
    player->add_bonus(ICE_BONUS, true);
  
  sound_manager->play("sounds/fire-flower.wav");
  remove_me();
  return ABORT_MOVE;
}

