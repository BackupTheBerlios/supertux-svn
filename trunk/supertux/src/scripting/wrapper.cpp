/**
 * WARNING: This file is automatically generated from:
 *  'src/scripting/wrapper.interface.hpp'
 * DO NOT CHANGE
 */
#include <config.h>

#include <new>
#include <assert.h>
#include <string>
#include <squirrel.h>
#include "wrapper_util.hpp"
#include "wrapper.interface.hpp"

using namespace Scripting;

static int DisplayEffect_fade_out_wrapper(HSQUIRRELVM v)
{
  Scripting::DisplayEffect* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  float arg0;
  sq_getfloat(v, 2, &arg0);
  
  _this->fade_out(arg0);
  
  return 0;
}

static int DisplayEffect_fade_in_wrapper(HSQUIRRELVM v)
{
  Scripting::DisplayEffect* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  float arg0;
  sq_getfloat(v, 2, &arg0);
  
  _this->fade_in(arg0);
  
  return 0;
}

static int DisplayEffect_set_black_wrapper(HSQUIRRELVM v)
{
  Scripting::DisplayEffect* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  SQBool arg0;
  sq_getbool(v, 2, &arg0);
  
  _this->set_black(arg0);
  
  return 0;
}

static int DisplayEffect_is_black_wrapper(HSQUIRRELVM v)
{
  Scripting::DisplayEffect* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  
  bool return_value = _this->is_black();
  
  sq_pushbool(v, return_value);
  return 1;
}

static int Camera_shake_wrapper(HSQUIRRELVM v)
{
  Scripting::Camera* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  float arg0;
  sq_getfloat(v, 2, &arg0);
  float arg1;
  sq_getfloat(v, 3, &arg1);
  float arg2;
  sq_getfloat(v, 4, &arg2);
  
  _this->shake(arg0, arg1, arg2);
  
  return 0;
}

static int Camera_set_pos_wrapper(HSQUIRRELVM v)
{
  Scripting::Camera* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  float arg0;
  sq_getfloat(v, 2, &arg0);
  float arg1;
  sq_getfloat(v, 3, &arg1);
  
  _this->set_pos(arg0, arg1);
  
  return 0;
}

static int Camera_set_mode_wrapper(HSQUIRRELVM v)
{
  Scripting::Camera* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  const char* arg0;
  sq_getstring(v, 2, &arg0);
  
  _this->set_mode(arg0);
  
  return 0;
}

static int Level_finish_wrapper(HSQUIRRELVM v)
{
  Scripting::Level* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  
  _this->finish();
  
  return 0;
}

static int Level_spawn_wrapper(HSQUIRRELVM v)
{
  Scripting::Level* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  const char* arg0;
  sq_getstring(v, 2, &arg0);
  const char* arg1;
  sq_getstring(v, 3, &arg1);
  
  _this->spawn(arg0, arg1);
  
  return 0;
}

static int Level_flip_vertically_wrapper(HSQUIRRELVM v)
{
  Scripting::Level* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  
  _this->flip_vertically();
  
  return 0;
}

static int ScriptedObject_set_animation_wrapper(HSQUIRRELVM v)
{
  Scripting::ScriptedObject* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  const char* arg0;
  sq_getstring(v, 2, &arg0);
  
  _this->set_animation(arg0);
  
  return 0;
}

static int ScriptedObject_get_animation_wrapper(HSQUIRRELVM v)
{
  Scripting::ScriptedObject* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  
  std::string return_value = _this->get_animation();
  
  sq_pushstring(v, return_value.c_str(), return_value.size());
  return 1;
}

static int ScriptedObject_move_wrapper(HSQUIRRELVM v)
{
  Scripting::ScriptedObject* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  float arg0;
  sq_getfloat(v, 2, &arg0);
  float arg1;
  sq_getfloat(v, 3, &arg1);
  
  _this->move(arg0, arg1);
  
  return 0;
}

