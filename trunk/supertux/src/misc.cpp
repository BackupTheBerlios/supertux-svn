//  $Id$
//
//  SuperTux
//  Copyright (C) 2004 Tobas Glaesser <tobi.web@gmx.de>
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#include <config.h>

#include "misc.hpp"
#include "main.hpp"
#include "gameconfig.hpp"
#include "game_session.hpp"
#include "gui/menu.hpp"
#include "gui/button.hpp"
#include "audio/sound_manager.hpp"
#include "title.hpp"
#include "resources.hpp"
#include "worldmap.hpp"
#include "gettext.hpp"
#include "options_menu.hpp"
#include "control/joystickkeyboardcontroller.hpp"

Menu* main_menu      = 0;
Menu* game_menu      = 0;

void setup_menu()
{
  main_menu      = new Menu();
  game_menu      = new Menu();
  worldmap_menu  = new Menu();

  main_menu->set_pos(SCREEN_WIDTH/2, 335);
  main_menu->add_entry(MNID_STARTGAME, _("Start Game"));
  main_menu->add_entry(MNID_LEVELS_CONTRIB, _("Contrib Levels"));
  main_menu->add_submenu(_("Options"), get_options_menu());
  main_menu->add_entry(MNID_CREDITS, _("Credits"));
  main_menu->add_entry(MNID_QUITMAINMENU, _("Quit"));
  
  game_menu->add_label(_("Pause"));
  game_menu->add_hl();
  game_menu->add_entry(MNID_CONTINUE, _("Continue"));
  game_menu->add_submenu(_("Options"), get_options_menu());
  game_menu->add_hl();
  game_menu->add_entry(MNID_ABORTLEVEL, _("Abort Level"));

  worldmap_menu->add_label(_("Pause"));
  worldmap_menu->add_hl();
  worldmap_menu->add_entry(WorldMapNS::MNID_RETURNWORLDMAP, _("Continue"));
  worldmap_menu->add_submenu(_("Options"), get_options_menu());
  worldmap_menu->add_hl();
  worldmap_menu->add_entry(WorldMapNS::MNID_QUITWORLDMAP, _("Quit Game"));
}

void free_menu()
{
  delete worldmap_menu;
  delete main_menu;
  delete game_menu;
  free_options_menu();
}

