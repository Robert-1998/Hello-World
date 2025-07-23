#define _BSD_SOURCE
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 24
#define FOV 120  // Увеличил FOV
#define DISTANCE 15

char buffer[HEIGHT][WIDTH];
double zbuffer[HEIGHT][WIDTH];

// Вершины куба
double cube[8][3] = {
    {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
    {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}
};

// Ребра куба
int edges[12][2] = {
    {0,1}, {1,2}, {2,3}, {3,0},  // Задняя грань
    {4,5}, {5,6}, {6,7}, {7,4},  // Передняя грань
    {0,4}, {1,5}, {2,6}, {3,7}   // Соединения
};

void clear_buffer() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            buffer[i][j] = ' ';
            zbuffer[i][j] = -1000;  // Изменил начальное значение
        }
    }
}

void draw_point(int x, int y, char c, double z) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if (z > zbuffer[y][x]) {
            buffer[y][x] = c;
            zbuffer[y][x] = z;
        }
    }
}

void project(double x, double y, double z, int *px, int *py, double *pz) {
    // Исправленная проекция
    double factor = FOV / (z + DISTANCE + 5);  // Добавил смещение
    *px = (int)(x * factor + WIDTH / 2);
    *py = (int)(y * factor + HEIGHT / 2);      // Убрал минус
    *pz = z;
}

// Алгоритм Брезенхема для рисования линии
void draw_line(int x0, int y0, int x1, int y1, double z, char c) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        draw_point(x0, y0, c, z);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void rotate(double point[3], double angle_x, double angle_y, double angle_z, double result[3]) {
    double x = point[0], y = point[1], z = point[2];

    // Поворот вокруг X
    double y1 = y * cos(angle_x) - z * sin(angle_x);
    double z1 = y * sin(angle_x) + z * cos(angle_x);
    y = y1; z = z1;

    // Поворот вокруг Y
    double x2 = x * cos(angle_y) + z * sin(angle_y);
    z1 = -x * sin(angle_y) + z * cos(angle_y);
    x = x2; z = z1;

    // Поворот вокруг Z
    double x3 = x * cos(angle_z) - y * sin(angle_z);
    double y3 = x * sin(angle_z) + y * cos(angle_z);
    x = x3; y = y3;

    result[0] = x;
    result[1] = y;
    result[2] = z;
}

int main() {
    double angle_x = 0, angle_y = 0, angle_z = 0;

    while (1) {
        clear_buffer();
        angle_x += 0.05;
        angle_y += 0.03;
        angle_z += 0.01;

        double projected[8][3];

        // Проекция всех точек
        for (int i = 0; i < 8; i++) {
            double rot[3];
            rotate(cube[i], angle_x, angle_y, angle_z, rot);
            int px, py;
            double pz;
            project(rot[0], rot[1], rot[2], &px, &py, &pz);
            projected[i][0] = px;
            projected[i][1] = py;
            projected[i][2] = pz;
        }

        // Рисование ребер
        for (int i = 0; i < 12; i++) {
            int p1 = edges[i][0];
            int p2 = edges[i][1];
            
            int x1 = (int)projected[p1][0];
            int y1 = (int)projected[p1][1];
            int x2 = (int)projected[p2][0];
            int y2 = (int)projected[p2][1];
            
            double avg_z = (projected[p1][2] + projected[p2][2]) / 2;
            
            draw_line(x1, y1, x2, y2, avg_z, '#');
        }

        // Вывод
        system("clear");
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                putchar(buffer[i][j]);
            }
            putchar('\n');
        }

        usleep(50000);
    }

    return 0;
}