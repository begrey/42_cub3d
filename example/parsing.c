#include "cub_3d.h"

void map_check(t_game *g)
{
    int i;
    int j;

    i = 0;
    while (i < g->element.map_x)
    {
        j = 0;
        while (j < g->element.map_y)
        {
            if (i == 0 || j == 0 || i == g->element.map_x - 1 || j == g->element.map_y - 1)
            {
                if (g->element.map[i][j] == 0 || g->element.map[i][j] == 'W' ||
                    g->element.map[i][j] == 'S' || g->element.map[i][j] == 'E' || g->element.map[i][j] == 'N')
                {
                    printf("MAP ERROR!\n");
                    exit(0);
                }
            }
            j++;
        }
        i++;
    }
}

void parse_map(t_game *g, int fd, int fd2)
{
    char *line;
    char *line2;
    int ret;
    int i;

    i = 0;
    while ((ret = get_next_line(fd, &line)) != -1)
    {
        if (*line != 0)
            get_row_and_col(line, g);
        if (ret == 0)
            break;
    }
    g->element.map = malloc(sizeof(int *) * g->element.x_size);

    while ((ret = get_next_line(fd2, &line2)) != -1)
    {
        if (*line2 != 0)
            get_map(line2, g, i++);
        if (ret == 0)
            break;
    }
}

void parse_line(t_game *g, int fd, int fd2)
{
    char *line;
    char *line2;
    int m;

    m = 0;
    while (get_next_line(fd, &line) != 0 && get_next_line(fd2, &line2) != 0)
    {
        if (*line != 0)
        {
            if (m < 7)
                get_next_word(line, g);
            else
                break;
            m++;
        }
    }
    get_next_word(line, g);
    parse_map(g, fd, fd2);
}

void open_file(t_game *g)
{
    int fd;
    int fd2;

    g->element.map_x = 0;
    g->element.map_y = 0;
    fd = open("./ex.cub", O_RDONLY);
    fd2 = open("./ex.cub", O_RDONLY);
    if (fd == -1 || fd2 == -1)
        printf("ERROR!\n");
    parse_line(g, fd, fd2);
    map_check(g);
    close(fd);
    close(fd2);
}