/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 17:58:34 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/15 18:42:29 by lumenthi         ###   ########.fr       */
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

# define ROTATE_LEFT 0
# define ROTATE_UP 13
# define ROTATE_RIGHT 2
# define ROTATE_DOWN 1

# define SPEED 20

# define LEFT 123
# define UP 126
# define RIGHT 124
# define DOWN 125

# define DFLT_BASE 0xffffff
# define DFLT_PERSP 0x9e001f

# define RESET 36

# define PERSP_P 6
# define PERSP_M 7

# define ZOOM 4
# define UNZOOM 5

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
	int				color;
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
	int				start;
	int				zoom;
	int				zoom_z;
	int				x_rot;
	int				y_rot;
	int				x_pos;
	int				y_pos;
	int				reset;
}					t_cam;

typedef struct		s_prop
{
	int				max_x;
	int				max_y;
	int				max_z;
	int				size;
}					t_prop;

typedef struct		s_draw
{
	int				x0;
	int				y0;
	int				x1;
	int				y1;
	int				dx;
	int				dy;
	int				sx;
	int				sy;
	int				err;
	int				e2;
}					t_draw;

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
