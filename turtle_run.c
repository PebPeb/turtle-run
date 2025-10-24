// #include <stdio.h>

// int main() {
//     FILE *file;
//     char filename[] = "./art/shell.txt"; // Change to your file's name
//     int ch;

//     // Open the file in read mode
//     file = fopen(filename, "r");

//     if (file == NULL) {
//         perror("Error opening file");
//         return 1;
//     }

//     // Read and print the contents character by character
//     while ((ch = fgetc(file)) != EOF) {
//         putchar(ch);
//     }

//     // Close the file
//     fclose(file);

//     return 0;
// }

#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>     // usleep
#include <sys/ioctl.h>  // ioctl, winsize

int main() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);   // Get terminal size

    int width = w.ws_col; 
    int height = w.ws_row;

    char temp = '-';



    initscr();

    for (int i = 0; i < 10; i++) {
      for (int x = 0 + i; x < 10 + i; x++)
        for (int y = 0; y < 10; y++)
          mvaddch(y, x, temp);
  
          
      refresh();
      usleep(200000);
      clear();
    }
    endwin();

    // signal(SIGINT, SIG_IGN);
    // noecho();
    // curs_set(0);
    // nodelay(stdscr, TRUE);
    // leaveok(stdscr, TRUE);
    // scrollok(stdscr, FALSE);

    // for (x = COLS - 1; ; --x) {
    //     if (LOGO == 1) {
    //         if (add_sl(x) == ERR) break;
    //     }
    //     else if (C51 == 1) {
    //         if (add_C51(x) == ERR) break;
    //     }
    //     else {
    //         if (add_D51(x) == ERR) break;
    //     }
    //     getch();
    //     refresh();
    //     usleep(40000);
    // }
    // mvcur(0, COLS - 1, LINES - 1, 0);
    // endwin();

    // We'll draw 4 lines spaced evenly vertically
    // int num_lines = 4;
    // int line_rows[4];

    // for (int i = 0; i < num_lines; ++i) {
    //     line_rows[i] = (height / (num_lines + 1)) * (i + 1);
    // }

    // // Hide the cursor
    // printf("\033[?25l");
    // fflush(stdout);

    // for (int col = 0; col < width; ++col) {
    //     // Clear the screen on each frame
    //     printf("\033[2J");

    //     // Draw all 4 lines at the current column
    //     for (int i = 0; i < num_lines; ++i) {
    //         int row = line_rows[i];
    //         printf("\033[%d;%dH", row, col + 1); // Move to (row, col)
    //         putchar('-');
    //     }

    //     fflush(stdout);
    //     usleep(30000); // Delay to slow down animation
    // }

    // // Move cursor to bottom and show it again
    // printf("\033[%d;1H", height);
    // printf("\033[?25h");
    // fflush(stdout);

    return 0;
}
