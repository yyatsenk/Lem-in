/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyatsenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/25 15:49:44 by yyatsenk          #+#    #+#             */
/*   Updated: 2018/02/25 15:49:50 by yyatsenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

int connection_valid_check(char *str)
{
    int i;

    i = -1;
    if (!str)
        return (0);
    if (!str[0])
        return (0);
    while(str[++i] && str[i] != '-')
    {
        if (!ft_isdigit(str[i]) && !ft_isalpha(str[i]))
            return (0);
    }
    if (str[i] == '\0')
        return (0);
    while(str[++i])
    {
        if (!ft_isdigit(str[i]) && !ft_isalpha(str[i]))
            return (0);
    }
    return (1);
}

int line_check(char *str)
{
    int i;

    i = -1;
    if (!ft_strcmp("##start", str))
        return (2);
    if (!ft_strcmp("##end", str))
        return (3);
    if (str[0] == 'L' || str[0] == '#')
        return (-1);
    while (str[++i] && str[i] != ' ')
    {
        if (!ft_isdigit(str[i]) && !ft_isalpha(str[i]))
            return (0);
    }
    if (!str[i])
        return (0);
    else
    {
        while (str[++i] && str[i] != ' ')
              if (!ft_isdigit(str[i]))
                 return (0);
        if (!str[i])
            return (0);
         while (str[++i])
              if (!ft_isdigit(str[i]))
                 return (0);
        return (1);
    }
}

void data_init(t_data *data)
{
    data->matrix = NULL;
	data->n_reb = 0;
    data->num_rooms = 0;
	data->start = NULL;
	data->end = NULL;
	data->num_of_ants = -1;
	data->queue = NULL;
}

t_room *add_back(t_room *room_to_end, t_room *begin)
{
    t_room *begin_copy;

    begin_copy = begin;
    while (begin->next)
        begin = begin->next;
    begin->next = room_to_end;
    return (begin_copy);
}

t_room *room_init(char *str, t_room *room_1)
{
    char **str_copy;
    t_room *room;

    room = (t_room*)malloc(sizeof(t_room));
    if (room_1)
    {
        while (room_1->next)
            room_1 = room_1->next;
        room->index = room_1->index + 1;
    }
    else
        room->index = 0;
    room->is_visited = 0;
    room->down = NULL;
    room->is_end = 0;
    room->is_start = 0;
    room->distance = 7777777;
    room->next = NULL;
    if (str && (line_check(str) == 1))
    {
        str_copy = ft_strsplit(str, ' ');
        room->room_name = str_copy[0];
        room->coords.x = ft_atoi(str_copy[1]);
        room->coords.y = ft_atoi(str_copy[2]);
    }
    else
    {
        str_copy = NULL;
        room->room_name = NULL;
        room->coords.x = -1;
        room->coords.y = -1;
    }
    return (room);
}

t_room *start_set(t_data *data, int fd, char *str, t_room *room)
{
    t_room *new;

    get_next_line(fd, &str);
    if (!line_check(str))
    {
        printf ("Not a valid line\n");
        exit(1);
    }
    new = room_init(str, room);
    new->is_start = 1;
    room = add_back(new, room);
    return (room);
    //free_2d(str_copy);
}

t_room *end_set(t_data *data, int fd,char *str, t_room *room)
{
     t_room *new;
    
    get_next_line(fd, &str);
    if (!line_check(str))
    {
        printf ("Not a valid line\n");
        exit(1);
    }
    new = room_init(str, room);
    new->is_end = 1;
    room = add_back(new, room);
    return (room);
    //free_2d(str_copy);
}

int my_list_size(t_room *room)
{
    int i;

    i = 0;
    while (room)
    {  
        i++;
        room = room->next;
    }
    return (i - 1);
}
t_rebro *add_rebro(t_rebro *rebro_to_end, t_rebro *begin)
{
    t_rebro *begin_copy;

    begin_copy = begin;
    if (!begin)
        begin_copy = rebro_to_end;
    else
    {
        while (begin->next)
            begin = begin->next;
        begin->next = rebro_to_end;
    }
    return (begin_copy);
}

t_rebro *rebro_init(int index)
{
    t_rebro *rebro;

    rebro = (t_rebro*)malloc(sizeof(t_rebro));
    rebro->index_of_elem = index;
    rebro->next = NULL;
    return (rebro);
}
t_queue *queue_init(int index)
{
    t_queue *queue;

    queue = (t_queue*)malloc(sizeof(t_queue));
    queue->next = NULL;
    queue->q_index = index;
    return (queue);
}

t_queue *add_que(t_queue *rebro_to_end, t_queue *begin)
{
    t_queue *begin_copy;

    begin_copy = begin;
    if (!begin)
        begin_copy = rebro_to_end;
    else
    {
        while (begin->next)
            begin = begin->next;
        begin->next = rebro_to_end;
    }
    return (begin_copy);
}

t_queue *add_que_first(t_queue *rebro_to_end, t_queue *begin)
{
   if (!begin)
        begin = rebro_to_end;
    else
    {
       rebro_to_end->next = begin;
       begin = rebro_to_end;
    }
    return (begin);
}

t_queue *del_que_first(t_queue *begin)
{
    t_queue *tmp;

    tmp = begin;
    if (begin)
        begin = begin->next;
    free(tmp);
    tmp = NULL;
    return (begin);
}

int check_queue(t_queue *queue, int index)
{
    if (!queue)
        return (0);
    while (queue)
    {
        if (queue->q_index == index)
            return (0);
        queue = queue->next;
    }
    return (1);
}

t_way_down *add_way_down(t_way_down *rebro_to_end, t_way_down *begin)
{
    t_way_down *begin_copy;

    begin_copy = begin;
    if (!begin)
        begin_copy = rebro_to_end;
    else
    {
        while (begin->next)
            begin = begin->next;
        begin->next = rebro_to_end;
    }
    return (begin_copy);
}

t_way_down *way_down_init(int index, int distance)
{
    t_way_down *way_down;

    way_down = (t_way_down*)malloc(sizeof(t_way_down));
    way_down->elem = index;
    way_down->distance = distance;
    way_down->next = NULL;
    return (way_down);
}

t_way_down *w_down_copy(t_way_down *list)
{
    t_way_down *way_down;

    way_down = NULL;
    while (list)
    {
       way_down = add_way_down(way_down_init(list->elem, 0),  way_down);
       list = list->next;
    }
    return (way_down);
}

int list_size_down(t_way_down *list)
{
    int i;

    i = 0;
    while (list)
    {
        list = list->next;
        i++;
    }
    return (i);
}
t_way *way_init(int index, int parent)
{
    t_way *way;

    way = (t_way*)malloc(sizeof(t_way));
    way->num_of_way = index;
    way->parent = parent;
    way->ignore = 0;
    way->weight = -1;
    way->next = NULL;
    way->down = NULL;
    return (way);
}

t_way *add_way(t_way *rebro_to_end, t_way *begin)
{
    t_way *begin_copy;

    begin_copy = begin;
    if (!begin)
        begin_copy = rebro_to_end;
    else
    {
        while (begin->next)
            begin = begin->next;
        begin->next = rebro_to_end;
    }
    return (begin_copy);
}

t_way *way_sort(t_way *way, t_room *room)
{
    t_way *begin;
    t_way_down *down_copy;

    begin = way;
    while (room->is_end != 1)
        room = room->next;
    while (way)
    {
        down_copy = way->down;
        while (way->down->next)
            way->down = way->down->next;
        if (way->down->elem != room->index)   
            way->ignore = 1;
        else
            way->weight = list_size_down(down_copy);
        way->down = down_copy;
        way = way->next;
    }
    return (begin);
}

int shortest(t_way *way)
{
    t_way *begin;
    int min;

    begin = way;
    min = 2147483647;
    while (way)
    {
        if (way->weight != -1 && way->weight < min)
            min = way->weight;
        way = way->next;
    }
    way = begin;
    return (min);
}

int main(void)
{
    char *str;
    char **str_sp;
    t_data data;
    t_room *room;
    t_rebro *rebro;
    t_queue *queue;
    t_room *room_copy;
    t_room *room_copy_2;
    t_way *way;
    int num_of_ants;
    int fd;
    int i;
    int res;

    i = -1;
  /*  way->down = (t_way_down*)malloc(sizeof(t_way_down));
    way->down->elem = 0;
    way->down->next = NULL;*/
    data_init(&data);
    fd = open("test", O_RDONLY);
    get_next_line(fd, &str);
