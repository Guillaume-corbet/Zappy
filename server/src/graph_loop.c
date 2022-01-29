/*
** EPITECH PROJECT, 2020
** Visual Studio Live Share (Workspace)
** File description:
** garph_loop
*/

#include "server.h"

t_graph graph_evt[] =
{
    {"pin", &pin},
    {"plv", &plv}
};

int manage_event_graph(client_t *info, char *buffer, map_t **map)
{
    int check = -1;

    for (unsigned int i = 0; i < sizeof(graph_evt) / 16; i++) {
        if (strncmp(buffer, graph_evt[i].evt, strlen(graph_evt[i].evt)) == 0)
            check = i;
    } if (check == -1) {
        send_from_socket(info->fd, info, KO, NULL);
        free(buffer);
        return (0);
    } graph_evt[check].func(buffer, info);
    return (0);
}

void graph_cut(client_t *info, map_t **map)
{
    char *buffer = receive_from_socket(info);
    char **tab;

    if (!buffer || !strcmp(buffer, "exit\n")) {
        close(info->fd);
        return;
    } tab = cut_str(buffer, '\n');
    for (size_t i = 0; i < arraylen(tab); i++) {
        manage_event_graph(info, buffer, map);
    } destroy_array(tab);
    free(buffer);
}

client_t *graph_loop(client_t *cur, client_t **chain, map_t **map)
{
    fd_set cl;
    struct timeval time;
    int value = 0;

    time.tv_sec = 0;
    time.tv_usec = 100;
    if (cur != NULL) {
        FD_ZERO(&cl);
        FD_SET(cur->fd, &cl);
        if (select(FD_SETSIZE, &cl, NULL, NULL, &time) == -1) {
            cur = clean_client(cur, chain);
            value = -1;
        } else if (FD_ISSET(cur->fd, &cl)) {
            graph_cut(cur, map);
        } if (cur && value != -1)
            cur = cur->next;
    } return (cur);
}