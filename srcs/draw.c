/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 18:45:21 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/20 18:47:46 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static t_draw	draw_init(t_point tab0, t_point tab1)
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
	return (d);
}

static int		draw_color(t_point tab0, t_point tab1, t_mlx data)
{
	int	color;

	if (tab0.b_z == data.prop.max_z && tab1.b_z == data.prop.max_z)
		color = mlx_get_color_value(data.mlx, data.prop.high_col);
	else if (tab1.b_z > 0 || tab0.b_z > 0)
	{
		if (tab1.color != 0)
			color = mlx_get_color_value(data.mlx, tab1.color);
		else
			color = mlx_get_color_value(data.mlx, data.prop.persp_col);
	}
	else if (tab0.b_z == data.prop.max_z && tab1.b_z == data.prop.max_z)
		color = mlx_get_color_value(data.mlx, data.prop.high_col);
	else
	{
		if (tab1.color != 0)
			color = mlx_get_color_value(data.mlx, tab1.color);
		else
			color = mlx_get_color_value(data.mlx, data.prop.base_col);
	}
	return (color);
}

static void	just_draw(t_point tab0, t_point tab1, t_mlx data)
{
	t_draw	d;
	int		color;

	d = draw_init(tab0, tab1);
	while (!(d.x0 == d.x1 && d.y0 == d.y1))
	{
		color = draw_color(tab0, tab1, data);
		add_pixel(data, d.x0, d.y0, color);
		d.e2 = 2 * d.err;
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
		if (data.tab[pos].b_y < data.prop.max_y)
		{
			t_p = data.tab[pos + data.prop.max_x + 1];
			if (data.tab[pos].b_x == t_p.b_x)
				just_draw(data.tab[pos], t_p, data);
		}
		if (data.tab[pos].b_x < data.prop.max_x)
		{
			t_p = data.tab[pos + 1];
			just_draw(data.tab[pos], t_p, data);
		}
		pos++;
	}
}