//-----------------------------------------
    while(str[++i])
    {
        if (str[i] == '0')
            return (0);
        if (!ft_isdigit(str[i]))
        { 
            write(1, "error", 5);
            return (0);
        }
    }
    num_of_ants = ft_atoi(str);
    free(str);
    printf("%d\n", num_of_ants);
//-----------------------------------------
    room = room_init(str, NULL);
    get_next_line(fd, &str);
    while ((res = line_check(str)))
    {
        if (res == 2)
            room = start_set(&data, fd, str, room);
        else if (res == 3)
            room = end_set(&data, fd, str, room);
        else if (res == -1)
        {
           get_next_line(fd, &str);
           continue;         
        }
        else 
            room = add_back(room_init(str, room), room);
        free(str);
        get_next_line(fd, &str);
    }
   // data.num_rooms = my_list_size(room);
    //printf ("%d\n", data.num_rooms);
//-----------------------------------------
    //rebro = rebro_init(0);
   // printf ("%s\n", str);
    while ((res = connection_valid_check(str)))
    {
        room_copy = room->next;
        room_copy_2 = room->next;
        str_sp = ft_strsplit(str, '-');
        while (room_copy->next && ft_strcmp(room_copy->room_name, str_sp[0]))
            room_copy = room_copy->next;
        while (room_copy_2->next && ft_strcmp(room_copy_2->room_name, str_sp[1]))
            room_copy_2 = room_copy_2->next;
       // rebro = add_rebro(rebro_init(room_copy_2->index), rebro);
        room_copy->down = add_rebro(rebro_init(room_copy_2->index), room_copy->down);
        //printf ("%p\n", room_copy->down);
        get_next_line(fd, &str);
    }
