/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 18:11:26 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/20 18:13:02 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	values_assign(t_mlx *data, char *str, t_nrm n)
{
	data->tab[n.size].x = n.x;
	data->tab[n.size].y = n.y;
	data->tab[n.size].z = ft_atoi(str);
	data->tab[n.size].b_x = n.x;
	data->tab[n.size].b_y = n.y;
	data->tab[n.size].b_z = data->tab[n.size].z;
	if (n.x > data->prop.max_x)
		data->prop.max_x = n.x;
	if (n.y > data->prop.max_y)
		data->prop.max_y = n.y;
	if (data->tab[n.size].z > data->prop.max_z)
		data->prop.max_z = data->tab[n.size].z;
}

int		invalid_value(char *str)
{
	int i;

	i = 0;
	while (str[i] && str[i] != ',')
	{
		if (ft_strchr_index("-0123456789", str[i]) == -1)
		{
			ft_putstr_fd("fdf: invalid value ", 2);
			ft_putendl_fd(str, 2);
			return (1);
		}
		i++;
	}
	if (i == 0)
	{
		ft_putstr_fd("fdf: invalid_value\n", 2);
		return (1);
	}
	return (0);
}


int		fill_value(t_mlx *data, char *str, t_nrm n)
{
	char	*found;

	if (invalid_value(str))
		return (0);
	values_assign(data, str, n);
	if ((found = ft_strchr(str, ',')))
	{
		if (ft_strlen(found) > 1)
		{
			found++;
			data->tab[n.size].color = hex_to_int(found);
			if (data->tab[n.size].color == 0)
			{
				if (data->tab[n.size].z > 0)
					data->tab[n.size].color = data->prop.high_col;
				else
					data->tab[n.size].color = data->prop.base_col;
			}
		}
	}
	else
		data->tab[n.size].color = 0;
	return (1);
}

void	get_tab_init(t_mlx *data, int tab_size, t_nrm *n)
{
	n->size = 0;
	n->x = 0;
	n->y = 0;
	n->in = 0;
	data->prop.max_x = 0;
	data->prop.max_y = 0;
	data->prop.max_z = 0;
	if (!(data->tab = (t_point *)malloc(sizeof(t_point) * tab_size)))
	{
		ft_putstr_fd("Malloc error\n", 2);
		exit(-1);
	}
}
