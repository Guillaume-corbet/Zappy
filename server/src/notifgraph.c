/*
** EPITECH PROJECT, 2020
** PSU_zappy_2019
** File description:
** notifgraph
*/

#include "server.h"

void plv(char *cmd, client_t *cur)
{
    int tofind = -1;
    char *str = malloc(2048);
    client_t *temp = cur;

    if (cmd[4])
        tofind = atoi(cmd + 4);
    while (temp && temp->prev)
        temp = temp->prev;
    while (temp){
        if (temp->ia && temp->ia->number == tofind){
            str[sprintf(str, "plv %i %i\n",
            temp->ia->number, temp->ia->level)] = 0;
            send_from_socket(cur->fd, cur, str, NULL);
            break;
        } temp = temp->next;
    } free(str);
}

void pin(char *cmd, client_t *cur)
{
    int tofind = -1;
    char *str = malloc(2048);
    client_t *temp = cur;

    if (cmd[4])
        tofind = atoi(cmd + 4);
    while (temp && temp->prev)
        temp = temp->prev;
    while (temp){
        if (temp->ia && temp->ia->number == tofind){
            str[sprintf(str, "pin %i %i %i %i %i %i %i %i %i %i\n",
            temp->ia->number, temp->ia->posx, temp->ia->posy,
            temp->ia->inv[0].count, temp->ia->inv[1].count,
            temp->ia->inv[2].count, temp->ia->inv[3].count,
            temp->ia->inv[4].count, temp->ia->inv[5].count,
            temp->ia->inv[6].count)] = 0;
            send_from_socket(cur->fd, cur, str, NULL);
            break;
        } temp = temp->next;
    } free(str);
}

void incantation_notif(client_t **cli)
{
    char *str = malloc(2048);

    (*cli)->ia->incan = 1;
    str[sprintf(str, "pic %i %i %i %i\n", (*cli)->ia->posx, (*cli)->ia->posy,
    (*cli)->ia->level + 1, (*cli)->ia->number)] = 0;
    notif_graph(str, *cli);
    free(str);
}

void notif_graph(char *cmd, client_t *cur)
{
    client_t *temp = cur;

    while (temp && temp->prev)
        temp = temp->prev;
    while (temp){
        if (temp->graph)
            send_from_socket(temp->fd, temp, cmd, NULL);
        temp = temp->next;
    }
}