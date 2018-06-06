/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 17:57:57 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/06 11:42:59 by lumenthi         ###   ########.fr       */
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

void	ft_printcoords(t_mlx data)
{
	int i;

	i = 0;
	while (i < data.prop.size)
	{
		printf("(*tab)[%d][x]: %d\n", i, data.tab[i].x);
		printf("(*tab)[%d][y]: %d\n", i, data.tab[i].y);
		printf("(*tab)[%d][z]: %d\n", i, data.tab[i].z);
		printf("____________________________________________\n");
		i++;
	}
	printf("data->size: %d\n", data.prop.size);
	printf("data->max_x: %d\n", data.prop.max_x);
	printf("data->max_y: %d\n", data.prop.max_y);
	printf("data->max_z: %d\n", data.prop.max_z);
}

void	fill_line(t_mlx *data, char *str, int y, int *size)
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
//			printf("size: %d\n", *size);
//			printf("before_malloc\n");
			data->tab = (t_point *)ft_realloc(data->tab, sizeof(t_point) * ((*size) + 1));
//			printf("after_malloc\n");
			data->tab[*size].x = x;
			data->tab[*size].y = y;
			data->tab[*size].z = ft_atoi(str + i);
			if (x > data->prop.max_x)
				data->prop.max_x = x;
			if (y > data->prop.max_y)
				data->prop.max_y = y;
			if (data->tab[*size].z > data->prop.max_z)
				data->prop.max_z = data->tab[*size].z;
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

void	get_tab(int fd, t_mlx *data)
{
	char	*line;
	int		col;
	int		size;

	size = 0;
	line = NULL;
	col = 0;
	data->prop.max_x = 0;
	data->prop.max_y = 0;
	data->prop.max_z = 0;
	data->tab = (t_point *)malloc(sizeof(t_point) * 1);
	while (get_next_line(fd, &line))
	{
		fill_line(data, line, col, &size);
		free(line);
		col++;
	}
	free(line);
	data->prop.size = size;
}

int		reader(char *filename, t_mlx *data)
{
	int		fd;

	if ((fd = open(filename, O_RDONLY)) == -1)
	{
		ft_putstr_fd("Invalid file\n", 2);
		return (0);
	}
	else
		get_tab(fd, data);
	return (1);
}

t_point *tab_resize(t_mlx data, int zoom)
{
	int		i;

	i = 0;
	while (i < data.prop.size)
	{
		data.tab[i].x *= zoom;
		data.tab[i].y *= zoom;
		data.tab[i].z *= zoom;
		i++;
	}
	return (data.tab);
}

void	just_draw(int x0, int y0, int x1, int y1, t_mlx data)
{
	int dx = abs(x1-x0);
	int dy = abs(y1-y0);
	signed short sx,sy;
	signed int err,e2;

	if (x0 < x1) {sx = 1;} else {sx = -1;}
	if (y0 < y1) {sy = 1;} else {sy = -1;}
	err = dx-dy;
	while (!(x0==x1 && y0==y1))
	{
		mlx_pixel_put(data.mlx, data.win, x0, y0, 0250250250);
		e2 = 2*err;
		if (e2 > -dy)
		{
			err = err - dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err = err + dx;
			y0 += sy;
		}
	}
}

void	draw_lines(t_mlx data)
{
	int		pos;
	t_point	t_p;

	pos = 0;
	while (pos < data.prop.size)
	{
		printf("data.pos.x = %d\n", data.tab[pos].x);
		printf("data.max_x = %d\n", data.prop.max_x);
		if (data.tab[pos].y < data.prop.max_y * data.cam.zoom)
		{
			t_p = data.tab[pos + data.prop.max_x + 1];
			if (data.tab[pos].x == t_p.x)
				just_draw(data.tab[pos].u, data.tab[pos].v, t_p.u, t_p.v, data);
		}
		if (data.tab[pos].x < data.prop.max_x * data.cam.zoom)
		{
			t_p = data.tab[pos + 1];
			just_draw(data.tab[pos].u, data.tab[pos].v, t_p.u, t_p.v, data);
		}
		pos++;
	}
}

t_point	to_isometric(t_point tab)
{
	int	u;
	int	v;

	u = (tab.x - tab.z)/sqrt(2);
	v = (tab.x + 2 * tab.y + tab.z)/sqrt(6);
	tab.u = u;
	tab.v = v;
	return (tab);
}

void	display_tab(t_mlx data)
{
	int		pos = 0;
	printf("cam_zoom: %d\n", data.cam.zoom);
	data.tab = tab_resize(data, data.cam.zoom);
	while (pos < data.prop.size)
	{
		data.tab[pos] = to_isometric(data.tab[pos]);
		if (data.tab[pos].z > 0)
			mlx_pixel_put(data.mlx, data.win, data.tab[pos].u, data.tab[pos].v, 0250250250);
		else
			mlx_pixel_put(data.mlx, data.win, data.tab[pos].u, data.tab[pos].v, 0666);
		pos++;
	}
	draw_lines(data);
}

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

int		get_zoom(t_mlx data)
{
	int		max;
	int		zoom;

	if (data.prop.max_x > data.prop.max_y)
		max = data.prop.max_x;
	else
		max = data.prop.max_y;
	zoom = WIN_X / max;
	return (zoom);
}

int		main(int argc, char **argv)
{
	t_mlx		data;

	if (argc == 2)
	{
		if (!reader(argv[1], &data))
			return (-1);
//		ft_printcoords(data);
		data.cam.zoom = get_zoom(data);
		new_window(&data);
		display_tab(data);
		mlx_key_hook(data.win, ft_key, &data);
		free(data.tab);
		mlx_loop(data.mlx);
	}
	return (1);
}
