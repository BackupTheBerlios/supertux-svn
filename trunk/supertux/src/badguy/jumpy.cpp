#include <config.h>

#include "jumpy.h"

static const float JUMPSPEED=600;
static const float JUMPY_MID_TOLERANCE=8;
static const float JUMPY_LOW_TOLERANCE=2;

Jumpy::Jumpy(const lisp::Lisp& reader)
{
  reader.get("x", start_position.x);
  reader.get("y", start_position.y);
  bbox.set_size(31.8, 31.8);
  sprite = sprite_manager->create("jumpy");
}

void
Jumpy::write(lisp::Writer& writer)
{
  writer.start_list("jumpy");

  writer.write_float("x", start_position.x);
  writer.write_float("y", start_position.y);

  writer.end_list("jumpy");
}

HitResponse
Jumpy::collision_solid(GameObject& , const CollisionHit& chit)
{
  return hit(chit);
}

HitResponse
Jumpy::collision_badguy(BadGuy& , const CollisionHit& chit)
{
  return hit(chit);
}

HitResponse
Jumpy::hit(const CollisionHit& chit)
{
  // hit floor?
  if(chit.normal.y < -.5) {
    physic.set_velocity_y(JUMPSPEED);
    SoundManager::get()->play_sound(IDToSound(SND_SKID));
  } else if(chit.normal.y < .5) { // bumped on roof
    physic.set_velocity_y(0);
  }

  return CONTINUE;
}

void
Jumpy::active_action(float elapsed_time)
{
  BadGuy::active_action(elapsed_time);
  
  dir = Sector::current()->player->get_pos().x > get_pos().x
    ? RIGHT : LEFT;
    //FIXME: add middle and up here
  
  if ( get_pos().y >= (start_position.y - JUMPY_MID_TOLERANCE) )
    sprite->set_action(dir == LEFT ? "left-middle" : "right-middle");
  else if ( get_pos().y >= (start_position.y - JUMPY_LOW_TOLERANCE) )
    sprite->set_action(dir == LEFT ? "left-down" : "right-down");
  else
    sprite->set_action(dir == LEFT ? "left-up" : "right-up");
}

IMPLEMENT_FACTORY(Jumpy, "jumpy")
