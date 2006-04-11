#include <config.h>

#include <stdexcept>
#include "resources.hpp"
#include "main.hpp"
#include "math/rect.hpp"
#include "sprite/sprite_manager.hpp"
#include "sprite/sprite.hpp"
#include "video/drawing_context.hpp"
#include "lisp/lisp.hpp"
#include "floating_image.hpp"

FloatingImage::FloatingImage(const std::string& spritefile) 
  : layer(LAYER_FOREGROUND1 + 1), visible(false), anchor(ANCHOR_MIDDLE)
{
  sprite.reset(sprite_manager->create(spritefile));
}

FloatingImage::~FloatingImage()
{
}

void
FloatingImage::update(float elapsed_time)
{
  (void) elapsed_time;
}

void
FloatingImage::draw(DrawingContext& context)
{
  if(!visible)
    return;

  context.push_transform();
  context.set_translation(Vector(0, 0));

  Vector spos = pos + get_anchor_pos(Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT),
      sprite->get_width(), sprite->get_height(), anchor);

  sprite->draw(context, spos, layer);

  context.pop_transform();
}
