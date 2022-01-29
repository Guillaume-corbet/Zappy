/*
** EPITECH PROJECT, 2020
** PSU_zappy_2019
** File description:
** ia
*/

#include "server.h"

void clean_ia(IA_t **to_delete)
{
    free((*to_delete)->team_name);
    free((*to_delete)->life);
    free((*to_delete)->next_action);
    for (size_t i = 0; i < 7; i++)
        free((*to_delete)->inv[i].name);
    delete_queue(&(*to_delete)->queued);
}

void set_values_ia(IA_t **ia, char *team_name, int f)
{
    int entire_value = 126 / f;
    (*ia)->life = malloc(sizeof(struct timeval));
    (*ia)->next_action = malloc(sizeof(struct timeval));

    (*ia)->level = 1;
    (*ia)->team_name = strdup(team_name);
    (*ia)->orientation = rand() % 4 + 1;
    (*ia)->queued = create_queue();
    (*ia)->incan = 0;
    inc_timer((*ia)->life, 126, f);
}

IA_t *create_ia(int posx, int posy, char *team_name, int f)
{
    IA_t *newer = malloc(sizeof(IA_t));
    static int playernb = 1;

    printf("x = %i y = %i\n", posx, posy);
    for (size_t i = 0; i < 7; i++) {
        newer->inv[i].count = 0;
        newer->inv[i].name = my_strdup(placeable_name[i]);
        newer->inv[i].token = placeable_name[i][0];
    } newer->inv[0].count = 9;
    newer->posx = posx;
    newer->posy = posy;
    newer->number = playernb;
    playernb++;
    set_values_ia(&newer, team_name, f);
    return (newer);
}

void addin_ia(team_t *teams, client_t *cli, map_t *map, int f)
{
    char *str = malloc(128);
    client_t *temp = cli;

    cli->ia = create_ia(rand() % map->width,
    rand() % map->height, teams->name, f);
    str[sprintf(str, NEW_PLAYER, teams->max - teams->count,
    map->width, map->height)] = 0;
    send_from_socket(cli->fd, cli, str, NULL);
    free(str);
    str = malloc(2048);
    str[sprintf(str, NEW_IA, cli->ia->number, cli->ia->posx,
    cli->ia->posy, cli->ia->orientation, cli->ia->level,
    cli->ia->team_name)] = 0;
    while (temp && temp->prev && (temp = temp->prev));
    while (temp) {
        if (temp->graph)
            send_from_socket(temp->fd, temp, str, NULL);
        temp = temp->next;
    } free(str);
}