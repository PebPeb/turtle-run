
#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>     
#include <sys/ioctl.h> 
#include <string.h>

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

    initscr();
    noecho();
    curs_set(0);

    FILE *file;
    char line[256];

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
          file = fopen("./art/hide_0.txt", "r");
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
          file = fopen("./art/hide_1.txt", "r");
          if (hideTurtle) 
            nextState = HIDE_0;
          else
            nextState = HIDE_2;
          break;
        case HIDE_2:
          file = fopen("./art/hide_2.txt", "r");
          if (hideTurtle) 
            nextState = HIDE_1;
          else
            nextState = HIDE_3;
          break;
        case HIDE_3:
          file = fopen("./art/hide_3.txt", "r");
          if (hideTurtle) 
            nextState = HIDE_2;
          else
            nextState = HIDE_4;
          break;
        case HIDE_4:
          wait = HIDE_RATE;
          file = fopen("./art/hide_4.txt", "r");
          
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
          file = fopen("./art/start_walk.txt", "r");
          if (hideTurtle) 
            nextState = HIDE_4;
          else
            nextState = WALK_0;
          x++;
          break;
        case WALK_0:
          wait = FRAME_RATE;
          file = fopen("./art/walk_0.txt", "r");
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
          file = fopen("./art/walk_1.txt", "r");
          if (forwardBackward)
            nextState = WALK_2;
          else 
            nextState = WALK_0;
          x++;
          break;
        case WALK_2:
          file = fopen("./art/walk_2.txt", "r");
          if (forwardBackward)
            nextState = WALK_3;
          else
            nextState = WALK_1;
          x++;
          break;
        case WALK_3:
          file = fopen("./art/walk_3.txt", "r");
          if (forwardBackward)
            nextState = WALK_4;
          else
            nextState = WALK_2;
          x++;
          break;
        case WALK_4:
          file = fopen("./art/walk_4.txt", "r");
          nextState = WALK_3;
          forwardBackward = false;
          x++;
          break;
      }

      if (!file) {
        perror("Error opening file");
        return 0;
      }

      // Display
      while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        
        for (int x_offset = 0; x_offset < len; x_offset++) {
          char temp = line[x_offset];
          mvaddch(y + y_offset, x + x_offset, temp);
        }
        y++;
      }

      fclose(file);
      currentState = nextState;
      if (refresh)
        refresh();
      usleep(wait);
      if (refresh)
        clear();
    }

    endwin();
    return 0;
}
