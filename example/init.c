#include "cub_3d.h"
#include "../mlx/mlx.h"
#define PI 3.1415926535

void init_user(t_game *g)
{
    g->SQ_H = g->element.y_size / 10;
    g->SQ_W = g->element.x_size / 10;
    g->user.x = g->SQ_W + 10;
    g->user.y = g->element.y_size - g->SQ_W - 10;
    g->user.turn_d = 0;
    g->user.walk_d = 0;
    g->user.angle = PI * 3 / 2;
    g->user.walk_speed = 1;
    g->user.turn_speed = 1 * (PI / 180);
    g->x_walk = 0;
    g->y_walk = 0;
    g->rotate = 0;
}