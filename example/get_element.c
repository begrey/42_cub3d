#include "cub_3d.h"
#include "stdio.h"

typedef struct s_list
{
	char *word;
	struct s_list *next;
} t_list;

int ft_atoi_base(const char *str, int str_base)
{
	int i = 0, len = 0, ans = 0, base = 1, num = 1;

	while (str[i] != '\0')
		i++;
	len = i - 1;
	while (len >= 0)
	{
		if (str[len] >= '0' && str[len] <= '9')
		{
			num = str[len] - '0';
			ans += num * base;
		}
		else if (str[len] >= 'A' && str[len] <= 'F')
		{
			num = str[len] - 'A' + 10;
			ans += num * base;
		}
		base *= str_base;
		len--;
	}
	return ans;
}

char *make_hex(int n)
{
	//rgb에서 hex로 갈때, major + minor 로 구성된다(16진수) 181 = b5
	char *hex;
	char major, minor;

	if (!(hex = (char *)malloc(sizeof(char) * 3)))
		return (NULL);
	major = n / 16 + '0';
	minor = n % 16 + '0';
	if (major > 57)
		major += 7;
	if (minor > 57)
		minor += 7;

	hex[0] = major;
	hex[1] = minor;
	hex[2] = '\0';
	return hex;
}

int ft_atoi(const char *str)
{
	long long minus;
	long long total;

	minus = 1;
	total = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			minus = -1;
		str++;
	}
	if (*str != '\0' && (*str >= '0' && *str <= '9'))
		total += *str++ - '0';
	while (*str != '\0' && (*str >= '0' && *str <= '9'))
	{
		if (minus == 1 && total > 2147483647)
			return (-1);
		if (minus == -1 && total > 2147483648)
			return (0);
		total *= 10;
		total += *str++ - '0';
	}
	return ((int)(total * minus));
}

int rgb_to_hex(int *color)
{
	int r, g, b, len;
	char *hex;
	r = color[0];
	g = color[1];
	b = color[2];
	hex = ft_strjoin(make_hex(r), make_hex(g));
	hex = ft_strjoin(hex, make_hex(b));
	return (ft_atoi_base(hex, 16));
}

int ft_strncmp(const char *str1, const char *str2, size_t num)
{
	unsigned char *p1;
	unsigned char *p2;
	size_t i;

	i = 0;
	p1 = (unsigned char *)str1;
	p2 = (unsigned char *)str2;
	while (i < num && *p1 != '\0' && *p2 != '\0')
	{
		if (*p1 != *p2)
			return (*p1 - *p2);
		p1++;
		p2++;
		i++;
	}
	if (i == num)
		return (0);
	else if (*p1 != '\0' || *p2 != '\0')
		return (*p1 - *p2);
	return (0);
}

void handle_color(t_game *g, char **color, char type)
{
	int i;
	int j;
	int c[3];

	i = 0;
	j = 0;
	while(i < 3)
	{
		if (ft_atoi(color[i]) > 255 && ft_atoi(color[i]) < 0)
			print_error("WRONG COLOR INPUT!");
		c[j] = ft_atoi(color[i]);
		j++;
		i++;
	}
	if (type == 'F')
		g->element.f_color = rgb_to_hex(c);
	else
		g->element.c_color = rgb_to_hex(c);
}

void handle_size(t_game *g, char **word)
{
	if (word[3] != NULL)
		print_error("WRONG SIZE NUM!");
	g->element.x_size = ft_atoi(word[1]);
	g->element.y_size = ft_atoi(word[2]);
}

char *handle_texture(t_game *g, char **word)
{
	if (word[2] != NULL)
		print_error("WRONG TEXTURE NUM!");
	return (word[1]);
}

void seperate_type(t_game *g, char *type, char **word)
{
	if (ft_strncmp(type, "R", ft_strlen(type)) == 0)
		handle_size(g, word);
	else if (ft_strncmp(type, "NO", ft_strlen(type)) == 0)
		g->element.n_texture = handle_texture(g, word);
	else if (ft_strncmp(type, "S", ft_strlen(type)) == 0)
		g->element.sprite = handle_texture(g, word);
	else if (ft_strncmp(type, "SO", ft_strlen(type)) == 0)
		g->element.s_texture = handle_texture(g, word);
	else if (ft_strncmp(type, "WE", ft_strlen(type)) == 0)
		g->element.w_texture = handle_texture(g, word);
	else if (ft_strncmp(type, "EA", ft_strlen(type)) == 0)
		g->element.e_texture = handle_texture(g, word);
	else
		print_error("WRONG TYPE!");
}

void get_next_word(char *line, t_game *g)
{
	char **word;
	char **color;
	char* type;
	t_list* w;
	int comma;
	int i;

	i = 0;
	comma = 0;
	word = ft_split(line, ' ');
	type = word[0];
	if (ft_strncmp(type, "F", ft_strlen(type)) == 0 ||
	ft_strncmp(type, "C", ft_strlen(type)) == 0)
	{
		while(line[i] != '\0')
		{
			if (line[i] == ',')
				comma++;
			i++;
		}
		if (comma != 2)
			print_error("WRONG COMMA NUM!");
		color = ft_split(line + 1, ',');
		handle_color(g, color, line[0]);
		return;
	}
	seperate_type(g, type, word);
}

void get_row_and_col(char *line, t_game *g)
{
	g->element.map_x += 1;					//행
	if (g->element.map_y < ft_strlen(line)) //열
		g->element.map_y = ft_strlen(line);
}

void get_map(char *line, t_game *g, int i)
{
	int j;

	j = 0;
	g->element.map[i] = malloc(sizeof(int) * g->element.map_y);
	while (line[j] != '\0')
	{
		if (line[j] == ' ')
			g->element.map[i][j] = 3;
		else if (line[j] >= '0' && line[j] <= '2')
			g->element.map[i][j] = line[j] - '0';
		else if (line[j] == 'N' || line[j] == 'W' || line[j] == 'E' || line[j] == 'S')
			g->element.map[i][j] = line[j];
		else
			print_error("WRONG MAP INPUT!");
		j++;
	}
	while (j < g->element.map_y)
	{
		g->element.map[i][j] = 3;
		j++;
	}
}