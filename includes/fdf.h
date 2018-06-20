/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 17:58:34 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/20 18:47:26 by lumenthi         ###   ########.fr       */
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

# define DFLT 0xffffff

# define RESET 36

# define PERSP_P 6
# define PERSP_M 7

# define ZOOM 14
# define UNZOOM 12

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
	int				base_col;
	int				persp_col;
	int				high_col;
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

typedef struct		s_nrm
{
	int				x;
	int				y;
	int				size;
	int				in;

}					t_nrm;

typedef struct		s_mlx
{
	void			*mlx;
	void			*win;
	t_point			*tab;
	t_cam			cam;
	t_image			img;
	t_prop			prop;
}					t_mlx;

int					hex_to_int(char *hexVal);
void				usage(void);
void				clear_screen(t_mlx *data);
void				error_msg(void);
void				add_pixel(t_mlx d, int x, int y, int color);
int					fill_value(t_mlx *data, char *str, t_nrm n);
void				get_tab_init(t_mlx *data, int tab_size, t_nrm *n);
int					reader(char *filename, t_mlx *data);
int					actions(int key, t_mlx *data);
t_point				move_u(t_point tab, int pos);
t_point				move_v(t_point tab, int pos);
t_mlx				get_zoom(t_mlx data);
void				tab_zoom(t_mlx *data, int i);
t_point				to_isometric(t_point tab, t_mlx data, t_point ref);
t_point				rotate_y(t_point tab, float v, t_point ref);
t_point				rotate_x(t_point tab, float v, t_point ref);
int					get_colors(int argc, char **argv, t_mlx *data);
int					new_window(t_mlx *data);
int					is_hex(char *file);
void				draw_lines(t_mlx data);

#endif
