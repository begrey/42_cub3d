#include "cub_3d.h"
#include "stdio.h"

typedef struct s_list
{
    char *word;
	struct s_list *next;
} t_list;

int ft_atoi_base(const char *str, int str_base)
{
    int i = 0, minus = 0, len = 0, ans = 0, base = 1, num = 1;

    if (str_base <= 1)
        return 0;
    //맨 뒤부터 계산하기 시작하기
    while (str[i] != '\0')
        i++;
    len = i - 1;
    //1234인 경우, 4부터 시작함
    while (len >= 0)
    {
        if (str[len] == '-' || str[len] == '+') { //첫번째 문자인 경우에반 부호 인정
            if (len != 0)
                return 0;
            if (str[len] == '-')
                minus = 1;
        }
        if (str[len] >= '0' && str[len] <= '9') { //숫자일때  
            num = str[len] - '0';
            ans += num * base;
        }
        else if (str[len] >= 'a' && str[len] <= 'f') { //a ~ f일때
            num = str[len] - 'a' + 10;
            ans += num * base;
        }
        else if (str[len] >= 'A' && str[len] <= 'F') { //A ~ F일때
            num = str[len] - 'A' + 10;
            ans += num * base;
        }
        base *= str_base; //4진수인 경우, 곱하게 될 base 는 계속 4씩 곱해줌
        len --;
    }
    if (minus == 1) return ans * -1;
    else return ans;
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

int				ft_atoi(const char *str)
{
	long long	minus;
	long long	total;

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

int	rgb_to_hex(char *rgb)
{
	int r, g, b, len;
	char **answer;
	char *hex;
    char *s = rgb;
    answer = ft_split(s, ',');
	r = ft_atoi(answer[0]);
	g = ft_atoi(answer[1]);
	b = ft_atoi(answer[2]);
	hex = ft_strjoin(make_hex(r), make_hex(g));
	hex = ft_strjoin(hex, make_hex(b));
	return (ft_atoi_base(hex, 16));
}

int					ft_strncmp(const char *str1, const char *str2, size_t num)
{
	unsigned char	*p1;
	unsigned char	*p2;
	size_t			i;

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

t_list		*ft_lstlast(t_list *lst)
{
	t_list	*temp;

	if (!lst)
		return (NULL);
	temp = lst;
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

void		ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;
	t_list	*temp;

	temp = *lst;
	if (temp == NULL)
	{
		*lst = new;
		new->next = NULL;
		return ;
	}
	temp = *lst;
	last = ft_lstlast(temp);
	last->next = new;
}

t_list		*ft_lstnew(char *content)
{
	t_list	*lst;

	if (!(lst = (t_list *)malloc(sizeof(t_list) * 1)))
		return (NULL);
	lst->word = content;
	lst->next = NULL;
	return (lst);
}

void set_element(t_list *e, t_game *g)
{
	t_list *temp;

	temp = e;
	if (ft_strncmp(temp->word, "R", ft_strlen(temp->word)) == 0)
	{
		g->element.x_size = ft_atoi(temp->word);
		g->element.y_size = ft_atoi(temp->word);
	}
	else if (ft_strncmp(temp->word, "NO", ft_strlen(temp->word)) == 0)
		g->element.n_texture = temp->next->word;
	else if (ft_strncmp(temp->word, "SO", ft_strlen(temp->word)) == 0)
		g->element.s_texture = temp->next->word;
	else if (ft_strncmp(temp->word, "WE", ft_strlen(temp->word)) == 0)
		g->element.w_texture = temp->next->word;
	else if (ft_strncmp(temp->word, "EA", ft_strlen(temp->word)) == 0)
		g->element.e_texture = temp->next->word;
	else if (ft_strncmp(temp->word, "S", ft_strlen(temp->word)) == 0)
		g->element.sprite = temp->next->word;
	else if (ft_strncmp(temp->word, "F", ft_strlen(temp->word)) == 0)
		g->element.f_color = rgb_to_hex(temp->next->word);
	else if (ft_strncmp(temp->word, "C", ft_strlen(temp->word)) == 0)
		g->element.c_color = rgb_to_hex(temp->next->word);
}

void get_next_word(char *line, t_game *g)
{
	int i, j;
	char *word;
	t_list *w, *list;

	list = NULL;
	word = "";
	i = 0;
	j = 0;
	while(line[i] != '\0')
	{
		if (line[i] == ' ')
		{
			w = ft_lstnew(ft_substr(line, j, i - j));
			ft_lstadd_back(&list, w);
			j = i + 1;
		}
		i++;
	}
	w = ft_lstnew(ft_substr(line, j, i - j + 1));
	ft_lstadd_back(&list, w);
	set_element(list, g);
	free(w);
	free(list);
}