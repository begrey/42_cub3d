#include "../mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cub_3d.h"
#define PI 3.1415926535
#define HEIGHT 401
#define WIDTH 401
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
#define RAY_ANGLE 60;

int map[10][10] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

void draw(t_game *g, int x, int y, int color)
{
    g->img.data[x + y * WIDTH] = color;
}

void make_wall(t_game *g, double x, double y, int num, double angle)
{
    int ray_x, ray_y, i;
    int start = HEIGHT / 2;
    int colora, colorb;
    double distance, ratio;

    ray_x = g->user.x - x;
    ray_y = g->user.y - y;
    distance = sqrt(ray_x * ray_x + ray_y * ray_y) * cos(angle);
    i = 0;
    ratio = 1 / (distance / 5);

    while (i < HEIGHT / 2)
    {
        if (i < ratio * 1000)
        {
            colora = g->wall2.data[i + (ray_x % SQ_H) * g->wall2.width];
            colorb = g->wall2.data[i + (ray_x % SQ_H) * g->wall2.width];
        }
        else
        {
            colora = g->element.c_color;
            colorb = g->element.f_color;
        }
        draw(g, num, start + i, colora);
        draw(g, num, start - i , colorb);
        i++;
    }
}

void make_user(t_game *g)
{
    int i, j;
    int x, y;
    double angle, cos_angle;
    i = -1;
    j = -1;
    x = (int)g->user.x - (10 / 2);
    y = (int)g->user.y + (10 / 2);
    cos_angle = -60 / 2 * PI / 180;

    while (i++ < 10)
    {
        while (j++ < 10)
            draw(g, x + i, y - j, 0x7B68EE);
        j = -1;
    }
    j = 1;
    i = 0;
    angle = g->user.angle;
    g->user.angle += cos_angle;
    while (i < WIDTH)
    {
        g->user.angle += 60 / (double)WIDTH * PI / 180;
        cos_angle += 60 / (double)WIDTH * PI / 180;
        while (1)
        {
            x = g->user.x + (cos(g->user.angle) * j);
            y = g->user.y + (sin(g->user.angle) * j);
            //draw(g, x, y, g->element.c_color);
            if (map[y / SQ_H][x / SQ_W] == 1)
            {
                make_wall(g, x, y, i, cos_angle);
                break;
            }
            j++;
        }
        j = 1;
        i++;
    }
    g->user.angle = angle;
}

void make_map(t_game *g, int x, int y)
{
    int i, j;
    i = -1;
    j = -1;
    while (i++ <= HEIGHT)
    {
        while (j++ <= WIDTH)
        {
            if (i % SQ_H != 0 && j % SQ_W != 0)
            {
                if (map[i / SQ_H][j / SQ_W] == 1)
                {
                    g->wall[j + i * WIDTH] = 1;
                    draw(g, j, i, 0x4682B4);
                }
                else if (map[i / SQ_H][j / SQ_W] == 0)
                {
                    draw(g, j, i, 0xFFFFFF);
                }
            }
            else
                draw(g, j, i, 0x000000);
        }
        j = -1;
    }
}
int is_pass_wall(t_game *g)
{
    int status;
    //x좌표와 y좌표를 따로 계산해서 구한다.
    status = 1;
    if (map[(int)((g->user.y - sin(g->user.angle - (PI / 2)) * 12) / 40)]
           [(int)(g->user.x / 40)] ||
        map[(int)(g->user.y / 40)]
           [(int)((g->user.x - cos(g->user.angle - (PI / 2)) * 12) / 40)]) //D
        status *= 2;
    if (map[(int)((g->user.y - sin(g->user.angle + (PI / 2)) * 12) / 40)]
           [(int)(g->user.x / 40)] ||
        map[(int)(g->user.y / 40)]
           [(int)((g->user.x - cos(g->user.angle + (PI / 2)) * 12) / 40)]) //A
        status *= 3;
    if (map[(int)((g->user.y - sin(g->user.angle) * 12) / 40)]
           [(int)(g->user.x / 40)] ||
        map[(int)(g->user.y / 40)]
           [(int)((g->user.x - cos(g->user.angle) * 12) / 40)]) //S
        status *= 5;
    if (map[(int)((g->user.y + sin(g->user.angle) * 12) / 40)]
           [(int)(g->user.x / 40)] ||
        map[(int)(g->user.y / 40)]
           [(int)((g->user.x + cos(g->user.angle) * 12) / 40)]) // W
        status *= 7;
    return status;
}

int press_event(int key, t_game *g) //여기서 움직인 위치를
{
    int i, j;
    i = 0;
    j = 0;
    //맵을 벗어나거나 벽에 닿지 않게 예외처리
    if (key == ESC)
        exit(0);
    else if (key == W && is_pass_wall(g) % 7 != 0)
    {
        g->user.x += cos(g->user.angle);
        g->user.y += sin(g->user.angle);
    }
    else if (key == S && is_pass_wall(g) % 5 != 0)
    {
        g->user.x -= cos(g->user.angle);
        g->user.y -= sin(g->user.angle);
    }
    else if (key == A && is_pass_wall(g) % 3 != 0)
    {
        g->user.x += cos(g->user.angle - (PI / 2));
        g->user.y += sin(g->user.angle - (PI / 2));
    }
    else if (key == D && is_pass_wall(g) % 2 != 0)
    {
        g->user.x += cos(g->user.angle + (PI / 2));
        g->user.y += sin(g->user.angle + (PI / 2));
    }
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
    int i, j;
    i = 0;
    j = 0;
    make_map(g, 0, 0);
    make_user(g);
    mlx_put_image_to_window(g->mlx, g->win, g->img.img, 100, 100);
    return 1;
}

int main()
{
    t_game game;
    int fd;
	char *line;
    int h, w;

	fd = open("./ex.cub", O_RDONLY);

	if (fd == -1)
		printf("ERROR!\n");
	while(get_next_line(fd, &line) != 0)
	{
        if (*line != 0)
		    get_next_word(line, &game);
	}
    close(fd);
    game.mlx = mlx_init();
    game.win = mlx_new_window(game.mlx, 800, 800, "jimkwon");
    game.img.img = mlx_new_image(game.mlx, WIDTH, HEIGHT);
    game.wall2.img = mlx_xpm_file_to_image(game.mlx, "./wall.xpm", &game.wall2.height, &game.wall2.width);
    game.wall2.data = (int *)mlx_get_data_addr(game.wall2.img, &game.wall2.bpp, &game.wall2.size_l, &game.wall2.endian);
    game.user.x = SQ_W + 20;
    game.user.y = HEIGHT - 50;
    game.user.turn_d = 0;
    game.user.walk_d = 0;
    game.user.angle = PI * 3 / 2;
    game.user.walk_speed = 2;
    game.user.turn_speed = 8 * (PI / 180);
    game.img.data = (int *)mlx_get_data_addr(game.img.img, &game.img.bpp, &game.img.size_l, &game.img.endian);

    make_map(&game, 0, 0);                                //맵만들기
    make_user(&game);                                     //init_user;
    mlx_hook(game.win, KEYPRESS, 0, &press_event, &game); //키입력시 유저이동
    mlx_loop_hook(game.mlx, &unpress_event, &game);       //입력 안할시 항시 맵 최신으로 유지
    mlx_loop(game.mlx);
}