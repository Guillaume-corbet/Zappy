/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** look
*/

#include "server.h"

void update_pos(int *x, int *y, map_t *map)
{
    int nb = *x;

    if ((*x) < 0)
        (*x) = map->width + (*x);
    else if ((*x) >= map->width)
        (*x) -= map->width;
    nb = *y;
    if ((*y) < 0)
        (*y) = map->height + (*y);
    else if ((*y) >= map->height)
        (*y) -= map->height;
}

char *set_tile_cnt(map_t *map, int x, int y, client_t *cli)
{
    char *tmp = my_strdup("");
    client_t *first = cli;

    while (first && first->prev)
        first = first->prev;
    update_pos(&x, &y, map);
    while (first) {
        if (first->ia && first->ia->posx == x && first->ia->posy == y)
            tmp = my_strcat(tmp, "Player ", 1);
        first = first->next;
    } for (int i = 0; i < 7; i++) {
        if (map->map[y][x].items[i].count > 0) {
            tmp = my_strcat(tmp,
            my_strcat(map->map[y][x].items[i].name, " ", 0), 3);
        }
    } if (my_strlen(tmp))
        tmp[my_strlen(tmp) - 1] = ',';
    else {
        free(tmp);
        tmp = my_strdup(",");
    } return (tmp);
}

char *get_tile_look(map_t *map, int i, int j, client_t *cli)
{
    int x = cli->ia->posx;
    int y = cli->ia->posy;

    if (cli->ia->orientation == 1) {
        y -= i;
        x += j;
    } else if (cli->ia->orientation == 3) {
        y += i;
        x -= j;
    } else {
        if (cli->ia->orientation == 2) {
            y += j;
            x += i;
        } else if (cli->ia->orientation == 4) {
            y -= j;
            x -= i;
        }
    }
    return (set_tile_cnt(map, x, y, cli));
}

void look(char *cmd, client_t *cli, map_t *map)
{
    int pos;
    char *tmp = my_strdup("[");

    if (cli->ia->orientation < 1 || cli->ia->orientation > 4) {
        send_from_socket(cli->fd, cli, KO, NULL);
        free(tmp);
        return;
    } for (int i = 0; i < cli->ia->level; i++) {
        for (int j = -i; j <= i; j++) {
            tmp = my_strcat(tmp, get_tile_look(map, i, j, cli), 3);
        }
    } if (my_strlen(tmp))
        tmp[my_strlen(tmp) - 1] = 0;
    tmp = my_strcat(tmp, "]\n", 1);
    send_from_socket(cli->fd, cli, tmp, NULL);
    free(tmp);
}

void inventory(char *cmd, client_t *cli, map_t *map)
{
    char *tmp = malloc(1024);

    tmp[sprintf(tmp, INV, cli->ia->inv[0].count, cli->ia->inv[1].count,
    cli->ia->inv[2].count, cli->ia->inv[3].count, cli->ia->inv[4].count,
    cli->ia->inv[5].count, cli->ia->inv[6].count)] = 0;
    send_from_socket(cli->fd, cli, tmp, NULL);
    free(tmp);
}