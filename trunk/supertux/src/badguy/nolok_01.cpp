#include <config.h>

#include "nolok_01.h"
#include "badguy/bouncing_snowball.h"
#include "trigger/door.h"

#define WALK_TIME 2.5
#define SHOOT_TIME 0.4
#define JUMP_TIME 0.5
#define INITIAL_HITPOINTS 3

static const float WALKSPEED = 90;

//TODO: Create sprite, limit max number of snowballs
Nolok_01::Nolok_01(const lisp::Lisp& reader)
{
  reader.get("x", start_position.x);
  reader.get("y", start_position.y);
  bbox.set_size(31.8, 63.8);
  sprite = sprite_manager->create("dummyguy");
}

Nolok_01::Nolok_01(float pos_x, float pos_y)
{
  start_position.x = pos_x;
  start_position.y = pos_y;
  bbox.set_size(31.8, 63.8);
  sprite = sprite_manager->create("dummyguy");
}

void
Nolok_01::write(lisp::Writer& writer)
{
  writer.start_list("nolok_01");

  writer.write_float("x", start_position.x);
  writer.write_float("y", start_position.y);

  writer.end_list("nolok_01");
}

void
Nolok_01::activate()
{
  hitpoints = INITIAL_HITPOINTS;
  physic.set_velocity_x(dir == LEFT ? -WALKSPEED : WALKSPEED);
  sprite->set_action(dir == LEFT ? "left" : "right");
  action = WALKING;
  action_timer.start(WALK_TIME);
}

void
Nolok_01::active_action(float elapsed_time)
{
   if (action_timer.check()) {
     switch (action) {       
       case WALKING:
        {
         sprite->set_action("jump");
         physic.set_velocity_y(700);
         action = JUMPING;
         action_timer.start(JUMP_TIME);
         break;
        }
       case JUMPING:
       {
        sprite->set_action("throw");
        action = SHOOTING;
        action_timer.start(SHOOT_TIME);
        break;
       }
       case SHOOTING:
       {
        Sector::current()->add_object(new BouncingSnowball(get_pos().x - 64, get_pos().y, LEFT));
        Sector::current()->add_object(new BouncingSnowball(get_pos().x + 64, get_pos().y, RIGHT));
        physic.set_velocity_x(dir == LEFT ? -WALKSPEED : WALKSPEED);
        sprite->set_action(dir == LEFT ? "left" : "right");
        action = WALKING;
        action_timer.start(WALK_TIME);
        break;
       }
     }
   }
   movement = physic.get_movement(elapsed_time);
}

bool
Nolok_01::collision_squished(Player& player)
{
  bool result = false;
  player.bounce(*this);
  if (hitpoints <= 0) {
    sprite->set_action("dead"); 
    kill_squished(player);
    Sector::current()->add_object(new Door((int)get_pos().x+32, 512, "sector1", "main2"));
    result = true;
  }
  return result;
}

HitResponse
Nolok_01::collision_solid(GameObject& , const CollisionHit& hit)
{
  if(fabsf(hit.normal.y) > .5){ // hit floor or roof?
    if (action != JUMPING) physic.set_velocity_y(0);
  } else { // hit right or left
    dir = dir == LEFT ? RIGHT : LEFT;
    sprite->set_action(dir == LEFT ? "left" : "right");
    physic.set_velocity_x(-physic.get_velocity_x());
  }

  return CONTINUE;
}

//TODO: Hitpoint count incorrect when combining squishing and shooting
void
Nolok_01::kill_fall()
{
  if (hitpoints <= 0) {
   SoundManager::get()->play_sound(IDToSound(SND_FALL), this,
         Sector::current()->player->get_pos());
   physic.set_velocity_y(0);
   physic.enable_gravity(true);
   set_state(STATE_FALLING);
   Sector::current()->add_object(new Door((int)get_pos().x+32, 512, "sector1", "main2"));
  }
  else {hitpoints--;}
}

IMPLEMENT_FACTORY(Nolok_01, "nolok_01")
