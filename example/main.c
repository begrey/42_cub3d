#include "../mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cub_3d.h"
#define PI 3.1415926535
#define KEYPRESS 2
#define ESC 53
#define W 13
#define S 1
#define A 0
#define D 2
#define Q 12
#define E 14
#define USER 10;
#define RAY_ANGLE 60;

void draw(t_game *g, int x, int y, int color)
{
    g->img.data[x + y * g->element.x_size] = color;
}
void make_wall(t_game *g, double x, double y, int num, double angle)
{
    int i, w_x, w_y;
    int start = g->element.y_size / 2;
    int colora, colorb, color, h;
    double distance, ratio, ray_x, ray_y;
    ray_x = g->user.x - x;
    ray_y = g->user.y - y;
    distance = sqrt(ray_x * ray_x + ray_y * ray_y) * cos(angle);
    i = 0;
    ratio = (double)1 / (distance / 10) * g->element.y_size * ((double)1000 / 400);
    while (i < g->element.y_size / 2)
    {
        if (i < (int)(ratio))
        {
            h = g->wall_e.height / 2;
            w_x = (int)x % g->SQ_W;
            w_y = (int)y % g->SQ_H;
            if (w_x == 0) //동
            {
                colora = g->wall_e.data[(int)(w_y * (g->wall_e.width) / (double)(g->SQ_H)) + (h - (int)(i * h / (ratio))) * g->wall_e.width];
                colorb = g->wall_e.data[(int)(w_y * (g->wall_e.width) / (double)(g->SQ_H)) + (h + (int)(i * h / (ratio))) * g->wall_e.width];
            }
            if (w_y == 0) //남
            {
                colora = g->wall_s.data[(int)(g->wall_s.width - (w_x * (g->wall_s.width / (double)(g->SQ_H)))) + (h - (int)(i * h / (ratio))) * g->wall_s.width];
                colorb = g->wall_s.data[(int)(g->wall_s.width - (w_x * (g->wall_s.width / (double)(g->SQ_H)))) + (h + (int)(i * h / (ratio))) * g->wall_s.width];
            }
            if (w_x == (g->SQ_H - 1)) // 서
            {
                colora = g->wall_w.data[(int)(g->wall_w.width - (w_y * (g->wall_w.width / (double)(g->SQ_H)))) + (h - (int)(i * h / (ratio))) * g->wall_w.width];
                colorb = g->wall_w.data[(int)(g->wall_w.width - (w_y * (g->wall_w.width / (double)(g->SQ_H)))) + (h + (int)(i * h / (ratio))) * g->wall_w.width];
            }
            if (w_y == (g->SQ_H - 1)) // 북
            {
                colora = g->wall_n.data[(int)(w_x * (g->wall_n.width / (double)(g->SQ_H))) + (h - (int)(i * h / (ratio))) * g->wall_n.width];
                colorb = g->wall_n.data[(int)(w_x * (g->wall_n.width / (double)(g->SQ_H))) + (h + (int)(i * h / (ratio))) * g->wall_n.width];
            }
            draw(g, num, start + i, colorb);
            draw(g, num, start - i, colora);
        }
        else
        {
            draw(g, num, start + i, g->element.c_color);
            draw(g, num, start - i, g->element.f_color);
        }
        i++;
    }
}

void make_sprite(t_game *g, double x, double y)
{
    int s_x, s_y;
}

void make_user(t_game *g)
{
    int i, j;
    int x, y;
    double angle, cos_angle;
    cos_angle = -60 / 2 * PI / 180;
    j = 1;
    i = 0;
    angle = g->user.angle;
    g->user.angle += cos_angle;
    while (i < g->element.x_size)
    {
        g->user.angle += 60 / (double)g->element.x_size * PI / 180;
        cos_angle += 60 / (double)g->element.x_size * PI / 180;
        while (1)
        {
            x = g->user.x + (cos(g->user.angle) * j);
            y = g->user.y + (sin(g->user.angle) * j);
            //draw(g, x, y, g->element.c_color);
            if (g->element.map[y / g->SQ_H][x / g->SQ_W] == 1)
            {
                make_wall(g, x, y, i, cos_angle);
                break;
            }
            else if (g->element.map[y / g->SQ_H][x / g->SQ_W] == 2)
            {
                make_sprite(g, x, y);
                break;
            }
            j++;
        }
        j = 1;
        i++;
    }
    g->user.angle = angle;
}

