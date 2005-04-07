#ifndef __BOMB_H__
#define __BOMB_H__

#include "badguy.h"

class RocketExplosion : public BadGuy
{
public:
  RocketExplosion(const Vector& pos, Direction dir);

  void write(lisp::Writer& writer);
  HitResponse collision_solid(GameObject& other, const CollisionHit& hit);
  HitResponse collision_player(Player& player, const CollisionHit& hit);
  HitResponse collision_badguy(BadGuy& badguy, const CollisionHit& hit);
  void active_action(float elapsed_time);
  void kill_fall();
  void explode();

private:
  Timer2 timer;
};

#endif