static int ScriptedObject_set_pos_wrapper(HSQUIRRELVM v)
{
  Scripting::ScriptedObject* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  float arg0;
  sq_getfloat(v, 2, &arg0);
  float arg1;
  sq_getfloat(v, 3, &arg1);
  
  _this->set_pos(arg0, arg1);
  
  return 0;
}

static int ScriptedObject_get_pos_x_wrapper(HSQUIRRELVM v)
{
  Scripting::ScriptedObject* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  
  float return_value = _this->get_pos_x();
  
  sq_pushfloat(v, return_value);
  return 1;
}

static int ScriptedObject_get_pos_y_wrapper(HSQUIRRELVM v)
{
  Scripting::ScriptedObject* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  
  float return_value = _this->get_pos_y();
  
  sq_pushfloat(v, return_value);
  return 1;
}

static int ScriptedObject_set_velocity_wrapper(HSQUIRRELVM v)
{
  Scripting::ScriptedObject* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  float arg0;
  sq_getfloat(v, 2, &arg0);
  float arg1;
  sq_getfloat(v, 3, &arg1);
  
  _this->set_velocity(arg0, arg1);
  
  return 0;
}

static int ScriptedObject_get_velocity_x_wrapper(HSQUIRRELVM v)
{
  Scripting::ScriptedObject* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  
  float return_value = _this->get_velocity_x();
  
  sq_pushfloat(v, return_value);
  return 1;
}

static int ScriptedObject_get_velocity_y_wrapper(HSQUIRRELVM v)
{
  Scripting::ScriptedObject* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  
  float return_value = _this->get_velocity_y();
  
  sq_pushfloat(v, return_value);
  return 1;
}

static int ScriptedObject_set_visible_wrapper(HSQUIRRELVM v)
{
  Scripting::ScriptedObject* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  SQBool arg0;
  sq_getbool(v, 2, &arg0);
  
  _this->set_visible(arg0);
  
  return 0;
}

static int ScriptedObject_is_visible_wrapper(HSQUIRRELVM v)
{
  Scripting::ScriptedObject* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  
  bool return_value = _this->is_visible();
  
  sq_pushbool(v, return_value);
  return 1;
}

static int ScriptedObject_get_name_wrapper(HSQUIRRELVM v)
{
  Scripting::ScriptedObject* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  
  std::string return_value = _this->get_name();
  
  sq_pushstring(v, return_value.c_str(), return_value.size());
  return 1;
}

static int Sound_play_music_wrapper(HSQUIRRELVM v)
{
  Scripting::Sound* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  const char* arg0;
  sq_getstring(v, 2, &arg0);
  
  _this->play_music(arg0);
  
  return 0;
}

static int Sound_play_wrapper(HSQUIRRELVM v)
{
  Scripting::Sound* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  const char* arg0;
  sq_getstring(v, 2, &arg0);
  
  _this->play(arg0);
  
  return 0;
}

static int Text_set_text_wrapper(HSQUIRRELVM v)
{
  Scripting::Text* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  const char* arg0;
  sq_getstring(v, 2, &arg0);
  
  _this->set_text(arg0);
  
  return 0;
}

static int Text_set_font_wrapper(HSQUIRRELVM v)
{
  Scripting::Text* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  const char* arg0;
  sq_getstring(v, 2, &arg0);
  
  _this->set_font(arg0);
  
  return 0;
}

static int Text_fade_in_wrapper(HSQUIRRELVM v)
{
  Scripting::Text* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  float arg0;
  sq_getfloat(v, 2, &arg0);
  
  _this->fade_in(arg0);
  
  return 0;
}

static int Text_fade_out_wrapper(HSQUIRRELVM v)
{
  Scripting::Text* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  float arg0;
  sq_getfloat(v, 2, &arg0);
  
  _this->fade_out(arg0);
  
  return 0;
}

static int Text_set_visible_wrapper(HSQUIRRELVM v)
{
  Scripting::Text* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  SQBool arg0;
  sq_getbool(v, 2, &arg0);
  
  _this->set_visible(arg0);
  
  return 0;
}

static int Player_set_bonus_wrapper(HSQUIRRELVM v)
{
  Scripting::Player* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  const char* arg0;
  sq_getstring(v, 2, &arg0);
  
  _this->set_bonus(arg0);
  
  return 0;
}

