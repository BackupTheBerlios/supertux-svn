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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.

#include <cstring>
#include <cstdlib>

#include "../app/setup.h"
#include "../video/screen.h"
#include "../video/drawing_context.h"
#include "../app/globals.h"
#include "../gui/button.h"

using namespace SuperTux;

Timer Button::popup_timer;
Font* Button::info_font = 0;

Button::Button(Surface* button_image, const std::string& ninfo,
               SDLKey nshortcut, int x, int y, int mw, int mh)
{
  popup_timer.init(false);

  if(button_image)
    icon.push_back(button_image);

  info = ninfo;

  shortcut = nshortcut;

  rect.x = x;
  rect.y = y;
  rect.w = icon[0]->w;
  rect.h = icon[0]->h;
  tag = -1;
  state = BUTTON_NONE;
  show_info = false;
}

Button::Button(const std::string& imagefilename, const std::string& ninfo,
               SDLKey nshortcut, int x, int y, int mw, int mh)
{
  popup_timer.init(false);

  add_icon(imagefilename, mw, mh);

  info = ninfo;

  shortcut = nshortcut;

  rect.x = x;
  rect.y = y;
  rect.w = icon[0]->w;
  rect.h = icon[0]->h;
  tag = -1;
  state = BUTTON_NONE;
  show_info = false;
}

void Button::add_icon(const std::string& icon_file, int mw, int mh)
{
  char filename[1024];

  if(!icon_file.empty())
    {
      snprintf(filename, 1024, "%s/%s", datadir.c_str(), icon_file.c_str());
      if(!FileSystem::faccessible(filename))
        snprintf(filename, 1024, "%s/images/icons/default-icon.png", datadir.c_str());
    }
  else
    {
      snprintf(filename, 1024, "%s/images/icons/default-icon.png", datadir.c_str());
    }

  if(mw != -1 || mh != -1)
    {
      icon.push_back(new Surface(filename,true));
      icon.back()->resize(mw,mh);
    }
  else
    icon.push_back(new Surface(filename,true));

}

void Button::draw(DrawingContext& context)
{
  if(state == BUTTON_HOVER)
    if(!popup_timer.check())
      show_info = true;

  fillrect(rect.x,rect.y,rect.w,rect.h,75,75,75,200);
  fillrect(rect.x+1,rect.y+1,rect.w-2,rect.h-2,175,175,175,200);

  for(std::vector<Surface*>::iterator it = icon.begin(); it != icon.end(); ++it)
    context.draw_surface(*it, Vector(rect.x,rect.y), LAYER_GUI);

  /*  if(drawable)
    {
      Camera viewport;
      viewport.set_translation(Vector(rect.x, rect.y));
      drawable->draw(viewport, 0);
    }*/

  if(show_info)
    {
      char str[80];
      int i = -32;

      if(0 > rect.x - info_font->get_text_width(info))
        i = rect.w + (int)info_font->get_text_width(info);

      if(!info.empty())
        context.draw_text(info_font, info, Vector(i + rect.x - info_font->get_text_width(info), rect.y), LAYER_GUI);
      sprintf(str,"(%s)", SDL_GetKeyName(shortcut));
      context.draw_text(info_font, str, Vector(i + rect.x -  info_font->get_text_width(str), rect.y + info_font->get_height()+2), LAYER_GUI);
    }
  if(state == BUTTON_PRESSED || state == BUTTON_DEACTIVE)
    fillrect(rect.x,rect.y,rect.w,rect.h,75,75,75,200);
  else if(state == BUTTON_HOVER)
    fillrect(rect.x,rect.y,rect.w,rect.h,150,150,150,128);
}

Button::~Button()
{
  for(std::vector<Surface*>::iterator it = icon.begin(); it != icon.end(); ++it)
    delete (*it);
  icon.clear();
  // FIXME TODO XXX: commenting this out fixes the leveleditor quit crash
  //   probably should be deleted somehow, though
  //delete drawable;
}

