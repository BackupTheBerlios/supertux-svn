/*
  supertux.c
  
  Super Tux
  
  by Bill Kendrick & Tobias Glaesser <tobi.web@gmx.de>
  bill@newbreedsoftware.com
  http://www.newbreedsoftware.com/supertux/
  
  April 11, 2000 - March 15, 2004
*/

#include "supertux.h"
#include "resources.h"

int main(int argc, char * argv[])
{
  bool done;
  
  st_directory_setup();
  parseargs(argc, argv);
  
  st_audio_setup();
  st_video_setup();
  st_joystick_setup();
  st_general_setup();
  st_menu();
  loadshared();

  if (launch_worldmap_mode)
    {
      worldmap_run();
    }
  else if (level_startup_file)
    {
      GameSession session(level_startup_file, 1, ST_GL_LOAD_LEVEL_FILE);
      session.run();
    }
  else
    {  
      done = false;
      while (!done)
        {
          done = title();
        }
    }
  
  clearscreen(0, 0, 0);
  updatescreen();

  unloadshared();
  st_shutdown();
  
  return 0;
}
