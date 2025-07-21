#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#define MAX_BULLETS 100
#define MAX_DOGS 100
#define MAX_COUNT_HEALTH 5
int health_ind = 5;

void drawing(int y, int x);
void control(int *x, int *y, int key, int *dx, int *dy);
void spawn_dogs();
void spawn_car();
void spawn_health();
void move_dogs(int player_x, int player_y);
void check_collision(int player_x, int player_y);
void check_health_collision(int player_x, int player_y);
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

typedef struct {
  int x, y;
  int dx, dy;
  int status;
} Car;

typedef struct {
  int x, y;
  int status;
} Health;


Health health;
int health_count = 0;

int in_car = 0;
int car_speed = 2;  

Car car;
Dog dogs[MAX_DOGS];
int dog_count = 0;

int main() {
    initscr();
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
    nodelay(stdscr, TRUE);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    srand(time(NULL));
    int car_spawn_timer = 0;
    int dog_spawn_timer = 0;
    int health_spawn_timer = 0;
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
        
        car_spawn_timer++;
        if (car_spawn_timer >= 100 && !car.status) {
            spawn_car();
            car_spawn_timer = 0;
        }

        dog_spawn_timer++;
        if(dog_spawn_timer >= 50) {
          spawn_dogs();
          dog_spawn_timer = 0;
        }

        health_spawn_timer++;
        if (health_spawn_timer >= 70) {
            spawn_health();
            health_spawn_timer = 0;
        }
        
        move_dogs(x, y);
        check_collision(x, y);
        check_health_collision(x, y);
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

  if (car.status) {
    attron(COLOR_PAIR(4));
    mvprintw(car.y, car.x, "[[[]]]");
    attroff(COLOR_PAIR(4));
  }

  if (health.status) {
    attron(COLOR_PAIR(5));
    mvprintw(health.y, health.x, "+");
    attroff(COLOR_PAIR(5));
  }
    mvprintw(0, 0, "Health: %d", health_ind);
    refresh();
}


void control(int *x, int *y, int key, int *dx, int *dy) {
  int speed = in_car ? car_speed : 1;
  switch(key) {

    case KEY_UP:
      (*y) -= speed;
      *dx = 0, *dy = -1;
      break;

    case KEY_DOWN:
      (*y) += speed;
      *dx = 0, *dy = +1;
      break;

    case KEY_LEFT:
      (*x) -= speed;
      *dx = -1, *dy = 0;
      break;

    case KEY_RIGHT:
      (*x) += speed;
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

    case 'z':
    if (car.status &&
        abs(*x - car.x) <= 1 &&
        abs(*y - car.y) <= 1) {
        in_car = 1;
        car.status = 1; 
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

void spawn_car() {
    if (!car.status) {
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);

        car.x = rand() % max_x;
        car.y = rand() % max_y;
        car.status = 1;
    }
}

void spawn_health() {
    if (!health.status && health_count < MAX_COUNT_HEALTH) {
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);

        health.x = rand() % max_x;
        health.y = rand() % max_y;
        health.status = 1;
        health_count++;
    }
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
      health_ind--;
      dogs[i].status = 0;
    }
  }
}

void check_health_collision(int player_x, int player_y) {
    if (health.status && health.x == player_x && health.y == player_y) {
        health_ind++;
        health.status = 0;
        health_count = 0;
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