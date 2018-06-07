/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 17:58:34 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/07 19:10:21 by lumenthi         ###   ########.fr       */
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

# define ROTATE_LEFT 123
# define ROTATE_UP 126
# define ROTATE_RIGHT 124
# define ROTATE_DOWN 125
# define ZOOM 69
# define UNZOOM 78
# define ESC 53

# define WIN_X 700
# define WIN_Y 700
# define ROTATION 0.174533
# define WIN_TITLE "fdf"

typedef struct		s_point
{
	int				x;
	int				b_x;
	int				y;
	int				b_y;
	int				u;
	int				v;
	int				z;
	int				b_z;
}					t_point;

typedef struct		s_image
{
	int				bpp;
	int				size_line;
	int				endian;
	void			*ptr;
	char			*data;
}					t_image;

typedef struct		s_cam
{
	int				zoom;
	int				zoomed;
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
	t_image			img;
	t_prop			prop;
}					t_mlx;

#endif
