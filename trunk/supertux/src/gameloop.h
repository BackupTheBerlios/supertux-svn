/*
  gameloop.h
  
  Super Tux - Game Loop!
  
  by Bill Kendrick
  bill@newbreedsoftware.com
  http://www.newbreedsoftware.com/supertux/
  
  April 11, 2000 - December 28, 2003
*/

#if !defined( SUPERTUX_GAMELOOP_H )
#define SUPERTUX_GAMELOOP_H 1

        #include "sound.h"

        /* Direction (keyboard/joystick) states: */

        #define UP 0
        #define DOWN 1


        /* Directions: */

        #define LEFT 0
        #define RIGHT 1


        /* Sizes: */

        #define SMALL 0
        #define BIG 1


        /* Bounciness of distros: */

        #define NO_BOUNCE 0
        #define BOUNCE 1


        /* One-ups... */

        #define DISTROS_LIFEUP 100


        /* When to alert player they're low on time! */

        #define TIME_WARNING 50


        /* Dying types: */

        /* ---- NO 0 */
        #define SQUISHED 1
        #define FALLING 2


        /* Enemy modes: */

        #define NORMAL 0
        #define FLAT 1
        #define KICK 2


        /* Hurt modes: */

        #define KILL 0
        #define SHRINK 1


        /* Upgrade types: */

        enum {
          UPGRADE_MINTS,
          UPGRADE_COFFEE,
          UPGRADE_HERRING
        };


        /* Bad guy kinds: */

        enum {
          BAD_BSOD,
          BAD_LAPTOP,
          BAD_MONEY
        };


        /* Speed constraints: */

        #define MAX_WALK_XM 16
        #define MAX_RUN_XM 24
        #define MAX_YM 24
        #define MAX_JUMP_COUNT 3
        #define MAX_LIVES 4

        #define WALK_SPEED 2
        #define RUN_SPEED 4
        #define JUMP_SPEED 8
        #define BULLET_STARTING_YM 8
        #define BULLET_XM 16

        #define GRAVITY 2
        #define YM_FOR_JUMP 40
        #define KILL_BOUNCE_YM 8

        #define SKID_XM 8
        #define SKID_TIME 8


        #define BOUNCY_BRICK_MAX_OFFSET 8
        #define BOUNCY_BRICK_SPEED 4


        /* Times: */

        #define TUX_SAFE_TIME 16
        #define TUX_INVINCIBLE_TIME 200

        /* Size constraints: */

        #define OFFSCREEN_DISTANCE 256

        #define LEVEL_WIDTH 375


        /* Array sizes: */

        #define NUM_BOUNCY_DISTROS 8
        #define NUM_BROKEN_BRICKS 32
        #define NUM_BOUNCY_BRICKS 4
        #define NUM_BAD_GUYS 128
        #define NUM_FLOATING_SCORES 6
        #define NUM_UPGRADES 2
        #define NUM_BULLETS 3


        /* Scores: */

        #define SCORE_BRICK 5
        #define SCORE_DISTRO 25

        /* Function prototypes: */

        int gameloop(void);

#endif

