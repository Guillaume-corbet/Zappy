/*
** EPITECH PROJECT, 2020
** Visual Studio Live Share (Workspace)
** File description:
** communication_cmd
*/

#include "server.h"

char *get_text(char *cmd)
{
    char *res;
    int i = 10;

    if (!cmd || my_strlen(cmd) <= 10)
        return (NULL);
    while (cmd[i] && cmd[i] != '\n')
        i++;
    res = my_strndup(cmd + 10, i - 10);
    return (res);
}

int get_all_dir(client_t *sender, client_t *receiver, int sizex, int sizey)
{
    if (receiver->ia->posx == sender->ia->posx &&
    receiver->ia->posy == sender->ia->posy)
        return (0);
    else if ((abs(receiver->ia->posx - sender->ia->posx) >= abs(sizex -
    (receiver->ia->posx - sender->ia->posx))) != (sender->ia->posx >
    receiver->ia->posx)) {
        if ((abs(receiver->ia->posy - sender->ia->posy) >= abs(sizey -
        (receiver->ia->posy - sender->ia->posy))) != (sender->ia->posy
        > receiver->ia->posy))
            return (6);
        else
            return (8);
    } else {
        if ((abs(receiver->ia->posy - sender->ia->posy) >= abs(sizey -
        (receiver->ia->posy - sender->ia->posy))) != (sender->ia->posy
        > receiver->ia->posy))
            return (4);
        else
            return (2);
    }
}

int update_dir(int dir, int x, int y)
{
    if (dir == 0)
        return (0);
    if (y <= (int)((0.33333) * (float)(x)) || y == 0) {
        if (dir == 2 || dir == 6)
            dir++;
        else
            dir--;
    } else if (y >= 3 * x || x == 0) {
        if (dir == 4)
            dir++;
        else if (dir == 8)
            dir = 1;
        else
            dir--;
    }
    return (dir);
}

int get_direction(client_t *sender, client_t *receiver, map_t *map)
{
    int dir = get_all_dir(sender, receiver, map->width, map->height);
    int x = abs(receiver->ia->posx - sender->ia->posx);
    int y = abs(receiver->ia->posy - sender->ia->posy);

    if (abs(map->height - (receiver->ia->posy - sender->ia->posy)) < y)
        y = abs(map->height - (receiver->ia->posy - sender->ia->posy));
    if (abs(map->width - (receiver->ia->posx - sender->ia->posx)) < x)
        x = abs(map->width - (receiver->ia->posx - sender->ia->posx));
    if ((dir = update_dir(dir, x, y)) == 0)
        return (0);
    dir = (dir + (2 * (receiver->ia->orientation - 1))) % 8;
    if (dir == 0)
        dir = 8;
    return (dir);
}

void broadcast(char *cmd, client_t *cli, map_t *map)
{
    char *text = get_text(cmd);
    char *tmp;
    client_t *cur = cli;

    if (!text)
        return;
    tmp = malloc(my_strlen(text) + 17);
    while (cur && cur->prev)
        cur = cur->prev;
    while (cur) {
        if (cur->ia) {
            memset(tmp, 0, my_strlen(text) + 17);
            tmp[sprintf(tmp, BROAD, get_direction(cli, cur, map), text)] = 0;
            send_from_socket(cur->fd, cur, tmp, NULL);
        } cur = cur->next;
    } send_from_socket(cli->fd, cli, OK, NULL);
    free(tmp);
    free(text);
}