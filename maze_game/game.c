/**
 * @Author: S. Sharma <silentcat>
 * @Date:   2020-01-02T22:10:26-06:00
 * @Email:  silentcat@protonmail.com
 * @Last modified by:   silentcat
 * @Last modified time: 2020-01-03T22:50:58-06:00
 */

/**
 * Task: Create a small maze game where the player goes through multiple
 * levels attempting to cycle through mazes, with each game getting more
 * difficult progressively.
 * Solution: Use a set of maze algorithms to generate the maze. Display this
 * on the screen, and have the user go through the set of mazes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int main(int argc, char *argv[])
{
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  int startx = 0, starty = 0;
  getmaxyx(stdscr, starty, startx);
  starty /= 2;
  startx /= 2;
  mvprintw(starty, startx, ">");

  int ch = 0;
  char *str = ">";
  while (1) {
    ch = getch();
    if (ch == 'w') {
      str = "^";
      starty -= 1;
    } else if (ch == 'a') {
      str = "<";
      startx -= 1;
    } else if (ch == 's') {
      str = "v";
      starty += 1;
    }
    else if (ch == 'd') {
      str = ">";
      startx += 1;
    }
    clear();
    mvprintw(starty, startx, str);
    refresh();
  }
  endwin();
  return 0;
}
