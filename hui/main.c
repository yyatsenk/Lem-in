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

int check_ignore(t_way *way)
{
    int is_way;
    
    is_way = 0;
    while(way)
    {
        if (way->ignore == 0)
            is_way++;
        way = way->next;
    }
    if (is_way == 0)
        return (0);
    else
        return (1);
}

int check_end_start(t_room *room)
{
    t_room *begin;
    int end;
    int start;

    end = 0;
    start = 0;
    begin  = room;
    while (room)
    {
        if (room->is_start)
            start++;
        if (room->is_end)
            end++;
        room = room->next;
    }
    if (start != 1 || end != 1)
        return (1);
    else
        return (0);
}

int connection_valid_check(char *str)
{
    int i;

    i = -1;
    if (!str)
        return (0);
    if (!str[0] || str[0] == '\n')
        return (3);
    if (!ft_strncmp("#", str, 1))
        return (2);
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
    if (ft_strcmp("##start", str) && ft_strcmp("##end", str) && !ft_strncmp("##", str, 2))
        return (-1);
    if (!ft_strcmp("##start", str))
        return (2);
    if (!ft_strcmp("##end", str))
        return (3);
    if (str[0] == 'L' || (str[0] == '#' && str[1] != '#'))
        return (-1);
    while (str[++i] && str[i] != ' ')
    {
        if (!ft_isdigit(str[i]) && !ft_isalpha(str[i]))
            return (0);
    }
    while (str[i] && str[i] == ' ')
        i++;
    if (!str[i])
        return (0);
    else
    {
        i--;
        while (str[++i] && str[i] != ' ')
              if (!ft_isdigit(str[i]))
                 return (0);
        while (str[i] && str[i] == ' ')
            i++;
        if (!str[i])
            return (0);
        i--;
        while (str[++i])
              if (!ft_isdigit(str[i]))
                 return (0);
        return (1);
    }
}

t_data *data_init(char *word)
{
    t_data *data;

    data = (t_data *)malloc(sizeof(t_data));
	data->word = word;
    data->next = NULL;
    return (data);
}