static int Player_make_invincible_wrapper(HSQUIRRELVM v)
{
  Scripting::Player* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  
  _this->make_invincible();
  
  return 0;
}

static int Player_add_life_wrapper(HSQUIRRELVM v)
{
  Scripting::Player* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  
  _this->add_life();
  
  return 0;
}

static int Player_add_coins_wrapper(HSQUIRRELVM v)
{
  Scripting::Player* _this;
  sq_getinstanceup(v, 1, (SQUserPointer*) &_this, 0);
  int arg0;
  sq_getinteger(v, 2, &arg0);
  
  _this->add_coins(arg0);
  
  return 0;
}

static int display_text_file_wrapper(HSQUIRRELVM v)
{
  const char* arg0;
  sq_getstring(v, 2, &arg0);
  
  Scripting::display_text_file(arg0);
  
  return 0;
}

static int set_wakeup_time_wrapper(HSQUIRRELVM v)
{
  float arg0;
  sq_getfloat(v, 2, &arg0);
  
  Scripting::set_wakeup_time(arg0);
  
  return 0;
}

static int translate_wrapper(HSQUIRRELVM v)
{
  const char* arg0;
  sq_getstring(v, 2, &arg0);
  
  std::string return_value = Scripting::translate(arg0);
  
  sq_pushstring(v, return_value.c_str(), return_value.size());
  return 1;
}

static int import_wrapper(HSQUIRRELVM v)
{
  HSQUIRRELVM arg0 = v;
  const char* arg1;
  sq_getstring(v, 2, &arg1);
  
  Scripting::import(arg0, arg1);
  
  return 0;
}

WrappedFunction supertux_global_functions[] = {
  { "display_text_file", &display_text_file_wrapper },
  { "set_wakeup_time", &set_wakeup_time_wrapper },
  { "translate", &translate_wrapper },
  { "import", &import_wrapper },
  { 0, 0 }
};

WrappedConstant<int> supertux_int_constants[] = {
  { 0, 0}
};

WrappedConstant<float> supertux_float_constants[] = {
  { 0, 0}
};

WrappedConstant<const char*> supertux_string_constants[] = {
  { 0, 0}
};

static WrappedFunction supertux_DisplayEffect_methods[] = {
  { "fade_out", &DisplayEffect_fade_out_wrapper },
  { "fade_in", &DisplayEffect_fade_in_wrapper },
  { "set_black", &DisplayEffect_set_black_wrapper },
  { "is_black", &DisplayEffect_is_black_wrapper },
};

static WrappedConstant<int> supertux_DisplayEffect_int_consts[] = {
  { 0, 0}
};

WrappedConstant<float> supertux_DisplayEffect_float_consts[] = {
  { 0, 0}
};

WrappedConstant<const char*> supertux_DisplayEffect_string_consts[] = {
  { 0, 0}
};

static WrappedFunction supertux_Camera_methods[] = {
  { "shake", &Camera_shake_wrapper },
  { "set_pos", &Camera_set_pos_wrapper },
  { "set_mode", &Camera_set_mode_wrapper },
};

static WrappedConstant<int> supertux_Camera_int_consts[] = {
  { 0, 0}
};

WrappedConstant<float> supertux_Camera_float_consts[] = {
  { 0, 0}
};

WrappedConstant<const char*> supertux_Camera_string_consts[] = {
  { 0, 0}
};

static WrappedFunction supertux_Level_methods[] = {
  { "finish", &Level_finish_wrapper },
  { "spawn", &Level_spawn_wrapper },
  { "flip_vertically", &Level_flip_vertically_wrapper },
};

static WrappedConstant<int> supertux_Level_int_consts[] = {
  { 0, 0}
};

WrappedConstant<float> supertux_Level_float_consts[] = {
  { 0, 0}
};

WrappedConstant<const char*> supertux_Level_string_consts[] = {
  { 0, 0}
};

