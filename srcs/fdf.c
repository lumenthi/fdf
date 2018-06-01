/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 17:57:57 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/01 17:52:07 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"
#include <stdio.h>

int		get_len(char *str)
{
	int		i;
	int		count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]))
		{
			if (str[i + 1] && ft_isdigit(str[i + 1]))
				i++;
			count++;
		}
		i++;
	}
	return (count);
}

void	ft_printcoords(t_point *tab)
{
	int i;

	i = 0;
	while (!tab[i].end)
	{
		printf("(*tab)[%d][x]: %d\n", i, tab[i].x);
		printf("(*tab)[%d][y]: %d\n", i, tab[i].y);
		printf("(*tab)[%d][z]: %d\n", i, tab[i].z);
		printf("____________________________________________\n");
		i++;
	}
}

void	fill_line(t_point **tab, char *str, int y, int *size)
{
	int		i;
	int		x;
	char	*inc;

	i = 0;
	x = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) || str[i] == '-')
		{
			*tab = (t_point *)ft_realloc(*tab, sizeof(t_point) * ((*size) + 1));
			(*tab)[*size].x = x;
			(*tab)[*size].y = y;
			(*tab)[*size].z = ft_atoi(str + i);
			(*tab)[*size].end = 0;
			(*size)++;
			x++;
			inc = ft_itoa(ft_atoi(str + i));
			i = i + ft_strlen(inc);
			free(inc);
		}
		else
			i++;
	}
}

t_point		*get_tab(int fd)
{
	t_point		*tab;
	char	*line;
	int		col;
	int		size;

	size = 0;
	tab = NULL;
	line = NULL;
	col = 0;
	tab = (t_point *)malloc(sizeof(t_point) * 1);
	while (get_next_line(fd, &line))
	{
		fill_line(&tab, line, col, &size);
		free(line);
		col++;
	}
	free(line);
	tab[size].end = 1;
	return (tab);
}

t_point		*reader(char *filename)
{
	int		fd;
	t_point	*tab;

	if ((fd = open(filename, O_RDONLY)) == -1)
	{
		ft_putstr_fd("Invalid file\n", 2);
		return (NULL);
	}
	else
		tab = get_tab(fd);
	return (tab);
}

int		main(int argc, char **argv)
{
	t_point		*tab;

	if (argc == 2)
	{
		if ((tab = reader(argv[1])) == NULL)
			return (-1);
		ft_printcoords(tab);
		free(tab);
	}
	while (1);
	return (1);
}
