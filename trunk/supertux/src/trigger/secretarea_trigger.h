#ifndef __SECRETAREA_TRIGGER_H__
#define __SECRETAREA_TRIGGER_H__

#include "trigger_base.h"
#include "serializable.h"
#include "resources.h"
#include "video/drawing_context.h"
#include "app/globals.h"

class SecretAreaTrigger : public TriggerBase, public Serializable
{
public:
  SecretAreaTrigger(LispReader& reader);
  SecretAreaTrigger(const Vector& pos, const std::string& sequence);
  ~SecretAreaTrigger();
 
  void write(LispWriter& writer);
  void event(Player& player, EventType type);
  void draw(DrawingContext& context);
  
private:
  EventType triggerevent;
  std::string message;
  int show_message;
};

#endif

