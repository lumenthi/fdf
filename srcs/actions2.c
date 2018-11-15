/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 18:38:04 by lumenthi          #+#    #+#             */
/*   Updated: 2018/11/15 11:14:25 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

t_point	rotate_x(t_point tab, float v, t_point ref)
{
	int	y;

	y = tab.y;
	if (v != 0)
	{
		tab.y = cos(v) * (tab.y - ref.y) - sin(v) * (tab.z - ref.z) + ref.y;
		tab.z = sin(v) * (y - ref.y) + cos(v) * (tab.z - ref.z) + ref.z;
	}
	return (tab);
}

t_point	rotate_y(t_point tab, float v, t_point ref)
{
	int	x;

	x = tab.x;
	if (v != 0)
	{
		tab.x = cos(v) * (tab.x - ref.x) + sin(v) * (tab.z - ref.z) + ref.x;
		tab.z = cos(v) * (tab.z - ref.z) - sin(v) * (x - ref.x) + ref.z;
	}
	return (tab);
}

t_point	to_isometric(t_point tab, t_mlx data, t_point ref)
{
	tab = rotate_x(tab, ROTATION * data.cam.x_rot, ref);
	tab = rotate_y(tab, ROTATION * data.cam.y_rot, ref);
	tab.u = (tab.x + tab.z) / sqrt(2);
	tab.v = (tab.x + 2 * tab.y - tab.z) / sqrt(6);
	return (tab);
}

void	tab_zoom(t_mlx *data, int i)
{
	int zoom;

	zoom = data->cam.zoom;
	data->tab[i].x = (data->tab[i].b_x * zoom) + data->cam.start;
	data->tab[i].y = (data->tab[i].b_y * zoom) + data->cam.start;
	data->tab[i].z = (data->tab[i].b_z * data->cam.zoom_z);
}

t_mlx	get_zoom(t_mlx data)
{
	int		max;

	if (data.prop.max_x > data.prop.max_y)
		max = data.prop.max_x;
	else
		max = data.prop.max_y;
	if (max == 0)
		max = 1;
	data.cam.start = WIN_X / 6;
	if (max > WIN_X)
		data.cam.zoom = 1;
	else
		data.cam.zoom = (WIN_X) / max;
	data.cam.zoom_z = 1;
	data.cam.x_rot = 0;
	data.cam.y_rot = 0;
	data.cam.x_pos = 0;
	data.cam.y_pos = 0;
	return (data);
}
