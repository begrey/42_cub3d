#include "../mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	void	*mlx, *win;
	int		img;

	mlx = mlx_init();
	win = mlx_new_window(mlx, 1000, 1000, "jimkwon's workspace");

	img = mlx_pixel_put(mlx, win, 0, 0, 1);
	mlx_put_image_to_window(mlx, win, &img, 0, 0);

	mlx_loop(mlx);
}