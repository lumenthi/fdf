/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 17:57:57 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/04 22:39:34 by lumenthi         ###   ########.fr       */
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

void	display_tab(t_mlx data);

int		ft_key(int key, t_mlx *data)
{
	void	*image_ptr;
	printf("key: %d\n", key);
	if (key == 126)
	{
		image_ptr = mlx_new_image(data->mlx, WIN_X, WIN_Y);
		data->cam.zoom += 1;
		mlx_put_image_to_window(data->mlx, data->win, image_ptr, 0, 0);
		display_tab(*data);
	}
	else if (key == 125 && data->cam.zoom > 0)
	{
		data->cam.zoom -= 1;
		display_tab(*data);
	}
	else if (key == 53)
	{
		ft_putendl("ESC key pressed, exiting.");
		exit(1);
	}
	return (0);
}

int		new_window(t_mlx *data)
{
	if (!(data->mlx = mlx_init()))
		return (0);
	data->win = mlx_new_window(data->mlx, WIN_X, WIN_Y, WIN_TITLE);
	return (1);
}

t_point *tab_resize(t_point *tab, int zoom)
{
	int		i;
	int		zoom_x;
	int		zoom_y;

	zoom_x = zoom;
	zoom_y = zoom;
	i = 0;
	while (!tab[i].end)
	{
		tab[i].x += zoom_x;
		tab[i].y += zoom_y;
		if (tab[i].y - zoom_y != tab[i + 1].y)
		{
			zoom_x = zoom;
			zoom_y += (zoom / 2);
		}
		else
			zoom_x += (zoom /2);
		i++;
	}
	return (tab);
}

void	display_tab(t_mlx data)
{
	int		pos = 0;
	printf("cam_zoom: %d\n", data.cam.zoom);
	data.tab = tab_resize(data.tab, data.cam.zoom);
	while (!data.tab[pos].end)
	{
		if (data.tab[pos].z > 0)
			mlx_pixel_put(data.mlx, data.win, data.tab[pos].x, data.tab[pos].y, 0250250250);
		else
			mlx_pixel_put(data.mlx, data.win, data.tab[pos].x, data.tab[pos].y, 0666);
		pos++;
	}
}

int		main(int argc, char **argv)
{
	t_mlx		data;

	if (argc == 2)
	{
		if ((data.tab = reader(argv[1])) == NULL)
			return (-1);
		data.cam.zoom = 10;
		new_window(&data);
		display_tab(data);
		mlx_key_hook(data.win, ft_key, &data);
		free(data.tab);
		mlx_loop(data.mlx);
	}
	return (1);
}
