/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** signal
*/

#include "server.h"

void destroy_clients(client_t **all)
{
    client_t *cur = *all;

    while (cur) {
        close(cur->fd);
        cur = clean_client(cur, all);
    }
}

void destroy_teams(team_t **all)
{
    team_t *cur = *all;
    team_t *sto;

    while (cur) {
        sto = cur;
        free(cur->name);
        cur = cur->next;
        free(sto);
    }
}

void destroy_all(client_t **clients, map_t **map, team_t **teams, args_t *arg)
{
    printf("*** DESTROY ALL ***\n");
    if (clients)
        destroy_clients(clients);
    if (*map)
        destroy_map(*map, 1);
    if (teams)
        destroy_teams(teams);
    free(arg);
}

int get_signal(int sig)
{
    static int last_sig = -1;

    if (sig != -1)
        last_sig = sig;
    return (last_sig);
}

void signal_handler(int sig)
{
    get_signal(sig);
}