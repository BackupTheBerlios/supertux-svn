#ifndef __SCRIPT_INTERPRETER_H__
#define __SCRIPT_INTERPRETER_H__

#include <squirrel.h>
#include <iostream>
#include "timer.h"
#include "game_object.h"
#include "scripting/sound.h"
#include "scripting/level.h"

class Sector;

class ScriptInterpreter : public GameObject
{
public:
  ScriptInterpreter(Sector* sector);
  ~ScriptInterpreter();

  void draw(DrawingContext& );
  void action(float );

  void load_script(std::istream& in, const std::string& sourcename = "");
  void start_script();
  
  void expose_object(void* object, const std::string& name,
                     const std::string& type);

  void set_wakeup_time(float seconds);

  static ScriptInterpreter* current()
  {
    return _current;
  }

private:
  HSQUIRRELVM v;
  static ScriptInterpreter* _current;
  Timer wakeup_timer;

  Scripting::Sound* sound;
  Scripting::Level* level;
};

#endif

