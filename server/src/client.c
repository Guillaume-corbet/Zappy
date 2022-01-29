/*
** EPITECH PROJECT, 2020
** PSU_zappy_2019
** File description:
** client
*/

#include "server.h"

client_t *find_client_by_fd(int fd, client_t *head)
{
    client_t *temp = head;

    while (temp && temp->fd != fd)
        temp = temp->next;
    if (temp && temp->fd == fd)
        return (temp);
    return (NULL);
}

int is_death(client_t *current, int f)
{
    struct timeval now;
    char *str;

    if (!current->ia)
        return (0);
    gettimeofday(&now, NULL);
    if (current->ia->life->tv_sec <= now.tv_sec
    && current->ia->life->tv_usec <= now.tv_usec) {
        current->ia->inv[0].count--;
        str = malloc(16);
        inc_timer(current->ia->life, 126, f);
        str[sprintf(str, "ptf %i\n", current->ia->number)] = 0;
        notif_graph(str, current);
        free(str);
    } if (current->ia->inv[0].count <= 0) {
        return (1);
    } return (0);
}

client_t *remove_client(client_t *to_delete, client_t **head)
{
    client_t *next;

    printf("client removed\n");
    if (to_delete->prev == NULL && to_delete->next == NULL) {
        (*head) = NULL;
        free(to_delete);
        return (NULL);
    }
    next = NULL;
    if (to_delete->prev == NULL) {
        (*head) = to_delete->next;
        to_delete->next->prev = NULL;
        next = to_delete->next;
    } else if (to_delete->next == NULL && to_delete->prev != NULL) {
        to_delete->prev->next = NULL;
    } else {
        to_delete->prev->next = to_delete->next;
        to_delete->next->prev = to_delete->prev;
        next = to_delete->next;
    } free(to_delete);
    return (next);
}

client_t *clean_client(client_t *to_delete, client_t **head)
{
    char *str;
    if (to_delete->ia) {
        str = malloc(16);
        str[sprintf(str, "pdi %i\n", to_delete->ia->number)] = 0;
        notif_graph(str, to_delete);
        free(str);
        clean_ia(&(to_delete->ia));
        free(to_delete->ia);
        close(to_delete->fd);
    } else if (to_delete->graph) {
        delete_graphic(&(to_delete->graph));
        free(to_delete->graph);
    }
    return (remove_client(to_delete, head));
}

void add_client(int fd, client_t **head)
{
    client_t *newer = malloc(sizeof(client_t));
    client_t *temp = (*head);

    newer->fd = fd;
    newer->graph = NULL;
    newer->ia = NULL;
    newer->next = NULL;
    if ((*head) == NULL){
        newer->prev = NULL;
        (*head) = newer;
    } else {
        while (temp && temp->next)
            temp = temp->next;
        temp->next = newer;
        newer->prev = temp;
    }
}