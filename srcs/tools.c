/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 17:56:39 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/20 18:06:59 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

int		hex_to_int(char *hexVal)
{
	int	base;
	int	ret;
	int	i;

	ret = 0;
	base = 1;
	i = ft_strlen(hexVal) - 1;
	while (i >= 0)
	{
		if (hexVal[i] >= '0' && hexVal[i] <= '9')
			ret += (hexVal[i] - 48) * base;
		else if (hexVal[i] >= 'a' && hexVal[i] <= 'f')
			ret += (hexVal[i] - 87) * base;
		else if (hexVal[i] >= 'A' && hexVal[i] <= 'F')
			ret += (hexVal[i] - 55) * base;
		base = base * 16;
		i--;
	}
	return (ret);
}

void	clear_screen(t_mlx *data)
{
	ft_bzero(data->img.data, WIN_X * WIN_Y * data->img.bpp);
}

void	usage(void)
{
	ft_putendl_fd("fdf: invalid usage", 2);
	ft_putendl_fd("usage: fdf [file] [color1] [color2] [color3]", 2);
}

void	add_pixel(t_mlx d, int x, int y, int color)
{
	if (x < 0 || x >= WIN_X || y < 0 || y >= WIN_Y)
		;
	else
		*(int *)(d.img.data + ((x + y * WIN_X) * d.img.bpp)) = color;
}

void	error_msg(void)
{
	ft_putstr_fd("fdf: invalid file\n", 2);
	exit(0);
}