static WrappedFunction supertux_ScriptedObject_methods[] = {
  { "set_animation", &ScriptedObject_set_animation_wrapper },
  { "get_animation", &ScriptedObject_get_animation_wrapper },
  { "move", &ScriptedObject_move_wrapper },
  { "set_pos", &ScriptedObject_set_pos_wrapper },
  { "get_pos_x", &ScriptedObject_get_pos_x_wrapper },
  { "get_pos_y", &ScriptedObject_get_pos_y_wrapper },
  { "set_velocity", &ScriptedObject_set_velocity_wrapper },
  { "get_velocity_x", &ScriptedObject_get_velocity_x_wrapper },
  { "get_velocity_y", &ScriptedObject_get_velocity_y_wrapper },
  { "set_visible", &ScriptedObject_set_visible_wrapper },
  { "is_visible", &ScriptedObject_is_visible_wrapper },
  { "get_name", &ScriptedObject_get_name_wrapper },
};

static WrappedConstant<int> supertux_ScriptedObject_int_consts[] = {
  { 0, 0}
};

WrappedConstant<float> supertux_ScriptedObject_float_consts[] = {
  { 0, 0}
};

WrappedConstant<const char*> supertux_ScriptedObject_string_consts[] = {
  { 0, 0}
};

static WrappedFunction supertux_Sound_methods[] = {
  { "play_music", &Sound_play_music_wrapper },
  { "play", &Sound_play_wrapper },
};

static WrappedConstant<int> supertux_Sound_int_consts[] = {
  { 0, 0}
};

WrappedConstant<float> supertux_Sound_float_consts[] = {
  { 0, 0}
};

WrappedConstant<const char*> supertux_Sound_string_consts[] = {
  { 0, 0}
};

static WrappedFunction supertux_Text_methods[] = {
  { "set_text", &Text_set_text_wrapper },
  { "set_font", &Text_set_font_wrapper },
  { "fade_in", &Text_fade_in_wrapper },
  { "fade_out", &Text_fade_out_wrapper },
  { "set_visible", &Text_set_visible_wrapper },
};

static WrappedConstant<int> supertux_Text_int_consts[] = {
  { 0, 0}
};

WrappedConstant<float> supertux_Text_float_consts[] = {
  { 0, 0}
};

WrappedConstant<const char*> supertux_Text_string_consts[] = {
  { 0, 0}
};

static WrappedFunction supertux_Player_methods[] = {
  { "set_bonus", &Player_set_bonus_wrapper },
  { "make_invincible", &Player_make_invincible_wrapper },
  { "add_life", &Player_add_life_wrapper },
  { "add_coins", &Player_add_coins_wrapper },
};

static WrappedConstant<int> supertux_Player_int_consts[] = {
  { 0, 0}
};

WrappedConstant<float> supertux_Player_float_consts[] = {
  { 0, 0}
};

WrappedConstant<const char*> supertux_Player_string_consts[] = {
  { 0, 0}
};

WrappedClass supertux_classes[] = {
  { "DisplayEffect", supertux_DisplayEffect_methods, supertux_DisplayEffect_int_consts, supertux_DisplayEffect_float_consts, supertux_DisplayEffect_string_consts },
  { "Camera", supertux_Camera_methods, supertux_Camera_int_consts, supertux_Camera_float_consts, supertux_Camera_string_consts },
  { "Level", supertux_Level_methods, supertux_Level_int_consts, supertux_Level_float_consts, supertux_Level_string_consts },
  { "ScriptedObject", supertux_ScriptedObject_methods, supertux_ScriptedObject_int_consts, supertux_ScriptedObject_float_consts, supertux_ScriptedObject_string_consts },
  { "Sound", supertux_Sound_methods, supertux_Sound_int_consts, supertux_Sound_float_consts, supertux_Sound_string_consts },
  { "Text", supertux_Text_methods, supertux_Text_int_consts, supertux_Text_float_consts, supertux_Text_string_consts },
  { "Player", supertux_Player_methods, supertux_Player_int_consts, supertux_Player_float_consts, supertux_Player_string_consts },
  { 0, 0, 0, 0, 0 }
};

