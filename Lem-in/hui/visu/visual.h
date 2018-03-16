/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyatsenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/25 15:56:34 by yyatsenk          #+#    #+#             */
/*   Updated: 2018/02/25 15:56:35 by yyatsenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VISUAL_H
# define VISUAL_H

# include <unistd.h>
# include "./libft/libft.h"
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_b
{
	char *word;
	struct s_b *next;
}				t_data;

typedef struct s_c
{
	int index_of_elem;
	struct s_c *next;
}				t_rebro;

typedef struct s_d
{
	int x;
	int y;
}				t_coords;

typedef struct s_e
{
	char *room_name;
	int index;
	t_coords coords;
	int distance;
	int is_start;
	int is_end;
	int is_visited;
	t_rebro *down;
	struct s_e *next;
}				t_room;
typedef struct s_f
{
	int q_index;
	struct s_f *next;
}				t_queue;

typedef struct s_g
{
	int elem;
	int distance;
	struct s_g *next;
	struct s_g *prev;
}				t_way_down;

typedef struct s_i
{
	int num_of_way;
	int parent;
	int weight;
	int ignore;
	struct s_i *next;
	t_way_down *down;
}				t_way;
typedef struct s_ant
{
	int num_of_ant;
	t_way_down *way;
	struct s_ant *next;
}				t_ant;
typedef struct s_all
{
	int got_end;
	int num_of_ants;
	int kof;
	t_ant *ant;
	t_ant *ant_copy;
	t_room *room;
	t_room *room_copy;
	t_room *room_copy_3;
	t_room *now_room;
	t_room *prev;
	int teplo;
	void *mlx_ptr;
	void *win_ptr;
}				t_p;
#endif




