/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozlov <ikozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 20:04:31 by ikozlov           #+#    #+#             */
/*   Updated: 2018/04/13 22:51:36 by ikozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <mlx.h>
#include <stdio.h>

void		iterate_points(t_mlx *mlx, void (*f)(t_mlx *, int, int))
{
	int		i;
	int		size;
	t_map	*map;

	i = -1;
	map = mlx->map;
	size = map->height * map->width;
	while (++i < size)
		f(mlx, i % map->width, i / map->width);
}

void		draw_line(t_mlx *mlx, t_bshm_line line)
{
	int		x;
	int		y;
	int		a;
	int		b;
	int		i;

	x = (int)line.p1.x;
	y = (int)line.p1.y;
	a = 2 * line.dy;
	b = a - 2 * line.dx;
	i = -1;
	while (++i < line.dx)
	{
		if (line.er < 0)
		{
			line.change == 1 ? (y += line.sy) : (x += line.sx);
			line.er += a;
		}
		else
		{
			y += line.sy;
			x += line.sx;
			line.er += b;
		}
	}
}

void		draw_lines(t_mlx *mlx, int x, int y)
{
	t_point3d	p;
	t_point3d	p2;

	p = point_project(GET_POINT(mlx->map, x, y), mlx);
	if (x + 1 < mlx->map->width)
	{
		p2 = point_project(GET_POINT(mlx->map, x + 1, y), mlx);
		draw_line(mlx, line_init(p, p2));
	}
	if (y + 1 < mlx->map->height)
	{
		p2 = point_project(GET_POINT(mlx->map, x, y + 1), mlx);
		draw_line(mlx, line_init(p, p2));
	}
}

void		light_points(t_mlx *mlx, int x, int y)
{
	t_point3d	p;

	p = GET_POINT(mlx->map, x, y);
	p = point_project(p, mlx);
	if (p.x > 0 && p.x <= MIN_WIDTH && p.y >= 0 && p.y <= MIN_HEIGHT)
		*(int *)(mlx->image->ptr +
				((int)p.x + (int)p.y * MIN_WIDTH) * mlx->image->bpp) = 0x00ff00;
}

void		render(t_mlx *mlx)
{
	iterate_points(mlx, draw_lines);
	iterate_points(mlx, light_points);
	mlx_put_image_to_window(mlx->mlx, mlx->window, mlx->image->image, 0, 0);
}
