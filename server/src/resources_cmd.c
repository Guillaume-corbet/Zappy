/*
** EPITECH PROJECT, 2020
** Visual Studio Live Share (Workspace)
** File description:
** resources_cmd
*/

#include "server.h"

void eject(char *cmd, client_t *cli, map_t *map)
{
    send_from_socket(cli->fd, cli, KO, NULL);
}

void change_inv_value(map_t *map, client_t *cli, int index, int value)
{
    char *str = malloc(2048);

    str[sprintf(str, "pgt %i %s\n", cli->ia->number,
    map->map[cli->ia->posy][cli->ia->posx].items[index].name)] = 0;
    if (value > 0 &&
    map->map[cli->ia->posy][cli->ia->posx].items[index].count >= value) {
        map->map[cli->ia->posy][cli->ia->posx].items[index].count -= value;
        cli->ia->inv[index].count += value;
        send_from_socket(cli->fd, cli, OK, NULL);
        notif_graph(str, cli);
    } else if (value < 0 && cli->ia->inv[index].count >= value) {
        map->map[cli->ia->posy][cli->ia->posx].items[index].count -= value;
        cli->ia->inv[index].count += value;
        send_from_socket(cli->fd, cli, OK, NULL);
    } else {
        send_from_socket(cli->fd, cli, KO, NULL);
    }
    free(str);
}

void take(char *cmd, client_t *cli, map_t *map)
{
    char *tmp = my_strdup(cmd + 5);

    if (tmp && (tmp[0] <= 122 && tmp[0] >= 97))
        tmp[0] -= 32;
    for (size_t i = 0; i < 7; i++) {
        if (!strncmp(placeable_name[i], tmp, my_strlen(placeable_name[i]))) {
            change_inv_value(map, cli, i, 1);
            return;
        }
    }
    send_from_socket(cli->fd, cli, KO, NULL);
}

void set(char *cmd, client_t *cli, map_t *map)
{
    char *tmp = my_strdup(cmd + 4);

    if (tmp && (tmp[0] <= 122 && tmp[0] >= 97))
        tmp[0] -= 32;
    for (size_t i = 0; i < 7; i++) {
        if (!strncmp(placeable_name[i], tmp, my_strlen(placeable_name[i]))) {
            change_inv_value(map, cli, i, -1);
            return;
        }
    }
    send_from_socket(cli->fd, cli, KO, NULL);
}

void fork_player(char *cmd, client_t *cli, map_t *map)
{
    send_from_socket(cli->fd, cli, KO, NULL);
}