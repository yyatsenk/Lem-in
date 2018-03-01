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

#ifndef LEMIN_H
# define LEMIN_H

# include <unistd.h>
# include "libft/libft.h"
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_a
{
	char *elem_1;
	char *elem_2;
}				t_connect;

typedef struct s_b
{
	char **matrix;
	int n_reb;
	int num_rooms;
	char *start;
	char *end;
	int num_of_ants;
	int *queue;
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
#endif




