/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 17:58:34 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/03 12:24:08 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "../libft/libft.h"
# include "../minilibx/mlx.h"

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>

# define WIN_X 1000
# define WIN_Y 1000
# define WIN_TITLE "fdf"
# define ZOOM 50

typedef struct		s_point
{
	int				x;
	int				y;
	int				z;
	int				end;
}					t_point;

typedef struct		s_mlx
{
	void			*mlx;
	void			*win;
}					t_mlx;

#endif
