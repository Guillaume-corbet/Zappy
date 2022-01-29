/*
** EPITECH PROJECT, 2020
** PSU_zappy_2019
** File description:
** graphic
*/

#include "server.h"

void delete_graphic(graphic_t **to_delete)
{
    free((*to_delete)->follow);
}

graphic_t *create_graphic(char *follow)
{
    graphic_t *newer = malloc(sizeof(graphic_t));

    newer->follow = strdup(follow);
    return (newer);
}

void addin_graphic(char *cmd, client_t *cli, map_t *map)
{
    char *str = malloc(2048);
    client_t *temp = cli;

    cli->graph = create_graphic(my_strdup("World"));
    str[sprintf(str, "msz %i * %i (%i)\n", map->width, map->height,
    map->height * map->width)] = 0;
    send_from_socket(cli->fd, cli, str, NULL);
    while (temp && temp->prev)
        temp = temp->prev;
    while (temp){
        if (temp->ia){
            memset(str, 0, 2047);
            str[sprintf(str, NEW_IA, temp->ia->number, temp->ia->posx,
            temp->ia->posy, temp->ia->orientation, temp->ia->level,
            temp->ia->team_name)] = 0;
            send_from_socket(cli->fd, cli, str, NULL);
        } temp = temp->next;
    } send_map(cli->fd, map);
    free(str);
}