//-----------------------------------------
    t_way *way_copy;
    //way = NULL;
    way = way_init(0, 0);
    way_copy = way;
    room_copy = room->next;
    room_copy_2 = room->next;
    while (room_copy->next && room_copy->is_start != 1)
         room_copy = room_copy->next;
    way->down = add_way_down(way_down_init(room_copy->index, 0),  way->down);
    room_copy->is_visited = 1;
    int j = -1;
    int count;
    t_way *way_rotate;
    t_way *way_copy_2;
    t_way_down *down_copy;
    count = 0;
   // way->down = way_down_init(room_copy->index, 0);
    way_copy = way;
    way_rotate = way;
    while (1)
    {   
        room_copy = room->next;
        if (++j != 0)
            way_rotate = way_rotate->next;
        if (!way_rotate)
            break ;
        down_copy = way_rotate->down;
        while (way_rotate->down->next)
            way_rotate->down = way_rotate->down->next;
        while (room_copy->index != way_rotate->down->elem)
            room_copy = room_copy->next;
        way_rotate->down = down_copy;
        while (room_copy->down)
        {          
            way = add_way(way_init(++count, way_rotate->num_of_way), way);
            way_copy_2 = way;
            while (way_copy_2->next)
                way_copy_2 = way_copy_2->next;
            way_copy = way;
            while (way_copy->num_of_way != way_copy_2->parent)
                way_copy = way_copy->next;
            way_copy_2->down = w_down_copy(way_copy->down);
            way_copy_2->down = add_way_down(way_down_init(room_copy->down->index_of_elem, (0)),  way_copy_2->down);
            room_copy->down = room_copy->down->next;
        }
        if (room_copy->is_end != 1)
            room_copy->is_visited = 1;
    }
//-----------------------------------------
    int min_way;
    int mini_ant;
    t_way_down *down;

    min_way = 0;
    mini_ant = -1;
    way = way_sort(way, room->next);
    way_copy = way;
    room_copy = room->next;
    while (room_copy)
    {
        room_copy->is_visited = 0;
        room_copy = room_copy->next;
    }

    while (mini_ant < num_of_ants)
    {
        way = way_copy;
        mini_ant++;
        if (mini_ant == num_of_ants)
            break ;
        min_way = shortest(way_copy);
        while (way->weight != min_way)
            way = way->next;
        way->weight++;
        down = way->down;
        while (way->down->next)
        {    
            room_copy = room->next;
            while (room_copy->index != way->down->next->elem)
                room_copy = room_copy->next;
            //if (room_copy->is_visited == 0)
                printf("L[%d]->%d ", mini_ant, way->down->next->elem);
            if (room_copy->is_end != 1)
                room_copy->is_visited = 1;
            way->down = way->down->next;
        }
        printf("\n");
        way->down = down;
    }
    printf("\n");
    while (way)
    { 
        printf ("Num_of_way %d (ignore = %d) (weight = %d)\n", way->num_of_way, way->ignore, way->weight);
        while (way->down)
        {
            printf ("(elem = %d)\n", way->down->elem);
            way->down = way->down->next;
        }
        way = way->next;
    }
    printf("\n");
   // queue = add_que(queue_init(room_copy->next->index), queue);
   
    /*while(room)
    {
        printf ("%10s %10d %10d (end = %3d) (start = %3d) (index = %d) (visited = %d) (p = %p)\n", room->room_name, room->coords.x,room->coords.y,
        room->is_end, room->is_start, room->index, room->is_visited, room->down);
        while(room->index != 0 && room->down)
        {   
            printf ("%d\n", room->down->index_of_elem);
            room->down = room->down->next;
        }
        room = room->next;
    }
    while(rebro)
    {
        printf ("%s - %s\n", rebro->con.elem_1, rebro->con.elem_2);
        rebro = rebro->next;
    }*/
   // fuckin_ants();
    close(fd);
    return (0);
}