//  $Id: hurting_platform.hpp 3506 2006-05-12 01:41:09Z sommer $
//
//  SuperTux - Hurting Platform
//  Copyright (C) 2006 Christoph Sommer <christoph.sommer@2006.expires.deltadevelopment.de>
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

#ifndef __HURTING_PLATFORM_H__
#define __HURTING_PLATFORM_H__

#include <memory>
#include "object/platform.hpp"

/**
 * Platform that hurts Tux and Badguys when touched
 */
class HurtingPlatform : public Platform
{
public:
  HurtingPlatform(const lisp::Lisp& reader);
  virtual HurtingPlatform* clone() const { return new HurtingPlatform(*this); }

  virtual HitResponse collision(GameObject& other, const CollisionHit& hit);

private:

};

#endif

