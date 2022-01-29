/*
** EPITECH PROJECT, 2020
** Visual Studio Live Share (Workspace)
** File description:
** team
*/

#include "server.h"

team_t *find_team_by_name(char *name, team_t **origin)
{
    team_t *temp = (*origin);

    while (temp && strncmp(name, temp->name, strlen(name) - 1) != 0) {
        temp = temp->next;
    } if (temp && strncmp(name, temp->name, strlen(name) - 1) == 0)
        return (temp);
    return (NULL);
}

int add_client_to_team(team_t **team)
{
    if ((*team)->count >= (*team)->max)
        return (-1);
    else {
        (*team)->count++;
        return (0);
    }
}

void create_team(char *team_name, int max, team_t **origin)
{
    team_t *newer = malloc(sizeof(team_t));
    team_t *temp = (*origin);

    newer->count = 0;
    newer->max = max;
    newer->name = strdup(team_name);
    newer->next = NULL;
    if ((*origin) == NULL) {
        newer->prev = NULL;
        (*origin) = newer;
    } else {
        while (temp && temp->next)
            temp = temp->next;
        temp->next = newer;
        newer->prev = temp;
    }
}