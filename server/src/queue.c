/*
** EPITECH PROJECT, 2020
** PSU_zappy_2019
** File description:
** queue
*/

#include "server.h"

void pop_action(queue_t **head)
{
    action_t *temp = (*head)->front;

    if ((*head)->front && (*head)->front->cmd) {
        free((*head)->front->cmd);
    } if ((*head)->front && (*head)->front->next)
        (*head)->front = (*head)->front->next;
    else
        (*head)->front = NULL;
    (*head)->count--;
    free(temp);
}

void create_action(char *cmd, queue_t **to_add, client_t *cur, int f)
{
    action_t *new = malloc(sizeof(action_t));
    action_t *temp = (*to_add)->front;
    int value = -1;

    new->cmd = my_strdup(cmd);
    new->next = NULL;
    while (temp && temp->next)
        temp = temp->next;
    if (temp)
        temp->next = new;
    (*to_add)->tail = new;
    for (int i = 0; i < 12; i++) {
        if (!strncmp(new->cmd, timer_cmd[i].evt, my_strlen(timer_cmd[i].evt)))
            value = timer_cmd[i].val;
    } if ((*to_add)->front == NULL && value != -1) {
        (*to_add)->front = new;
        inc_timer(cur->ia->next_action, value, f);
    }
}

void add_action(queue_t **to_add, client_t *cur, int f)
{
    char *cmd = receive_from_socket(cur);
    char **tab;

    if (!cmd) {
        cur->ia->inv[0].count = 0;
        return;
    } tab = cut_str(cmd, '\n');
    for (size_t i = 0; i < arraylen(tab); i++) {
        if ((*to_add)->count == 10) {
            break;
        } else if (!is_action_valid(tab[i])) {
            send_from_socket(cur->fd, cur, KO, NULL);
        } else {
            INCANT;
            create_action(tab[i], to_add, cur, f);
            (*to_add)->count++;
        }
    } destroy_array(tab);
    free(cmd);
}

void delete_queue(queue_t **to_delete)
{
    action_t *temp;
    action_t *temp2;

    if (!(*to_delete))
        return;
    temp = (*to_delete)->front;
    while (temp) {
        free(temp->cmd);
        temp2 = temp;
        temp = temp->next;
        free(temp2);
    }
    free(*to_delete);
}

queue_t *create_queue(void)
{
    queue_t *newer = malloc(sizeof(queue_t));

    newer->count = 0;
    newer->front = NULL;
    newer->tail = NULL;
    return (newer);
}