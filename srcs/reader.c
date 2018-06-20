/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 18:09:32 by lumenthi          #+#    #+#             */
/*   Updated: 2018/06/20 18:11:07 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	fill_tab(t_mlx *data, char **line, t_nrm *n, char *buf)
{
	if (n->in == 1)
	{
		if (!fill_value(data, *line, *n))
		{
			free(data->tab);
			free(*line);
			exit(-1);
		}
		free(*line);
		n->in = 0;
		*line = ft_strdup("");
		n->x++;
		n->size++;
	}
	if (buf[0] == '\n')
	{
		n->x = 0;
		n->y++;
	}
}

void	get_tab(int fd, t_mlx *data, int tab_size)
{
	char	*line;
	t_nrm	n;
	char	buf[2];

	line = ft_strdup("");
	get_tab_init(data, tab_size, &n);
	while ((read(fd, buf, 1)))
	{
		buf[1] = '\0';
		if (buf[0] != ' ' && buf[0] != '\n')
		{
			line = ft_strjoinl(line, buf);
			n.in = 1;
		}
		else
			fill_tab(data, &line, &n, buf);
	}
	free(line);
	data->prop.size = n.size;
}

void	get_size_open(int *fd, char *filename)
{
	if ((*fd = open(filename, O_RDONLY)) == -1)
	{
		ft_putendl_fd("fdf: open error\n", 2);
		exit(-1);
	}
}

int		get_size(char *filename)
{
	int		size;
	int		fd;
	char	buf[1];
	int		in;
	int		ret;

	size = 0;
	ret = 0;
	in = 0;
	get_size_open(&fd, filename);
	while ((ret = read(fd, buf, 1)) > 0)
	{
		if (buf[0] == ' ' || buf[0] == '\n')
		{
			if (in)
				size++;
		}
		else
			in = 1;
	}
	close(fd);
	return (size);
}

int		reader(char *filename, t_mlx *data)
{
	int		fd;
	int		size;

	if ((fd = open(filename, O_RDONLY)) == -1)
		error_msg();
	else
	{
		size = get_size(filename);
		if (size <= 0)
			error_msg();
		get_tab(fd, data, size);
	}
	return (1);
}
