/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 17:57:57 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/31 22:54:12 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"
#include <stdio.h>

void	ft_printint(int **tab)
{
	int i;

	i = 0;
	while (tab[i])
	{
		ft_putstr("tab[");
		ft_putnbr(i);
		ft_putstr("]: ");
		int j = 0;
		while (tab[j])
		{
			ft_putnbr(tab[i][j]);
			ft_putchar(' ');
			j++;
		}
		ft_putchar('\n');
		i++;
	}
}

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

int		*fill_line(char *str)
{
	int		i;
	int		cur;
	int		*tab;

	tab = NULL;
//	printf("get_len(%s): %d\n", str, get_len(str));
	tab = (int *)malloc(sizeof(int) * get_len(str));
	cur = 0;
	i = 0;
	while (str[i])
	{
//		printf("str[%d]: %d\n", i, str[i] - '0');
		if (ft_isdigit(str[i]))
		{
//			printf("str[%d]: %d\n", i, str[i] - '0');
			tab[cur] = str[i] - '0';
			if (str[i + 1] && ft_isdigit(str[i + 1]))
			{
				tab[cur] = (tab[cur] * 10) + (str[i + 1] - '0');
				i++;
			}
//			printf("tab[%d] = %d\n", cur, tab[cur]);
			cur++;
		}
		i++;
	}
	tab[cur] = -1;
	return (tab);
}

void	print_line(int *tab)
{
	int i = 0;
	while (tab[i] != -1)
	{
		ft_putnbr(tab[i]);
		ft_putchar(' ');
		i++;
	}
	ft_putchar('\n');
}

int		**get_tab(int fd)
{
	int		**tab;
	char	*line;
	int		col;

	tab = NULL;
	line = NULL;
	col = 0;
//	tab = (int **)malloc(sizeof(int *) * 99);
	tab = (int **)malloc(sizeof(int *) * 1);
	while (get_next_line(fd, &line))
	{
		tab = (int **)ft_realloc(tab, sizeof(int *) * (col + 1));
		tab[col] = fill_line(line);
		print_line(tab[col]);
		col++;
	}
	tab[col] = NULL;
//	ft_printint(tab);
	return (tab);
}

int		**reader(char *filename)
{
	int		fd;
	int		**tab;

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
	int		**tab;

	if (argc == 2)
	{
		if ((tab = reader(argv[1])) == NULL)
			return (-1);
	}
	return (1);
}
