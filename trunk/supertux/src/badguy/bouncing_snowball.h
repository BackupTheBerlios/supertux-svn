#ifndef __BOUNCING_SNOWBALL_H__
#define __BOUNCING_SNOWBALL_H__

#include "badguy.h"

class BouncingSnowball : public BadGuy
{
public:
  BouncingSnowball(LispReader& reader);

  void activate();
  void write(LispWriter& writer);
  HitResponse collision_solid(GameObject& other, const CollisionHit& hit);

protected:
  bool collision_squished(Player& player);
};

#endif
