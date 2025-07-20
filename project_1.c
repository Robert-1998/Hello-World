#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#define MAX_BULLETS 100

void drawing(int y, int x);
void control(int *x, int *y, int key, int *dx, int *dy);

typedef struct {
  int x, y;
  int dx, dy;
  int status;
} Bullet; 

Bullet bullets[MAX_BULLETS];
int bullet_count = 0;

int main() {
    initscr();
    nodelay(stdscr, TRUE);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    int x = 0, y = 0;
    int dx = 1, dy = 0;
    int key;

    while ((key = getch()) != 'q') {
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);

        control(&x, &y, key, &dx, &dy);

        for (int i = 0; i < bullet_count; i++) {
            if (bullets[i].status) {
                bullets[i].x += bullets[i].dx;
                bullets[i].y += bullets[i].dy;

                if (bullets[i].x < 0 || bullets[i].x >= max_x ||
                    bullets[i].y < 0 || bullets[i].y >= max_y) {
                    bullets[i].status = 0;
                }
            }
        }

        drawing(y, x);
        napms(100);
    }

    endwin();
    return 0;
}


void drawing(int y, int x) {
    clear();
    mvprintw(y, x, "o");

    for (int i = 0; i < bullet_count; i++) {
        if (bullets[i].status) {
            mvprintw(bullets[i].y, bullets[i].x, "*");
        }
    }

    refresh();
}


void control(int *x, int *y, int key, int *dx, int *dy) {
  switch(key) {

    case KEY_UP:
      (*y)--;
      *dx = 0, *dy = -1;
      break;

    case KEY_DOWN:
      (*y)++;
      *dx = 0, *dy = +1;
      break;

    case KEY_LEFT:
      (*x)--;
      *dx = -1, *dy = 0;
      break;

    case KEY_RIGHT:
      (*x)++;
      *dx = +1, *dy = 0;
      break;

    case 'x':
      if(bullet_count <  MAX_BULLETS) {
        bullets[bullet_count].x = *x;
        bullets[bullet_count].y = *y;
        bullets[bullet_count].dx = *dx;
        bullets[bullet_count].dy = *dy;
        bullets[bullet_count].status = 1;
        bullet_count++;
      }
    break;

  }


  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  if (*x < 0) *x = 0;
  if (*x >= max_x) *x = max_x - 1;
  if (*y < 0) *y = 0;
  if (*y >= max_y) *y = max_y - 1;

}