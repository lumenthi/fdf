/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 17:58:34 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/06 11:15:35 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "../libft/libft.h"
# include "../minilibx/mlx.h"

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <math.h>

# define WIN_X 700
# define WIN_Y 700
# define WIN_TITLE "fdf"

typedef struct		s_point
{
	int				x;
	int				y;
	int				u;
	int				v;
	int				z;
}					t_point;

typedef struct		s_cam
{
	int				zoom;
}					t_cam;

typedef struct		s_prop
{
	int				max_x;
	int				max_y;
	int				max_z;
	int				size;
}					t_prop;

typedef struct		s_mlx
{
	void			*mlx;
	void			*win;
	t_point			*tab;
	t_cam			cam;
	t_prop			prop;
}					t_mlx;

#endif
