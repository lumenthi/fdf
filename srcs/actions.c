/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 18:21:20 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/20 18:37:11 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

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

int		increment(int *i)
{
	*i = 1;
	return (1);
}

void	actions2(int key, t_mlx *data, int *i)
{
	if (key == ZOOM && increment(i))
		data->cam.zoom += 1;
	else if (key == UNZOOM && increment(i))
	{
		if (data->cam.zoom != 1)
			data->cam.zoom -= 1;
	}
	else if (key == PERSP_P && increment(i))
		data->cam.zoom_z += 1;
	else if (key == PERSP_M && increment(i))
		data->cam.zoom_z -= 1;
	else if (key == RESET && increment(i))
		data->cam.reset = 1;
}

int		actions(int key, t_mlx *data)
{
	int i;

	i = 0;
	if (key == ROTATE_LEFT && increment(&i))
		data->cam.y_rot++;
	else if (key == ROTATE_UP && increment(&i))
		data->cam.x_rot--;
	else if (key == ROTATE_RIGHT && increment(&i))
		data->cam.y_rot--;
	else if (key == ROTATE_DOWN && increment(&i))
		data->cam.x_rot++;
	else if (key == LEFT && increment(&i))
		data->cam.x_pos++;
	else if (key == RIGHT && increment(&i))
		data->cam.x_pos--;
	else if (key == UP && increment(&i))
		data->cam.y_pos++;
	else if (key == DOWN && increment(&i))
		data->cam.y_pos--;
	else
		actions2(key, data, &i);
	return (i);
}
