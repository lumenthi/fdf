/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 17:57:57 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/11 15:18:42 by lumenthi         ###   ########.fr       */
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

void	tab_zoom(t_mlx *data, int i)
{
	int zoom;

	zoom = data->cam.zoom;
	data->tab[i].x = (data->tab[i].b_x * zoom) + data->cam.start;
	data->tab[i].y = (data->tab[i].b_y * zoom) + data->cam.start;
	data->tab[i].z = (data->tab[i].b_z * data->cam.zoom_z) + data->cam.start;
}

void	add_pixel(char *image, int x, int y, int color, int bpp)
{
	if (x < 0 || x >= WIN_X || y < 0 || y >= WIN_Y)
		return ;
	*(int *)(image + ((x + y * WIN_X) * bpp)) = color;
}

void	just_draw(t_point tab0, t_point tab1, t_mlx data)
{
	t_draw d;
	d.x0 = tab0.u;
	d.x1 = tab1.u;
	d.y0 = tab0.v;
	d.y1 = tab1.v;
	d.dx = abs(d.x1-d.x0);
	d.dy = abs(d.y1-d.y0);
	d.sx = 0;
	d.sy = 0;
	d.err = 0;
	d.e2 = 0;
	if (d.x0 < d.x1)
		d.sx = 1;
	else
		d.sx = -1;
	if (d.y0 < d.y1)
		d.sy = 1;
	else 
		d.sy = -1;
	d.err = d.dx-d.dy;
	while (!(d.x0==d.x1 && d.y0==d.y1))
	{
		if (tab0.b_z > 0 || tab1.b_z > 0)
			add_pixel(data.img.data, d.x0, d.y0, 125, data.img.bpp);
		else
			add_pixel(data.img.data, d.x0, d.y0, 155, data.img.bpp);
		d.e2 = 2*d.err;
		if (d.e2 > -d.dy)
		{
			d.err = d.err - d.dy;
			d.x0 += d.sx;
		}
		if (d.e2 < d.dx)
		{
			d.err = d.err + d.dx;
			d.y0 += d.sy;
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
				just_draw(data.tab[pos], t_p, data);
		}
		if (data.tab[pos].b_x * data.cam.zoom < data.prop.max_x * data.cam.zoom)
		{
			t_p = data.tab[pos + 1];
			just_draw(data.tab[pos], t_p, data);
		}
		pos++;
	}
}

t_point	rotate_x(t_point tab, float v, t_point ref)
{
	if (v != 0)
	{
		tab.y = cos(v) * (tab.y - ref.y) - sin(v) * (tab.z - ref.z) + ref.y;
		tab.z = sin(v) * (tab.y - ref.y) + cos(v) * (tab.z - ref.z) + ref.z;
	}
	return (tab);
}

t_point	rotate_y(t_point tab, float v, t_point ref)
{
	if (v != 0)
	{
		tab.x = cos(v) * (tab.x - ref.x) + sin(v) * (tab.z - ref.z) + ref.x;
		tab.z = cos(v) * (tab.z - ref.z) - sin(v) * (tab.x - ref.x) + ref.z;
	}
	return (tab);
}

t_point		move_u(t_point tab, int pos)
{
	if (pos != 0)
		tab.u += pos;
	return (tab);
}

t_point		move_v(t_point tab, int pos)
{
	if (pos != 0)
		tab.v += pos;
	return (tab);
}

void	apply_modif(t_mlx *data)
{
	int i;
	t_point ref;

	i = 0;
	ref = data->tab[data->prop.size / 2];
	while (i < data->prop.size)
	{
		tab_zoom(data, i);
		data->tab[i] = rotate_x(data->tab[i], ROTATION * data->cam.x_rot, ref);
		data->tab[i] = rotate_y(data->tab[i], ROTATION * data->cam.y_rot, ref);
		data->tab[i] = to_isometric(data->tab[i]);
		data->tab[i] = move_u(data->tab[i], SPEED * data->cam.x_pos);
		data->tab[i] = move_v(data->tab[i], SPEED * data->cam.y_pos);
		i++;
	}
	printf("END_MODIFS\n");
}

void	display_tab(t_mlx *data)
{
	int		pos;

	pos = 0;
	ft_bzero(data->img.data, WIN_X * WIN_Y * data->img.bpp);
	apply_modif(data);
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
		data->cam.y_rot++;
	else if (key == ROTATE_UP)
		data->cam.x_rot--;
	else if (key == ROTATE_RIGHT)
		data->cam.y_rot--;
	else if (key == ROTATE_DOWN)
		data->cam.x_rot++;
	else if (key == LEFT)
		data->cam.x_pos--;
	else if (key == RIGHT)
		data->cam.x_pos++;
	else if (key == UP)
		data->cam.y_pos--;
	else if (key == DOWN)
		data->cam.y_pos++;
	else if (key == ZOOM)
		data->cam.zoom += 1;
	else if (key == UNZOOM)
		data->cam.zoom -= 1;
	else if (key == PERSP_P)
		data->cam.zoom_z += 1;
	else if (key == PERSP_M)
	{
		if (data->cam.zoom_z != 0)
			data->cam.zoom_z -= 1;
	}
	else if (key == RESET)
		data->cam.reset = 1;
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

t_mlx	get_zoom(t_mlx data)
{
	int		max;

	if (data.prop.max_x > data.prop.max_y)
		max = data.prop.max_x;
	else
		max = data.prop.max_y;
	data.cam.start = WIN_X / 6;
	data.cam.zoom = (WIN_X - data.cam.start) / max;
	data.cam.zoom_z = data.cam.zoom;
	data.cam.x_rot = 0;
	data.cam.y_rot = 0;
	data.cam.x_pos = 0;
	data.cam.y_pos = 0;
//	data.prop.max_x += data.cam.start;
//	data.prop.max_y += data.cam.start;
	return (data);
}

int		main(int argc, char **argv)
{
	t_mlx		data;

	if (argc == 2)
	{
		if (!reader(argv[1], &data))
			return (-1);
//		ft_printcoords(data);
		data = get_zoom(data);
		new_window(&data);
		display_tab(&data);
		mlx_key_hook(data.win, ft_key, &data);
		mlx_loop(data.mlx);
	}
	return (1);
}
