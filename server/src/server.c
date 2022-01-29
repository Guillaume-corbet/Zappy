/*
** EPITECH PROJECT, 2020
** Visual Studio Live Share (Workspace)
** File description:
** server
*/

#include "server.h"

t_event event[] =
{
    {"Forward", &forward},
    {"Right", &right},
    {"Left", &left},
    {"Look", &look},
    {"Inventory", &inventory},
    {"Broadcast", &broadcast},
    {"Connect_nbr", &connect_nbr},
    {"Fork", &fork_player},
    {"Eject", &eject},
    {"Take", &take},
    {"Set", &set},
    {"Incantation", &incantation}
};

int manage_event_server(client_t *info, char *buffer, map_t *map)
{
    int check = -1;

    if (!buffer) {
        close(info->fd);
        return (1);
    } for (unsigned int i = 0; i < sizeof(event) / 16; i++) {
        if (strncmp(buffer, event[i].evt, strlen(event[i].evt)) == 0)
            check = i;
    } if (check == -1) {
        send_from_socket(info->fd, info, KO, NULL);
        return (0);
    } event[check].func(buffer, info, map);
    return (0);
}

void checkqueue(client_t *cur, client_t **head, map_t **map, int t)
{
    struct timeval now;
    int value = -1;

    if (!cur->ia || !(cur->ia->queued) || !(cur->ia->queued->front))
        return;
    gettimeofday(&now, NULL);
    for (int i = 0; cur->ia->queued->front->next && i < 12; i++) {
        if (!strncmp(cur->ia->queued->front->next->cmd, timer_cmd[i].evt,
        my_strlen(timer_cmd[i].evt)))
            value = timer_cmd[i].val;
    } if (cur->ia->next_action->tv_sec <= now.tv_sec
    && cur->ia->next_action->tv_usec <= now.tv_usec) {
        manage_event_server(cur, cur->ia->queued->front->cmd, (*map));
        pop_action(&(cur->ia->queued));
        if (cur->ia->queued->front && value != -1) {
            inc_timer(cur->ia->next_action, value, t);
        }
    }
}

client_t *loop(client_t *cur, client_t **chain, map_t **map, int f)
{
    fd_set cl;
    struct timeval time;
    int value = 0;

    time.tv_sec = 0;
    time.tv_usec = 100;
    if (is_death(cur, f) == 1) {
        send_from_socket(cur->fd, cur, "dead\n", NULL);
        return (clean_client(cur, chain));
    } if (cur != NULL) {
        checkqueue(cur, chain, map, f);
        FD_ZERO(&cl);
        FD_SET(cur->fd, &cl);
        if (select(FD_SETSIZE, &cl, NULL, NULL, &time) == -1) {
            cur = clean_client(cur, chain);
            value = -1;
        } else if (FD_ISSET(cur->fd, &cl)) {
            add_action(&(cur->ia->queued), cur, f);
        } if (cur && value != -1)
            cur = cur->next;
    } return (cur);
}

int manage_new_client(client_t **cur, map_t **map, team_t ** origin, int f)
{
    team_t *test = NULL;
    char *buff = receive_from_socket((*cur));

    if (buff != NULL) {
        if (strcmp("GRAPHIC", buff) == 0)
            addin_graphic(buff, (*cur), *map);
        else if ((test = find_team_by_name(buff, origin)) != NULL
        && add_client_to_team(&test) == 0)
            addin_ia(test, (*cur), *map, f);
        else
            send_from_socket((*cur)->fd, (*cur), KO, NULL);
        free(buff);
        return (1);
    } return (0);
}

void run(client_t **chain, map_t **map, team_t **origin, int f)
{
    client_t *cur = *chain;

    while (cur != NULL) {
        if (cur->ia) {
            cur = loop(cur, chain, map, f);
        } else if (cur->graph) {
            cur = graph_loop(cur, chain, map);
        } else if (!manage_new_client(&cur, map, origin, f)) {
            cur = clean_client(cur, chain);
        }
    }
}