void Button::event(SDL_Event &event)
{
  if(state == BUTTON_DEACTIVE)
    return;

  SDLKey key = event.key.keysym.sym;

  if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
    {
      if(event.button.x < rect.x || event.button.x >= rect.x + rect.w ||
          event.button.y < rect.y || event.button.y >= rect.y + rect.h)
        return;

      if(event.button.button == SDL_BUTTON_RIGHT)
        {
          show_info = true;
          return;
        }
      else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == 4) /* Mouse wheel up. */
        {
          state = BUTTON_WHEELUP;
          return;
        }
      else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == 5) /* Mouse wheel down. */
        {
          state = BUTTON_WHEELDOWN;
          return;
        }

      if(event.button.button == SDL_BUTTON_LEFT)
        if(event.type == SDL_MOUSEBUTTONDOWN)
          state = BUTTON_PRESSED;
        else
          state = BUTTON_CLICKED;
    }
  else if(event.type == SDL_MOUSEMOTION)
    {
      if(event.motion.x < rect.x || event.motion.x >= rect.x + rect.w ||
          event.motion.y < rect.y || event.motion.y >= rect.y + rect.h)
        {
          state = BUTTON_NONE;
        }
      else
        {
          state = BUTTON_HOVER;
          popup_timer.start(1500);
        }

      if(show_info)
        {
          show_info = false;
        }
    }
  else if(event.type == SDL_KEYDOWN)
    {
      if(key == shortcut)
        state = BUTTON_PRESSED;
    }
  else if(event.type == SDL_KEYUP)
    {
      if(state == BUTTON_PRESSED && key == shortcut)
        state = BUTTON_CLICKED;
    }
}

int Button::get_state()
{
  int rstate;
  switch(state)
    {
    case BUTTON_CLICKED:
    case BUTTON_WHEELUP:
    case BUTTON_WHEELDOWN:
      rstate = state;
      state = BUTTON_NONE;
      return rstate;
    default:
      return state;
    }
}

ButtonPanel::ButtonPanel(const SDL_Rect& rect)
{
  ButtonPanel(rect.x, rect.y, rect.w, rect.h);
}

ButtonPanel::ButtonPanel(int x, int y, int w, int h)
{
  bw = 32;
  bh = 32;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  hidden = false;
  hlast = false;
}

Button* ButtonPanel::event(SDL_Event& event)
{
  if(!hidden)
    {
      Button* ret = NULL;
      for(std::vector<Button*>::iterator it = item.begin(); it != item.end(); ++it)
        {
          (*it)->event(event);
          if((*it)->state != BUTTON_NONE)
            {
              if(hlast && (*it)->state == BUTTON_CLICKED)
                last_clicked = it;
              ret = (*it);
            }
        }
      return ret;
    }
  else
    {
      return NULL;
    }
}

ButtonPanel::~ButtonPanel()
{
  for(std::vector<Button*>::iterator it = item.begin(); it != item.end(); ++it)
    {
      delete (*it);
    }
  item.clear();
}

void ButtonPanel::draw(DrawingContext& context)
{

  if(hidden == false)
    {
      fillrect(rect.x,rect.y,rect.w,rect.h,100,100,100,200);
      for(std::vector<Button*>::iterator it = item.begin(); it != item.end(); ++it)
        {
          (*it)->draw(context);
          if(hlast && it == last_clicked)
            {
              fillrect((*it)->get_pos().x,(*it)->get_pos().y,(*it)->get_pos().w,(*it)->get_pos().h,100,100,100,128);
            }
        }
    }
}

void ButtonPanel::additem(Button* pbutton, int tag)
{
  int max_cols, row, col;

  item.push_back(pbutton);

  /* A button_panel takes control of the buttons it contains and arranges them */

  max_cols = rect.w / bw;

  row = (item.size()-1) / max_cols;
  col = (item.size()-1) % max_cols;

  item[item.size()-1]->rect.x = rect.x + col * bw;
  item[item.size()-1]->rect.y = rect.y + row * bh;
  item[item.size()-1]->tag = tag;

}

void ButtonPanel::set_button_size(int w, int h)
{
  bw = w;
  bh = h;
}

Button* ButtonPanel::manipulate_button(int i)
{
  if(int(item.size())-1 < i)
    return item[item.size()-1];
  else
    return item[i];
}

void ButtonPanel::highlight_last(bool b)
{
  hlast = b;
}

void ButtonPanel::set_last_clicked(unsigned int last)
{
  if(hlast)
    {
      if(item.size() >= last)
        {
          last_clicked = item.begin() + last;
        }
    }
}