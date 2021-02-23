/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimkwon <jimkwon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 14:48:54 by jimkwon           #+#    #+#             */
/*   Updated: 2021/02/23 17:23:15 by jimkwon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_3D_H
# define CUB_3D_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef struct s_img
{
    void *img;
    int *data;

    int size_l;
    int bpp;
    int endian;
    int height;
    int width;
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

typedef struct s_element
{
    int x_size;
    int y_size;
    char *n_texture;
    char *s_texture;
    char *e_texture;
    char *w_texture;
    char *sprite;
    int f_color;
    int c_color;
} t_element;

typedef struct s_game
{
    void *mlx;
    void *win;
    int wall[1000000];
    t_img img;
    t_img wall2;
    t_user user;
    t_element element;

} t_game;

int			get_next_line(int fd, char **line);
char		*is_in_newline(char *str);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_substr(char *s, unsigned int start, size_t len);
size_t		ft_strlen(const char *str);
char		*ft_strdup(char *s);
size_t		ft_strlcpy(char *dest, const char *src, size_t size);
int			split_new_line(char **line, char **prev, char *index);
int			return_status(char **line, char **prev, char *index);
void		get_next_word(char *line, t_game *g);
static char	*get_word(char *s, int len);
static int	word_len(char *s, char c);
static int	count_len(char *s, char c);
static char	**free_ans(char **a);
char		**ft_split(char const *s, char c);

#endif
