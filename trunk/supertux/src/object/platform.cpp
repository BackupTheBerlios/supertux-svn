#include <config.h>

#include "platform.h"
#include "video/drawing_context.h"
#include "resources.h"
#include "player.h"
#include "special/sprite_manager.h"
#include "lisp/lisp.h"
#include "lisp/writer.h"
#include "object_factory.h"

Platform::Platform(const lisp::Lisp& reader)
{
  sprite = sprite_manager->create("flying_platform");
  movement = Vector(0, 1);
  reader.get("x", bbox.p1.x);
  reader.get("y", bbox.p1.y);
  bbox.set_size(sprite->get_width(), sprite->get_height());

  flags |= FLAG_SOLID;

  state = 0;
}

Platform::~Platform()
{
  delete sprite;
}

HitResponse
Platform::collision(GameObject& , const CollisionHit& )
{
#if 0
  if(typeid(object) == typeid(Player)) {
    Player* player = (Player*) &object;
    //player->movement += movement;
  }
#endif
  return FORCE_MOVE;
}

void
Platform::action(float )
{
  // just some test code...
  if(state == 0) {
    movement = Vector(0, 1);
    if(bbox.p1.y > 250)
      state = 1;
  }
  if(state == 1) {
    movement = Vector(0, -1);
    if(bbox.p1.y < 50)
      state = 2;
  }
  if(state == 2) {
    movement = Vector(1, 0);
    if(bbox.p1.x > 800)
      state = 3;
  }
  if(state == 3) {
    movement = Vector(-1, 0);
    if(bbox.p1.x < 400)
      state = 4;
  }
  if(state == 4) {
    movement = Vector(-1, 1);
    if(bbox.p1.x < 0)
      state = 0;
  }
}

void
Platform::draw(DrawingContext& context)
{
  sprite->draw(context, get_pos(), LAYER_OBJECTS);
}

IMPLEMENT_FACTORY(Platform, "flying_platform");
