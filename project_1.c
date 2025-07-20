#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#define MAX_BULLETS 100
#define MAX_DOGS 100

int health = 5;

void drawing(int y, int x);
void control(int *x, int *y, int key, int *dx, int *dy);
void spawn_dogs();
void move_dogs(int player_x, int player_y);
void check_collision(int player_x, int player_y);
void check_bullet_collision();

typedef struct {
  int x, y;
  int dx, dy;
  int status;
} Bullet; 

Bullet bullets[MAX_BULLETS];
int bullet_count = 0;


typedef struct {
  int x, y;
  int dx, dy;
  int status;
} Dog;

Dog dogs[MAX_DOGS];
int dog_count = 0;

int main() {
    initscr();
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    nodelay(stdscr, TRUE);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    srand(time(NULL));
    int dog_spawn_timer = 0;
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

        dog_spawn_timer++;
        if(dog_spawn_timer >= 50) {
          spawn_dogs();
          dog_spawn_timer = 0;
        }
        
        move_dogs(x, y);
        check_collision(x, y);
        check_bullet_collision();

        drawing(y, x);
        napms(100);
    }

    endwin();
    return 0;
}


void drawing(int y, int x) {
    clear();
    attron(COLOR_PAIR(2));
    mvprintw(y, x, "o");
    attroff(COLOR_PAIR(2));

    for (int i = 0; i < bullet_count; i++) {
        if (bullets[i].status) {
            attron(COLOR_PAIR(1));
            mvprintw(bullets[i].y, bullets[i].x, "*");
            attroff(COLOR_PAIR(1));
        }
    }
    
    for (int i = 0; i < dog_count; i++) {
      if (dogs[i].status) {
          attron(COLOR_PAIR(3));
          mvprintw(dogs[i].y, dogs[i].x, "SUKA");
          attroff(COLOR_PAIR(3));
      }
    }
    mvprintw(0, 0, "Health: %d", health);
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

void spawn_dogs() {
  if(dog_count < MAX_DOGS) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    dogs[dog_count].x = rand() % max_x;
    dogs[dog_count].y = rand() % max_y;
    dogs[dog_count].status = 1;
    dog_count++;
  }
}


void move_dogs(int player_x, int player_y) {
    for (int i = 0; i < dog_count; i++) {
        if (dogs[i].status) {
            if (rand() % 100 < 20) {
                int move = rand() % 4;
                switch(move) {
                    case 0: dogs[i].x++; break;
                    case 1: dogs[i].x--; break;
                    case 2: dogs[i].y++; break;
                    case 3: dogs[i].y--; break;
                }
            } else {
                if (dogs[i].x < player_x) dogs[i].x++;
                else if (dogs[i].x > player_x) dogs[i].x--;

                if (dogs[i].y < player_y) dogs[i].y++;
                else if (dogs[i].y > player_y) dogs[i].y--;
            }
        }
    }
}


void check_collision(int player_x, int player_y) {
  for(int i = 0; i < dog_count; i++) {
       if(dogs[i].status && dogs[i].x == player_x && dogs[i].y == player_y) {
          health--;
          dogs[i].status = 0;
       }
  }
}

void check_bullet_collision() {
  for(int i = 0; i < bullet_count; i++) {
    for(int j = 0; j < dog_count; j++) {
      if(dogs[j].status && bullets[i].x == dogs[j].x && bullets[i].y == dogs[j].y) {
          bullets[i].status = 0;
          dogs[j].status = 0;
          break;
      }     
    }
  }
}