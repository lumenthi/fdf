/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 17:57:57 by lumenthi          #+#    #+#             */
/*   Updated: 2018/07/03 19:04:05 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	apply_modif(t_mlx *d)
{
	int		i;
	t_point	ref;
	int		elem;

	i = 0;
	elem = ((d->prop.max_x + 1) * (d->prop.max_y / 2)) + d->prop.max_x / 2;
	tab_zoom(d, elem);
	ref = d->tab[elem];
	while (i < d->prop.size)
	{
		if (d->cam.reset == 1)
		{
			*d = get_zoom(*d);
			d->cam.reset = 0;
			tab_zoom(d, elem);
			ref = d->tab[elem];
		}
		if (i != elem)
			tab_zoom(d, i);
		d->tab[i] = to_isometric(d->tab[i], *d, ref);
		d->tab[i] = move_u(d->tab[i], SPEED * d->cam.x_pos);
		d->tab[i] = move_v(d->tab[i], SPEED * d->cam.y_pos);
		i++;
	}
}

void	display_tab(t_mlx *data)
{
	clear_screen(data);
	apply_modif(data);
	draw_lines(*data);
	mlx_put_image_to_window(data->mlx, data->win, data->img.ptr, 0, 0);
}

int		ft_key(int key, t_mlx *data)
{
	if (key == ESC)
	{
		ft_putendl("ESC key pressed, exiting.");
		free(data->tab);
		exit(1);
	}
	else if (actions(key, data) == 1)
		display_tab(data);
	return (0);
}

int		main(int argc, char **argv)
{
	t_mlx		data;

	if ((argc >= 2 && argc <= 5) && (get_colors(argc, argv, &data)))
	{
		if (!reader(argv[1], &data))
			return (-1);
		data = get_zoom(data);
		new_window(&data);
		display_tab(&data);
		mlx_key_hook(data.win, ft_key, &data);
		mlx_loop(data.mlx);
	}
	else
		usage();
	return (-1);
}
