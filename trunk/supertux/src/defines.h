/*
  defines.h
  
  Super Tux
  
  by Bill Kendrick
  bill@newbreedsoftware.com
  http://www.newbreedsoftware.com/supertux/
 
  April 11, 2000 - December 28, 2003
*/


#if !defined( SUPERTUX_DEFINES_H )

#define SUPERTUX_DEFINES_H
/* Version: */

#define VERSION "0.0.5"

/* Frames per second: */

#define FPS (1000 / 25)


/* Joystick buttons and axes: */

#define JOY_A 0
#define JOY_B 1

#define JOY_X 0
#define JOY_Y 1


/* Booleans: */

#define NO 0
#define YES 1

/* Direction (keyboard/joystick) states: */

#define UP 0
#define DOWN 1

/* Dying types: */

/* ---- NO 0 */
#define SQUISHED 1
#define FALLING 2

/* Hurt modes: */

#define KILL 0
#define SHRINK 1

/* Directions: */

#define LEFT 0
#define RIGHT 1

/* Sizes: */

#define SMALL 0
#define BIG 1

/* Speed constraints: */

#define MAX_WALK_XM 8
#define MAX_RUN_XM 3
#define MAX_YM 10
#define MAX_JUMP_TIME 1000
#define MAX_LIVES 4

#define WALK_SPEED 1
#define RUN_SPEED 1.5
#define JUMP_SPEED 1.5

#define GRAVITY 1
#define YM_FOR_JUMP 50
#define KILL_BOUNCE_YM 8

#define SKID_XM 4
#define SKID_TIME 8

/* Size constraints: */

#define OFFSCREEN_DISTANCE 256

#define LEVEL_WIDTH 375


/* Debugging */

#ifdef DEBUG
        #define DEBUG_MSG( msg ) { \
       	printf( msg ); printf("\n"); \
        }
        #else
       #define DEBUG_MSG( msg ) {}
#endif

#endif

