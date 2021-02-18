#include "../mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	void	*mlx;
	void	*win;
	void	*img;
	int h, w;

	mlx = mlx_init();
	win = mlx_new_window(mlx, 1000, 1000, "jimkwon");
	img = mlx_xpm_file_to_image(mlx, "./ex.xpm", &h, &w);
	mlx_put_image_to_window(mlx, win, img, 0, 0);

	mlx_loop(mlx);
}