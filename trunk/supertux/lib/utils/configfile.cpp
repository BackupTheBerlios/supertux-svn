//  $Id$
//
//  SuperTux -  A Jump'n Run
//  Copyright (C) 2004 Michael George <mike@georgetech.com>
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

#include <cstdlib>
#include <string>

#include "../utils/configfile.h"
#include "../app/setup.h"
#include "../app/globals.h"
#include "../audio/sound_manager.h"

using namespace SuperTux;

#ifdef WIN32
const char * config_filename = ("/"+ package_symbol_name + "_config.dat").c_str();
#else
const char * config_filename = "/config";
#endif

Config* SuperTux::config = 0;

static void defaults ()
{
  /* Set defaults: */
  debug_mode = false;
  SoundManager::get()->set_audio_device_available(true);

  use_fullscreen = false;
  show_fps = false;
  use_gl = false;

  SoundManager::get()->enable_sound(true);
  SoundManager::get()->enable_music(true);
}

FILE * SuperTux::opendata(const char * rel_filename, const char * mode)
{
  char * filename = NULL;
  FILE * fi;

  filename = (char *) malloc(sizeof(char) * (strlen(st_dir) +
                                             strlen(rel_filename) + 1));

  strcpy(filename, st_dir);
  /* Open the high score file: */

  strcat(filename, rel_filename);

  /* Try opening the file: */
  fi = fopen(filename, mode);

  if (fi == NULL)
    {
      fprintf(stderr, "Warning: Unable to open the file \"%s\" ", filename);

      if (strcmp(mode, "r") == 0)
        fprintf(stderr, "for read!!!\n");
      else if (strcmp(mode, "w") == 0)
        fprintf(stderr, "for write!!!\n");
    }
  free( filename );

  return(fi);
}

void Config::load()
{
  FILE * file = NULL;

  defaults();

  /* override defaults from config file */

  file = opendata(config_filename, "r");

  if (file == NULL)
    return;

  /* read config file */

  lisp_stream_t   stream;
  lisp_object_t * root_obj = NULL;

  lisp_stream_init_file (&stream, file);
  root_obj = lisp_read (&stream);

  if (root_obj->type == LISP_TYPE_EOF || root_obj->type == LISP_TYPE_PARSE_ERROR)
    return;

  if (strcmp(lisp_symbol(lisp_car(root_obj)), (package_symbol_name+"-config").c_str()) != 0)
    return;

  LispReader reader(lisp_cdr(root_obj));

  reader.read_bool("fullscreen", use_fullscreen);
  bool temp;
  reader.read_bool("sound",     temp);
  SoundManager::get()->enable_sound(temp);
  reader.read_bool("music",      temp);
  SoundManager::get()->enable_music(temp);
  reader.read_bool("show_fps",   show_fps);

  std::string video;
  reader.read_string ("video", video);
  if (video == "opengl")
    use_gl = true;
  else
    use_gl = false;

  reader.read_int ("joystick", joystick_num);

  if (joystick_num >= 0)
    {
    reader.read_int ("joystick-x", joystick_keymap.x_axis);
    reader.read_int ("joystick-y", joystick_keymap.y_axis);
    reader.read_int ("joystick-a", joystick_keymap.a_button);
    reader.read_int ("joystick-b", joystick_keymap.b_button);
    reader.read_int ("joystick-start", joystick_keymap.start_button);
    reader.read_int ("joystick-deadzone", joystick_keymap.dead_zone);
    }

  customload(reader);

  lisp_free(root_obj);
  fclose(file);
}

void Config::save ()
{
  /* write settings to config file */
  FILE * config = opendata(config_filename, "w");

  if(config)
    {
      fprintf(config, ("("+package_symbol_name+"-config\n").c_str());
      fprintf(config, "\t;; the following options can be set to #t or #f:\n");
      fprintf(config, "\t(fullscreen %s)\n", use_fullscreen ? "#t" : "#f");
      fprintf(config, "\t(sound      %s)\n", SoundManager::get()->sound_enabled()      ? "#t" : "#f");
      fprintf(config, "\t(music      %s)\n", SoundManager::get()->music_enabled()      ? "#t" : "#f");
      fprintf(config, "\t(show_fps   %s)\n", show_fps       ? "#t" : "#f");

      fprintf(config, "\n\t;; either \"opengl\" or \"sdl\"\n");
      fprintf(config, "\t(video      \"%s\")\n", use_gl ? "opengl" : "sdl");

      if(use_joystick)
        {
        fprintf(config, "\n\t;; joystick number:\n");
        fprintf(config, "\t(joystick   %d)\n", joystick_num);

        fprintf(config, "\t(joystick-x   %d)\n", joystick_keymap.x_axis);
        fprintf(config, "\t(joystick-y   %d)\n", joystick_keymap.y_axis);
        fprintf(config, "\t(joystick-a   %d)\n", joystick_keymap.a_button);
        fprintf(config, "\t(joystick-b   %d)\n", joystick_keymap.b_button);
        fprintf(config, "\t(joystick-start  %d)\n", joystick_keymap.start_button);
        fprintf(config, "\t(joystick-deadzone  %d)\n", joystick_keymap.dead_zone);
        }
	
	customsave(config);

      fprintf(config, ")\n");
      fclose(config);
    }
}

/* EOF */