#include "../mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
//w : 13 s : 1 esc : 53
typedef struct s_mlx {
	void *mlx;
	void *win;
	int	 x;
}		t_mlx;
int key_hook(int key, t_mlx *_mlx)
{
	if (key == 13) _mlx->x++;
	else if (key == 1) _mlx->x--;
	printf("%d\n", _mlx->x);
	if (key == 53)
		exit(0);
	return (0);
}
int main()
{
	struct s_mlx _mlx;
	_mlx.x = 0;

	_mlx.mlx = mlx_init();
	_mlx.win = mlx_new_window(_mlx.mlx, 300, 300, "jimkwon");	
	mlx_hook(_mlx.win, 02, 0, &key_hook, &_mlx);
	mlx_loop(_mlx.win);
}