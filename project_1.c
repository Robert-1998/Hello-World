#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

void drawing(int y, int x);
void control(int *x, int *y, int key);

int main() {

  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);

  int x = 0, y = 0;
  int key;

  while((key = getch()) != 'q') {
    control(&x, &y, key);
    drawing(y, x);
    napms(100);
  }

  endwin();
  return 0;
}


void drawing(int y, int x) {
    clear();
    mvprintw(y, x, "o");
    refresh();
}


void control(int *x, int *y, int key) {
  switch(key) {

    case KEY_UP:
      (*y)--;
      break;

    case KEY_DOWN:
      (*y)++;
      break;

    case KEY_LEFT:
      (*x)--;
      break;

    case KEY_RIGHT:
      (*x)++;
      break;

  }


  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  if (*x < 0) *x = 0;
  if (*x >= max_x) *x = max_x - 1;
  if (*y < 0) *y = 0;
  if (*y >= max_y) *y = max_y - 1;

}