/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** incantation
*/

#include "server.h"

int have_resources(client_t *cli)
{
    for (size_t i = 1; i < 7; i++) {
        if (cli->ia->inv[i].count <
        needs_for_incantation[cli->ia->level - 1][i])
            return (0);
    }
    return (1);
}

void dec_resources(client_t *cli)
{
    for (size_t i = 1; i < 7; i++) {
        cli->ia->inv[i].count -= needs_for_incantation[cli->ia->level - 1][i];
    } cli->ia->level++;
}

void set_all_client_state(client_t *cli)
{
    client_t *cur = cli;

    while (cur && cur->prev)
        cur = cur->prev;
    while (cur) {
        if (cur->ia && cur->ia->incan == 1 && cur->ia->level ==
        cli->ia->level && cur->ia->posx == cli->ia->posx &&
        cur->ia->posy == cli->ia->posy && have_resources(cur))
            cur->ia->incan = 2;
        cur = cur->next;
    }
}

int count_incanting_players(client_t *cli)
{
    int count = 0;
    int incan_count = 0;
    int check = 0;
    client_t *cur = cli;

    while (cur && cur->prev)
        cur = cur->prev;
    while (cur) {
        if (cur->ia && cur->ia->incan == 1 && cur->ia->level ==
        cli->ia->level && cur->ia->posx == cli->ia->posx &&
        cur->ia->posy == cli->ia->posy && have_resources(cur)) {
            check = 1;
            count++;
        } if (check == 1 && cur->ia->incan == 1)
            incan_count++;
        cur = cur->next;
        check = 0;
    } if (count == needs_for_incantation[cli->ia->level - 1][0])
        set_all_client_state(cli);
    return (count);
}

void incantation(char *cmd, client_t *cli, map_t *map)
{
    int result = 1;
    char *str = malloc(2048);

    str[sprintf(str, INCANTATION, cli->ia->level + 1)] = 0;
    if (cli->ia->incan == 1 && cli->ia->level == 1 && cli->ia->inv[1].count
    == needs_for_incantation[cli->ia->level - 1][1]) {
        send_from_socket(cli->fd, cli, str, NULL);
        dec_resources(cli);
    } else if ((cli->ia->incan == 1 && count_incanting_players(cli) ==
    needs_for_incantation[cli->ia->level - 1][0]) || cli->ia->incan == 2) {
        send_from_socket(cli->fd, cli, str, NULL);
        dec_resources(cli);
    } else {
        send_from_socket(cli->fd, cli, KO, NULL);
        result = -1;
    } str[sprintf(str, "pie %i %i %i %i %i\n", cli->ia->posx, cli->ia->posy,
    result, cli->ia->number, cli->ia->level)] = 0;
    cli->ia->incan = 0;
    notif_graph(str, cli);
    free(str);
}