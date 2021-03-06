/**
 * WARNING: This file is automatically generated from:
 *  'src/scripting/wrapper.interface.hpp'
 * DO NOT CHANGE
 */
#ifndef __supertux_WRAPPER_H__
#define __supertux_WRAPPER_H__

#include <squirrel.h>
#include "wrapper.interface.hpp"

namespace Scripting
{

void register_supertux_wrapper(HSQUIRRELVM v);

void create_squirrel_instance(HSQUIRRELVM v, Scripting::DisplayEffect* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::Camera* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::Level* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::ScriptedObject* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::Text* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::Player* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::FloatingImage* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::Platform* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::Candle* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::Wind* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::AmbientSound* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::Thunderstorm* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::TileMap* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::SSector* object, bool setup_releasehook = false);
void create_squirrel_instance(HSQUIRRELVM v, Scripting::LevelTime* object, bool setup_releasehook = false);

}

#endif

