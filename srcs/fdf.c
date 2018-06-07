/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 17:57:57 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/07 19:47:50 by lumenthi         ###   ########.fr       */
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
		if (data.tab[i].b_z > 0)
		{
			printf("(*tab)[%d][x]: %d\n", i, data.tab[i].x);
			printf("(*tab)[%d][y]: %d\n", i, data.tab[i].y);
			printf("(*tab)[%d][z]: %d\n", i, data.tab[i].z);
			printf("____________________________________________\n");
		}
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
			data->tab = (t_point *)ft_realloc(data->tab, sizeof(t_point) * ((*size) + 1));
			data->tab[*size].x = x;
			data->tab[*size].y = y;
			data->tab[*size].z = ft_atoi(str + i);
			data->tab[*size].b_x = x;
			data->tab[*size].b_y = y;
			data->tab[*size].b_z = data->tab[*size].z;
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

t_point	to_isometric(t_point tab)
{
	int	u;
	int	v;

	u = (tab.x + tab.z)/sqrt(2);
	v = (tab.x + 2 * tab.y - tab.z)/sqrt(6);
	tab.u = u;
	tab.v = v;
	return (tab);
}

t_point *tab_zoom(t_mlx data, int zoom)
{
	int		i;

	i = 0;
	while (i < data.prop.size)
	{
		data.tab[i].x = data.tab[i].b_x * zoom + WIN_X / 6;
		data.tab[i].y = data.tab[i].b_y * zoom + WIN_X / 6;
		data.tab[i].z = data.tab[i].b_z * zoom + WIN_X / 6;
		i++;
	}
	data.prop.max_x += WIN_X / 6;
	data.prop.max_y += WIN_X / 6;
	return (data.tab);
}

t_point *tab_iso(t_mlx data)
{
	int		i;

	i = 0;
	while (i < data.prop.size)
	{
		data.tab[i] = to_isometric(data.tab[i]);
		i++;
	}
	return (data.tab);
}

void	add_pixel(char *image, int x, int y, int color, int bpp)
{
	if (x < 0 || x >= WIN_X || y < 0 || y >= WIN_Y)
		return ;
	*(int *)(image + ((x + y * WIN_X) * bpp)) = color;
}

void	just_draw(int x0, int y0, int x1, int y1, char *image, int bpp)
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
		add_pixel(image, x0, y0, 250, bpp);
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
		if (data.tab[pos].b_y * data.cam.zoom < data.prop.max_y * data.cam.zoom)
		{
			t_p = data.tab[pos + data.prop.max_x + 1];
			if (data.tab[pos].b_x * data.cam.zoom == t_p.b_x * data.cam.zoom)
				just_draw(data.tab[pos].u, data.tab[pos].v, t_p.u, t_p.v, data.img.data, data.img.bpp);
		}
		if (data.tab[pos].b_x * data.cam.zoom < data.prop.max_x * data.cam.zoom)
		{
			t_p = data.tab[pos + 1];
			just_draw(data.tab[pos].u, data.tab[pos].v, t_p.u, t_p.v, data.img.data, data.img.bpp);
		}
		pos++;
	}
}

t_mlx	rotate_x(t_mlx data, float v)
{
	int		i;

	i = 0;
	while (i < data.prop.size)
	{
		data.tab[i].y = data.tab[i].y*cos(v) - data.tab[i].z*sin(v);
		data.tab[i].z = data.tab[i].y*sin(v) + data.tab[i].z*cos(v);
		i++;
	}
	return (data);
}

t_mlx	rotate_y(t_mlx data, float v)
{
	int		i;

	i = 0;
	while (i < data.prop.size)
	{
		if (i == 2)
		{
			float bite_x = data.tab[i].x*cos(v) + data.tab[i].z*sin(v);
			float bite_z = data.tab[i].z*cos(v) + data.tab[i].x*sin(v);
			printf("before: %f\n", bite_x);
			bite_x = bite_x*cos(-v) + bite_z*sin(-v);
			bite_z = bite_z*cos(-v) + bite_x*sin(-v);
			printf("after: %f\n", bite_x);
		}
		data.tab[i].x = data.tab[i].x*cos(v) + data.tab[i].z*sin(v);
		data.tab[i].z = data.tab[i].z*cos(v) + data.tab[i].x*sin(v);
		i++;
	}
	return (data);
}

t_mlx	rotate_z(t_mlx data, float v)
{
	int		i;

	i = 0;
	while (i < data.prop.size)
	{
		data.tab[i].x = data.tab[i].x*cos(v) - data.tab[i].y*sin(v);
		data.tab[i].y = data.tab[i].x*sin(v) + data.tab[i].y*cos(v);
		i++;
	}
	return (data);
}

t_mlx	rotate_tab(t_mlx data, char axe, float value)
{
	if (axe == 'x')
		data = rotate_x(data, value);
	else if (axe == 'y')
		data = rotate_y(data, value);
	else if (axe == 'z')
		data = rotate_z(data, value);
	return (data);
}

void	rotate(t_mlx *data, char arrow)
{
	if (arrow == 'u')
		*data = rotate_tab(*data, 'x', -ROTATION);
	else if (arrow == 'd')
		*data = rotate_tab(*data, 'x', ROTATION);
	else if (arrow == 'l')
		*data = rotate_tab(*data, 'z', -ROTATION);
	else if (arrow == 'r')
		*data = rotate_tab(*data, 'z', ROTATION);
}

void	display_tab(t_mlx *data)
{
	int		pos;

	pos = 0;
	data->tab = tab_iso(*data);
	ft_bzero(data->img.data, WIN_X * WIN_Y * data->img.bpp);
	draw_lines(*data);
	mlx_put_image_to_window(data->mlx, data->win, data->img.ptr, 0, 0);
}

int		ft_key(int key, t_mlx *data)
{
	printf("key: %d\n", key);
	if (key == ESC)
	{
		ft_putendl("ESC key pressed, exiting.");
		exit(1);
	}
	if (key == ROTATE_LEFT)
		rotate(data, 'l');
	else if (key == ROTATE_UP)
		rotate(data, 'u');
	else if (key == ROTATE_RIGHT)
		rotate(data, 'r');
	else if (key == ROTATE_DOWN)
		rotate(data, 'd');
	else if (key == ZOOM)
	{
		data->cam.zoom += 1;
		data->tab = tab_zoom(*data, data->cam.zoom);
	}
	else if (key == UNZOOM)
	{
		data->cam.zoom -= 1;
		data->tab = tab_zoom(*data, data->cam.zoom);
	}
	printf("END_ACTION\n");
	display_tab(data);
	return (0);
}

int		new_window(t_mlx *data)
{
	if (!(data->mlx = mlx_init()))
		return (0);
	data->win = mlx_new_window(data->mlx, WIN_X, WIN_Y, WIN_TITLE);
	data->img.ptr = mlx_new_image(data->mlx, WIN_X, WIN_Y);
	data->img.data = mlx_get_data_addr(data->img.ptr, &data->img.bpp,
	&data->img.size_line, &data->img.endian);
	data->img.bpp /= 8;
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
	zoom = (WIN_X / 2) / max;
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
		data.tab = tab_zoom(data, data.cam.zoom);
		display_tab(&data);
		mlx_key_hook(data.win, ft_key, &data);
		mlx_loop(data.mlx);
	}
	return (1);
}