int press_event(int key, t_game *g)
{
    if (key == ESC)
        exit(0);
    else if (key == W)
    {
        g->x_walk = cos(g->user.angle);
        g->y_walk = sin(g->user.angle);
    }
    else if (key == S)
    {
        g->x_walk = -cos(g->user.angle);
        g->y_walk = -sin(g->user.angle);
    }
    else if (key == A)
    {
        g->x_walk = cos(g->user.angle - (PI / 2));
        g->y_walk = sin(g->user.angle - (PI / 2));
    }
    else if (key == D)
    {
        g->x_walk = cos(g->user.angle + (PI / 2));
        g->y_walk = sin(g->user.angle + (PI / 2));
    }
    else if (key == Q)
        g->rotate = -1 * g->user.turn_speed;
    else if (key == E)
        g->rotate = g->user.turn_speed;
    return 1;
}
int unpress_event(int key, t_game *g)
{
    if (key == W || key == S || key == A || key == D)
    {
        g->x_walk = 0;
        g->y_walk = 0;
    }
    else if (key == Q || key == E)
        g->rotate = 0;
    return 1;
}
int main_loop(t_game *g)
{
    if ((g->element.map[(int)(g->user.y + g->y_walk) / g->SQ_H][(int)(g->user.x) / g->SQ_W]) != 1 &&
        (g->element.map[(int)(g->user.y) / g->SQ_H][(int)(g->user.x + g->x_walk) / g->SQ_W]) != 1)
    {
        g->user.x += g->x_walk;
        g->user.y += g->y_walk;
        g->user.angle += g->rotate;
    }
    make_user(g);
    mlx_put_image_to_window(g->mlx, g->win, g->img.img, 0, 0);
    return 1;
}
int main()
{
    t_game game;

    open_file(&game);
    game.mlx = mlx_init();
    game.win = mlx_new_window(game.mlx, game.element.x_size, game.element.y_size, "jimkwon");
    game.img.img = mlx_new_image(game.mlx, game.element.x_size, game.element.y_size);
    game.wall_e.img = mlx_xpm_file_to_image(game.mlx, game.element.e_texture, &game.wall_e.height, &game.wall_e.width);
    game.wall_n.img = mlx_xpm_file_to_image(game.mlx, game.element.n_texture, &game.wall_n.height, &game.wall_n.width);
    game.wall_s.img = mlx_xpm_file_to_image(game.mlx, game.element.s_texture, &game.wall_s.height, &game.wall_s.width);
    game.wall_w.img = mlx_xpm_file_to_image(game.mlx, game.element.w_texture, &game.wall_w.height, &game.wall_w.width);
    game.wall_e.data = (int *)mlx_get_data_addr(game.wall_e.img, &game.wall_e.bpp, &game.wall_e.size_l, &game.wall_e.endian);
    game.wall_n.data = (int *)mlx_get_data_addr(game.wall_n.img, &game.wall_n.bpp, &game.wall_n.size_l, &game.wall_n.endian);
    game.wall_s.data = (int *)mlx_get_data_addr(game.wall_s.img, &game.wall_s.bpp, &game.wall_s.size_l, &game.wall_s.endian);
    game.wall_w.data = (int *)mlx_get_data_addr(game.wall_w.img, &game.wall_w.bpp, &game.wall_w.size_l, &game.wall_w.endian);
    init_user(&game);
    game.img.data = (int *)mlx_get_data_addr(game.img.img, &game.img.bpp, &game.img.size_l, &game.img.endian);
    mlx_hook(game.win, 3, 0, &unpress_event, &game);
    mlx_hook(game.win, KEYPRESS, 0, &press_event, &game);
    mlx_loop_hook(game.mlx, &main_loop, &game);
    mlx_loop(game.mlx);
}
