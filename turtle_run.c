/******************************************************************************
 * File:    turtle_run.c
 * Author:  Bryce Keen
 * Date:    2025-10-27
 * Version: 1.0
 *
 * Description:
 *   A Turtle that walks across the screen
 *
 * Compilation:
 *   gcc turtle_run.c -o turtle_run -lncurses
 *
 * Usage:
 *   ./turtle_run
 *
 * Dependencies:
 *   - ncurses
 *   - turtle_run.h (contains ASCII art frames)
 *
 ******************************************************************************/

#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>     
#include <sys/ioctl.h> 
#include <string.h>
#include "turtle_run.h"

enum turtle_states {
  HIDE_0,
  HIDE_1,
  HIDE_2,
  HIDE_3,
  HIDE_4,
  START_WALKING,
  WALK_0,
  WALK_1,
  WALK_2,
  WALK_3,
  WALK_4
};

#define FRAME_RATE 120000     // u seconds
#define HIDE_RATE  300000
#define INITIAL_X_OFFSET -35

int main(int argc, char *argv[]) {
    
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    int width = w.ws_col; 
    int height = w.ws_row;

    const char* start_walking[] = {SHELL0, SHELL1, SHELL2, SHELL3, SHELL4, START_WALK0, START_WALK1};
    const char* walk0[] = {SHELL0, SHELL1, SHELL2, SHELL3, SHELL4, WALK0_0, WALK0_1};
    const char* walk1[] = {SHELL0, SHELL1, SHELL2, SHELL3, SHELL4, WALK1_0, WALK1_1};
    const char* walk2[] = {SHELL0, SHELL1, SHELL2, SHELL3, SHELL4, WALK2_0, WALK2_1};
    const char* walk3[] = {SHELL0, SHELL1, SHELL2, SHELL3, SHELL4, WALK3_0, WALK3_1};
    const char* walk4[] = {SHELL0, SHELL1, SHELL2, SHELL3, SHELL4, WALK4_0, WALK4_1};
    const char* hide0[] = {EMPTY, SHELL0, SHELL1, HIDE0_2, HIDE0_3, HIDE0_4, HIDE0_5};
    const char* hide1[] = {EMPTY, SHELL0, SHELL1, HIDE1_2, HIDE1_3, HIDE1_4, HIDE1_5};
    const char* hide2[] = {EMPTY, SHELL0, SHELL1, HIDE2_2, HIDE2_3, HIDE2_4, HIDE2_5};
    const char* hide3[] = {SHELL0, SHELL1, HIDE3_2, HIDE3_3, HIDE3_4, HIDE3_5, HIDE3_6};
    const char* hide4[] = {SHELL0, SHELL1, HIDE4_2, HIDE4_3, HIDE4_4, HIDE4_5, HIDE4_6};

    initscr();
    noecho();
    curs_set(0);

    const char** image = NULL; 
    int imageSize;

    int currentState = START_WALKING;
    int nextState = START_WALKING;
  
    int stallState = 0;

    int y = 0;
    int x = INITIAL_X_OFFSET;
    int y_offset = height/4;

    bool refresh = true;
    bool forwardBackward = true; // True for forward
    bool hideTurtle = false;
    bool turtleHid = false;

    int wait = FRAME_RATE;

    while (x < width) {
      y = 0;

      // State Machine
      switch (currentState) {
        case HIDE_0:
          image = hide0;
          imageSize = sizeof(hide0) / sizeof(hide0[0]);
          hideTurtle = false;
          
          if (stallState == 0) {
            nextState = HIDE_0;
            stallState++;
          } else if (stallState < 4) {
            refresh = false;
            nextState = HIDE_0;
            stallState++;
          } else {
            refresh = true;
            nextState = HIDE_1;
            stallState = 0;
          }

          break;
        case HIDE_1:
          image = hide1;
          imageSize = sizeof(hide1) / sizeof(hide1[0]);
          if (hideTurtle) 
            nextState = HIDE_0;
          else
            nextState = HIDE_2;
          break;
        case HIDE_2:
          image = hide2;
          imageSize = sizeof(hide2) / sizeof(hide2[0]);
          if (hideTurtle) 
            nextState = HIDE_1;
          else
            nextState = HIDE_3;
          break;
        case HIDE_3:
          image = hide3;
          imageSize = sizeof(hide3) / sizeof(hide3[0]);          
          if (hideTurtle) 
            nextState = HIDE_2;
          else
            nextState = HIDE_4;
          break;
        case HIDE_4:
          wait = HIDE_RATE;
          image = hide4;
          imageSize = sizeof(hide4) / sizeof(hide4[0]);
          
          if (hideTurtle) 
            nextState = HIDE_3;
          else if (stallState == 0) {
            nextState = HIDE_4;
            stallState++;
          } else if (stallState < 2) {
            refresh = false;
            nextState = HIDE_4;
            stallState++;
          } else {
            refresh = true;
            nextState = START_WALKING;
            stallState = 0;
          }
          break;
        case START_WALKING:
          wait = FRAME_RATE;
          image = start_walking;
          imageSize = sizeof(start_walking) / sizeof(start_walking[0]);

          if (hideTurtle) 
            nextState = HIDE_4;
          else
            nextState = WALK_0;
          x++;
          break;
        case WALK_0:
          wait = FRAME_RATE;
          image = walk0;
          imageSize = sizeof(walk0) / sizeof(walk0[0]);

          nextState = WALK_1;
          forwardBackward = true;

          if (x >= (width/6) && !turtleHid) {
            turtleHid = true;
            hideTurtle = true;
            nextState = START_WALKING;
          }

          x++;
          break;
        case WALK_1:
          image = walk1;
          imageSize = sizeof(walk1) / sizeof(walk1[0]);
          if (forwardBackward)
            nextState = WALK_2;
          else 
            nextState = WALK_0;
          x++;
          break;
        case WALK_2:
          image = walk2;
          imageSize = sizeof(walk2) / sizeof(walk2[0]);
          if (forwardBackward)
            nextState = WALK_3;
          else
            nextState = WALK_1;
          x++;
          break;
        case WALK_3:
          image = walk3;
          imageSize = sizeof(walk3) / sizeof(walk3[0]);
          if (forwardBackward)
            nextState = WALK_4;
          else
            nextState = WALK_2;
          x++;
          break;
        case WALK_4:
          image = walk4;
          imageSize = sizeof(walk4) / sizeof(walk4[0]);
          nextState = WALK_3;
          forwardBackward = false;
          x++;
          break;
      }
      
      currentState = nextState;
      if (refresh) {
        // Display
        for (int i = 0; i < imageSize; i++) {
          const char *line = image[i];
          int len = strlen(line);
          printf("%s\n", line);
          for (int x_offset = 0; x_offset < len; x_offset++) {
            char temp = line[x_offset];
            mvaddch(y + y_offset, x + x_offset, temp);
          }
          y++;
        }
        refresh();
      }
      usleep(wait);
      if (refresh)
        clear();
    }

    endwin();
    return 0;
}
