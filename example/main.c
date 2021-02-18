#include "../mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.1415926535
#define HEIGHT 400
#define WIDTH 400
#define SQ_W 40
#define SQ_H 40
#define KEYPRESS 2
#define ESC 53
#define W 13
#define S 1
#define A 0
#define D 2
#define Q 12
#define E 14
#define USER 10;

typedef struct s_img
{
    void *img;
    int *data;

    int size_l;
    int bpp;
    int endian;
} t_img;

typedef struct s_user
{
    double x;
    double y;
    int turn_d;
    int walk_d;
    double angle;
    int walk_speed;
    double turn_speed;
} t_user;

typedef struct s_game
{
    void *mlx;
    void *win;
    int wall[1000000];
    t_img img;
    t_user user;

} t_game;

int map[10][10] = {
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 0, 0, 1, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1}};

void make_user(t_game *g)
{
    int i, j;
    int x, y;
    i = -1;
    j = -1;
    x = (int)g->user.x - (10 / 2);
    y = (int)g->user.y + (10 / 2);

    while (i++ < 10)
    {
        while (j++ < 10)
            g->img.data[ (x + i) + (y - j) * WIDTH] = 0x7B68EE;
        j = -1;
    }
    i = 1;
    while(i < 30)
    {
        x = g->user.x + (cos(g->user.angle) * i);
        y = g->user.y + (sin(g->user.angle) * i);
        g->img.data[x + y * WIDTH] = 0xFF0000;
        i++;
    }
}

void make_square(t_game *g, int w, int h, int map)
{
    int i, j;
    i = -1;
    j = -1;

    while (j++ < SQ_W - 1)
    {
        while (i++ < SQ_H - 1)
        {
            if (map == 1)
            {
                g->wall[(w + i) + (h + j) * WIDTH] = 1;
                g->img.data[(w + i) + (h + j) * WIDTH] = 0x4682B4;
            }
            else
            {
                g->img.data[(w + i) + (h + j) * WIDTH] = 0xFFFFFF;
            }
            // if ((w % SQ_W == 0 || h % SQ_H == 0) && (i == 0 || j == 0))
            // {
            //     g->img.data[(w + i) + (h + j) * WIDTH] = 0x000000;
            // }
        }
        i = -1;
    }
}

void make_map(t_game *g, int x, int y)
{
    while (y < HEIGHT)
    {
        while (x < WIDTH)
        {
            make_square(g, x, y, map[y / SQ_W][x / SQ_H]);
            x += SQ_W;
        }
        x = 0;
        y += SQ_H;
    }
}
// int is_pass_wall(t_game *g)
// {
//     int status;
//     //기준점이 유저의 사각형 기준 0,0이기 떄문에, n,n의 부분도 염두해야함
//     status = 1;
//     if (g->wall[(g->user.x + 2 + 5) + (g->user.y) * WIDTH] == 1) //D
//         status *= 2;
//     if (g->wall[(g->user.x - 2 - 5) + (g->user.y) * WIDTH] == 1) //A
//         status *= 3;
//     if (g->wall[(g->user.x) + (g->user.y + 5) * WIDTH] == 1) //S
//         status *= 5;
//     if (g->wall[(g->user.x) + (g->user.y - 2 - 5) * WIDTH] == 1) // W
//         status *= 7;
//     return status;
// }

int press_event(int key, t_game *g) //여기서 움직인 위치를
{
    int i, j;
    i = 0;
    j = 0;
    //맵을 벗어나거나 벽에 닿지 않게 예외처리
    if (key == ESC)
        exit(0);
    else if (key == W)
    {
        g->user.x += cos(g->user.angle);
        g->user.y += sin(g->user.angle);
    }
    else if (key == S)
    {
        g->user.x -= cos(g->user.angle);
        g->user.y -= sin(g->user.angle);
    }
    else if (key == A)
        g->user.x -= 2;
    else if (key == D)
        g->user.x += 2;
    else if (key == Q)
        g->user.angle += -1 * g->user.turn_speed;
    else if (key == E)
        g->user.angle += g->user.turn_speed;
    make_map(g, 0, 0);
    make_user(g);
    mlx_put_image_to_window(g->mlx, g->win, g->img.img, 100, 100);
    return 1;
}

int unpress_event(t_game *g)
{
    make_map(g, 0, 0);
    make_user(g);
    mlx_put_image_to_window(g->mlx, g->win, g->img.img, 100, 100);
    return 1;
}

int main()
{
    t_game game;
    t_img img;

    game.mlx = mlx_init();
    game.win = mlx_new_window(game.mlx, 800, 800, "jimkwon");
    game.img.img = mlx_new_image(game.mlx, WIDTH, HEIGHT);
    game.user.x = SQ_W + 20;
    game.user.y = HEIGHT - SQ_H;
    game.user.turn_d = 0;
    game.user.walk_d = 0;
    game.user.angle = PI / 2;
    game.user.walk_speed = 2;
    game.user.turn_speed = 2 * (PI / 180);
    game.img.data = (int *)mlx_get_data_addr(game.img.img, &game.img.bpp, &game.img.size_l, &game.img.endian);

    make_user(&game);                                     //init_user;
    make_map(&game, 0, 0);                                //맵만들기
    mlx_hook(game.win, KEYPRESS, 0, &press_event, &game); //키입력시 유저이동
    mlx_loop_hook(game.mlx, &unpress_event, &game);       //입력 안할시 항시 맵 최신으로 유지
    mlx_loop(game.mlx);
}