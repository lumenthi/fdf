/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 18:42:31 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/20 18:43:03 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

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

int		is_hex(char *file)
{
	int	i;

	i = 0;
	while (file[i])
	{
		if ((ft_strchr_index("1234567890abcdefABCDEF", file[i])) == -1)
		{
			ft_putstr_fd("Invalid color: ", 2);
			ft_putendl_fd(file, 2);
			return (0);
		}
		i++;
	}
	return (1);
}

void	set_dflt(t_mlx *data)
{
	if (data->prop.high_col == 0)
		data->prop.high_col = DFLT;
	if (data->prop.persp_col == 0)
		data->prop.persp_col = DFLT;
	if (data->prop.base_col == 0)
		data->prop.base_col = DFLT;
}

int		get_colors(int argc, char **argv, t_mlx *data)
{
	int i;

	i = 2;
	while (i < argc)
	{
		if (is_hex(argv[i]))
		{
			if (i == 2)
				data->prop.base_col = hex_to_int(argv[i]);
			else if (i == 3)
				data->prop.persp_col = hex_to_int(argv[i]);
			else if (i == 4)
				data->prop.high_col = hex_to_int(argv[i]);
		}
		else
			return (0);
		i++;
	}
	set_dflt(data);
	return (1);
}
