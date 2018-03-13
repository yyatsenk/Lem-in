#include <unistd.h>
#include "libft/libft.h"
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void make_line()
{
    int x1;
    int y1;
    int x2;
    int y2;
    int kof;
    // find special room t_connect
    x1 = room_copy->coords.x;
    y1 = room_copy->coords.y;
    // find special room t_connect
    x2 = room_copy->coords.x;
    y2 = room_copy->coords.y;
    kof = (y2 - y1) /(x2- x1); 
    while (x1 != x2)
    {
        mlx_put_pixel(..., ..., x1, y1, RED);
        x1++;
        y1 += kof;
    }
}

int main(void)
{
    t_room *room;
    t_room *room_copy;
    int x1;
    int y1;
    int x2;
    int y2;
    void *mlx_ptr;
    void *win_ptr;

    room = malloc;
    mlx_ptr = mlx_new_window(win_ptr,100, 100);
    win_ptr = ...;
    room_copy = room->next;
    while (room_copy)
    {
        make_line(mlx, mlx, room_copy);
        mlx_write(..., ..., room->coords.x * kof, room->coords.x * kof, "R", GREEN);
        room_copy = room_copy->next;
    }
    return (0);
}