//  $Id$
// 
//  SuperTux
//  Copyright (C) 2004 Tobias Glaesser <tobi.web@gmx.de>
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

#ifndef SUPERTUX_MENU_H
#define SUPERTUX_MENU_H

#include <vector>
#include <set>
#include <string>
#include <utility>

#include "SDL.h"

#include "../video/surface.h"
#include "../video/font.h"
#include "../special/timer.h"
#include "../special/base.h"
#include "../gui/mousecursor.h"

namespace SuperTux
  {

  /* Joystick menu delay */
#define JOYSTICK_MENU_DELAY 500

  /* IDs for menus */

  bool confirm_dialog(Surface* background, std::string text);

  /* Kinds of menu items */
  enum MenuItemKind {
    MN_ACTION,
    MN_GOTO,
    MN_TOGGLE,
    MN_BACK,
    MN_DEACTIVE,
    MN_TEXTFIELD,
    MN_NUMFIELD,
    MN_CONTROLFIELD_KB,
    MN_CONTROLFIELD_JS,
    MN_STRINGSELECT,
    MN_LABEL,
    MN_HL, /* horizontal line */
  };

  class Menu;

  class MenuItem
    {
    public:
      MenuItem() {};
      MenuItem(MenuItemKind kind, int id = -1);
      MenuItem(MenuItemKind kind, int id, const std::string& text);
      MenuItemKind kind;
      int id;   // item id      
      int toggled;
      std::string text;
      std::string input;
      int *int_p;   // used for setting keys (can be used for more stuff...)
      std::pair<std::set<std::string>, std::set<std::string>::iterator> list;
      Menu* target_menu;

      void change_text (const std::string& text);
      void change_input(const std::string& text);

      static MenuItem* create(MenuItemKind kind, const std::string& text, int init_toggle, Menu* target_menu, int id, int* int_p);

      std::string get_input_with_symbol(bool active_item);   // returns the text with an input symbol
    private:
      bool input_flickering;
      Timer input_flickering_timer;
    };

  class Menu
    {
    private:
      static std::vector<Menu*> last_menus;
      static Menu* current_;

      static void push_current(Menu* pmenu);
      static void pop_current();

    public:
      /** Set the current menu, if pmenu is NULL, hide the current menu */
      static void set_current(Menu* pmenu);

      /** Return the current active menu or NULL if none is active */
      static Menu* current()
      {
        return current_;
      }

    private:
      /* Action done on the menu */
      enum MenuAction {
        MENU_ACTION_NONE = -1,
        MENU_ACTION_UP,
        MENU_ACTION_DOWN,
        MENU_ACTION_LEFT,
        MENU_ACTION_RIGHT,
        MENU_ACTION_HIT,
        MENU_ACTION_INPUT,
        MENU_ACTION_REMOVE
      };

      /** Number of the item that got 'hit' (ie. pressed) in the last
          event()/action() call, -1 if none */
      int hit_item;

      // position of the menu (ie. center of the menu, not top/left)
      int pos_x;
      int pos_y;

      /** input event for the menu (up, down, left, right, etc.) */
      MenuAction menuaction;

      /* input implementation variables */
      int delete_character;
      char mn_input_char;
      Timer joystick_timer;

    public:
      static Font* default_font;
      static Font* active_font;
      static Font* deactive_font;
      static Font* label_font;
      static Font* field_font;
    
      Timer effect;
      int arrange_left;
      int active_item;

      std::vector<MenuItem> item;

      Menu();
      ~Menu();

      void additem(const MenuItem& menu_item);      
      void additem(MenuItem* pmenu_item);
      void additem(MenuItemKind kind, const std::string& text, int init_toggle, Menu* target_menu, int id = -1, int *int_p = NULL);

      void  action ();

      /** Remove all entries from the menu */
      void clear();

      /** Return the index of the menu item that was 'hit' (ie. the user
          clicked on it) in the last event() call */
      int  check  ();

      MenuItem& get_item(int index)
      {
        return item[index];
      }
      MenuItem& get_item_by_id(int id);

      int get_active_item_id();

      bool isToggled(int id);

      void Menu::get_controlfield_key_into_input(MenuItem *item);
      void Menu::get_controlfield_js_into_input(MenuItem *item);

      void draw(DrawingContext& context);
      void draw_item(DrawingContext& context,
                     int index, int menu_width, int menu_height);
      void set_pos(int x, int y, float rw = 0, float rh = 0);

      /** translate a SDL_Event into a menu_action */
      void event(SDL_Event& event);

      int get_width() const;
      int get_height() const;

      bool is_toggled(int id) const;
    };

  extern Surface* checkbox;
  extern Surface* checkbox_checked;
  extern Surface* back;
  extern Surface* arrow_left;
  extern Surface* arrow_right;

} //namespace SuperTux

#endif /*SUPERTUX_MENU_H*/

/* Local Variables: */
/* mode: c++ */
/* End: */