t_data *add_data(t_data *data, t_data *to_add)
{
    t_data *copy;

    copy = data;
    while (data->next)
        data = data->next;
    data->next = to_add;
    return (copy);
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
        //free_2d(str_copy);
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

t_room *start_set(t_data **data, int fd, char *str, t_room *room)
{
    t_room *new;

    get_next_line(fd, &str);
    *data = add_data(*data, data_init(ft_strdup(str)));
    while (str && str[0] == '#')
    {
        free(str);
        get_next_line(fd, &str);
        *data = add_data(*data, data_init(ft_strdup(str)));
    }
    if (!line_check(str))
    {
        printf ("ERROR\n");
        exit(1);
    }
    new = room_init(str, room);
    new->is_start = 1;
    free(str);
    room = add_back(new, room);
    return (room);
}

t_room *end_set(t_data **data, int fd,char *str, t_room *room)
{
    t_room *new;
    
    get_next_line(fd, &str);
    *data = add_data(*data, data_init(ft_strdup(str)));
    while (str && str[0] == '#')
    {
        free(str);
        get_next_line(fd, &str);
        *data = add_data(*data, data_init(ft_strdup(str)));
    }
    if (!line_check(str))
    {
        printf ("ERROR\n");
        exit(1);
    }
    new = room_init(str, room);
    new->is_end = 1;
    free(str);
    room = add_back(new, room);
    return (room);
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
        rebro_to_end->prev = begin;
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
    way_down->prev = NULL;
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
        while (way->down && way->down->next)
            way->down = way->down->next;
        if (!way->down || way->down->elem != room->index)   
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

t_ant *ant_add(t_ant *rebro_to_end, t_ant *begin)
{
    t_ant *begin_copy;

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

t_ant *ant_init(int num_of_ant)
{
    t_ant *ant;

    ant = (t_ant*)malloc(sizeof(t_ant));
    ant->num_of_ant = num_of_ant;
    ant->next = NULL;
    ant->way = NULL;
    return (ant);
}

void room_free(t_room *room)
{
    t_room *room_copy;

    room_copy = room;
    while (room_copy)
    {
        if (room_copy->down)
            free(room_copy->down);
        free(room_copy->room_name);
        room_copy = room_copy->next;
    }
    free(room);
}
void way_free(t_way *way)
{
    t_way *way_copy;

    way_copy = way;
    while (way_copy)
    {
        if (way_copy->down)
            free(way_copy->down);
        way_copy = way_copy->next;
    }
    free(way);
}

void data_free(t_data *data)
{
   t_data *data_copy;

    data_copy = data;
    while (data_copy)
    {
        if (data_copy->word)
            free(data_copy->word);
        data_copy = data_copy->next;
    }
    free(data);
}

int main(void)
{
    char *str;
    char **str_sp;
    t_data *data;
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
    data = data_init(NULL);
    str = NULL;
    //fd = open("test", O_RDONLY);
    fd = 0;
    i = get_next_line(fd, &str);
    data = add_data(data, data_init(ft_strdup(str)));
    if (!str || !str[0] || i <= 0)
    {
        write(1, "ERROR2\n", 7);
        return (0);
    }
    i = -1;
    while(str && str[0]== '#')
    {
        free(str);
        get_next_line(fd, &str);
        data = add_data(data, data_init(ft_strdup(str)));
    }
//-----------------------------------------
    if (!str || !str[0] || !ft_isdigit(str[0]))
    {
        write(1, "ERROR2\n", 7);
        free(str);
        return (0);
    }
    while(str[++i])
    {
        if (!ft_isdigit(str[i]) || str[0] == '0')
        { 
            write(1, "ERROR2\n", 7);
            free(str);
            return (0);
        }
    }
    num_of_ants = ft_atoi(str);
    free(str);
   //printf("%d\n", num_of_ants);
//-----------------------------------------
    room = room_init(str, NULL);
    get_next_line(fd, &str);
    data = add_data(data, data_init(ft_strdup(str)));
    while ((res = line_check(str)))
        {
        if (res == 2)
            room = start_set(&data, fd, str, room);
        else if (res == 3)
            room = end_set(&data, fd, str, room);
        else if (res == -1)
        {
           get_next_line(fd, &str);
           data = add_data(data, data_init(ft_strdup(str)));
           continue;         
        }
        else 
            room = add_back(room_init(str, room), room);
        free(str);
        get_next_line(fd, &str);
        data = add_data(data, data_init(ft_strdup(str)));
    }
//-----------------------------------------
    t_rebro *reb_copy;
    if (str && !str[0])
    { 
        write(1, "ERROR3\n", 7);
        return (0);
    }
    while ((res = connection_valid_check(str)))
    {
        if (res == 3)
            break;
        if (res != 1)
        {
            free(str);
            get_next_line(fd, &str);
            data = add_data(data, data_init(ft_strdup(str)));
            continue ;
        }
        room_copy = room->next;
        room_copy_2 = room->next;
        if (!room->next && (res = 3))
            break ;
        str_sp = ft_strsplit(str, '-');
        while (room_copy->next && ft_strcmp(room_copy->room_name, str_sp[0]))
            room_copy = room_copy->next;
        while (room_copy_2->next && ft_strcmp(room_copy_2->room_name, str_sp[1]))
            room_copy_2 = room_copy_2->next;
        if (room_copy->is_end != 1)
            room_copy->down = add_rebro(rebro_init(room_copy_2->index), room_copy->down);
        else
            room_copy_2->down = add_rebro(rebro_init(room_copy->index), room_copy_2->down);
        room_copy = room->next;
        room_copy_2 = room->next;
        while (room_copy->next && ft_strcmp(room_copy->room_name, str_sp[0]))
            room_copy = room_copy->next;
        while (room_copy_2->next && ft_strcmp(room_copy_2->room_name, str_sp[1]))
            room_copy_2 = room_copy_2->next;
        if ((ft_strcmp(room_copy->room_name, str_sp[0]) 
        || ft_strcmp(room_copy_2->room_name, str_sp[1])))
        {
            write(1, "ERROR3\n", 7);
            exit(1);
        }
        if (room_copy->is_end != 1)
           room_copy_2->down = add_rebro(rebro_init(room_copy->index), room_copy_2->down);
        else
            room_copy->down = add_rebro(rebro_init(room_copy_2->index), room_copy->down);
        free(str);
        get_next_line(fd, &str);
        data = add_data(data, data_init(ft_strdup(str)));
    }
    if (res != 3 || (res = check_end_start(room->next)) || num_of_ants < 0)
    { 
        write(1, "ERROR3\n", 7);
        return (0);
    }
   /* room_copy = room->next;
    while(room_copy)
    {
        printf ("%10s %10d %10d (end = %3d) (start = %3d) (index = %d) (visited = %d) (p = %p)\n", room_copy->room_name, room_copy->coords.x,room_copy->coords.y,
        room_copy->is_end, room_copy->is_start, room_copy->index, room_copy->is_visited, room_copy->down);
        while(room_copy->index != 0 && room_copy->down)
        {   
            printf ("%d\n", room_copy->down->index_of_elem);
            room_copy->down = room_copy->down->next;
        }
        room_copy = room_copy->next;
    }
    printf ("aaaa\n\n\n\n");*/
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
    t_rebro *r_copy_down;
    //t_room *room_copy_2;
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
        r_copy_down = room_copy->down;
        while (room_copy->down)
        {   
            room_copy_2 = room->next;
            while (room_copy_2->index != room_copy->down->index_of_elem)
                room_copy_2 = room_copy_2->next;
            if (room_copy_2->is_visited)
            {
                room_copy->down = room_copy->down->next;
                continue ;
            }    
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
        room_copy->down = r_copy_down;
        if (room_copy->is_end != 1)
            room_copy->is_visited = 1;
    }
    room_copy = room->next;
    /*while(room_copy)
    {
        printf ("%10s %10d %10d (end = %3d) (start = %3d) (index = %d) (visited = %d) (p = %p)\n", room_copy->room_name, room_copy->coords.x,room_copy->coords.y,
        room_copy->is_end, room_copy->is_start, room_copy->index, room_copy->is_visited, room_copy->down);
        while(room_copy->index != 0 && room_copy->down)
        {   
            printf ("%d\n", room_copy->down->index_of_elem);
            room_copy->down = room_copy->down->next;
        }
        room_copy = room_copy->next;
    }
    printf ("aaaa\n\n\n\n");*/
    /*data = data->next;
    while(data)
    {
        printf("%s\n",data->word);
        data = data->next;
    }*/
//-----------------------------------------
    int min_way;
    int mini_ant;
    t_way_down *down;
    t_ant *ant;
    t_ant *ant_copy;
    t_room *now_room;
    int got_end;
    t_room *room_copy_3;

    min_way = 0;
    got_end = 0;
    mini_ant = 0;
    ant = NULL;
    t_way_down *way_down_copy;
    way_copy = way;
    /*while (way_copy)
    { 
        //if (way_copy->ignore == 0)
        printf ("Num_of_way %d (ignore = %d) (weight = %d)\n", way_copy->num_of_way, way_copy->ignore, way_copy->weight);
        way_down_copy = way_copy->down;
        while (way_copy->down)
        {
            room_copy = room->next;
            while (room_copy->index != way_copy->down->elem)
                room_copy = room_copy->next;
            printf ("(elem = %s)\n", room_copy->room_name);
            way_copy->down = way_copy->down->next;
        }
         way_copy->down = way_down_copy;
        way_copy = way_copy->next;
    }
    printf("hui\n");*/
    way = way_sort(way, room->next);
    if (!check_ignore(way))
    {
        write(1, "ERROR3\n", 7);
        return (0);
    }
    way_copy = way;
    room_copy = room->next;
    room_copy = room->next;
    while (room_copy)
    {
        room_copy->is_visited = 0;
        room_copy = room_copy->next;
    }
    room_copy = room->next;
    while (room_copy->is_start != 1)
        room_copy = room_copy->next;
    while (mini_ant != num_of_ants)
    {
        way_copy = way;
        mini_ant++;
        ant = ant_add(ant_init(mini_ant), ant);
        ant_copy = ant;
        min_way = shortest(way_copy);
        while (way_copy->weight != min_way)
            way_copy = way_copy->next;
        way_copy->weight++;
        down = way_copy->down;
        while(ant_copy->num_of_ant != mini_ant)
            ant_copy = ant_copy->next;
        ant_copy->way = down;
    }
    ant_copy = ant;
    /*while (ant_copy)
    {
        room_copy = room->next;
        while (room_copy->index!= ant_copy->way->elem)
            room_copy = room_copy->next;
        printf ("(num_of_ant = %d) (way_first = %s)\n", ant_copy->num_of_ant, room_copy->room_name);
        ant_copy = ant_copy->next;
    }*/
    //close(fd);
    //fd = open("test", O_RDONLY);
   /* int ret;
    j = -1;
    str = (char *)malloc(4096 + 1);
    while((ret = read(fd, str, 4096)))
    {
        if (++j != 0)
            str = (char *)malloc(4096 + 1);
        if (ret > 0)
        {
            str[ret] = '\0';
            printf("%s", str);
        }
        free(str);
    }
    //fd = 0;
    while (get_next_line(fd, &str))
    {
        printf("%s\n", str);
        free(str);
    }*/
    t_data *data_copy;
    data_copy = data;
    data = data->next;
    while(data->next)
    {
        printf("%s\n",data->word);
        data = data->next;
    }
    printf("\n");
    while (got_end != num_of_ants)
    {
        ant_copy = ant;
        while (ant_copy)
        {
            room_copy = room->next;
            room_copy_3 = room->next;
            if (ant_copy->way->next == NULL)
            {
                ant_copy = ant_copy->next;
                continue ;
            }
            now_room = room->next;
            while(now_room->index != ant_copy->way->elem)
                now_room = now_room->next;
            while (room_copy->index != ant_copy->way->next->elem)
                room_copy = room_copy->next;
            if (room_copy->is_visited == 0)
            {
                while (room_copy_3->index != ant_copy->way->next->elem)
                    room_copy_3 = room_copy_3->next;
                now_room->is_visited = 0;
                printf ("L%d->%s ", ant_copy->num_of_ant, room_copy_3->room_name);
                if (room_copy->is_end != 1)
                    room_copy->is_visited = 1;
                else if(!ant_copy->way->next->next)
                    got_end++;
                if (ant_copy->way->next)
                    ant_copy->way = ant_copy->way->next;
            }
            ant_copy = ant_copy->next;
        }
        printf("\n");
    }
    room_free(room);
    way_free(way);
    free(ant);
    data_free(data_copy);
    close(fd);
    return (0);
}