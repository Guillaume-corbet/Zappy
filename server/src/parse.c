/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** parse
*/

#include "server.h"

void manage_arg(char last, char *str, args_t *args)
{
    if (last == 'c') {
        args->client_nb = atoi(str);
    } else if (last == 'p') {
        args->port = atoi(str);
    } else {
        if (last == 'x')
            args->x = atoi(str);
        else if (last == 'y')
            args->y = atoi(str);
        else
            args->freq = atoi(str);
    }
}

args_t *init_args(void)
{
    args_t *args = (args_t *)malloc(sizeof(args_t));

    args->client_nb = 2;
    args->freq = 100;
    args->port = 4242;
    args->x = 10;
    args->y = 10;
    return (args);
}

char set_last(char *str, args_t *args)
{
    if (!strchr("pxycnf", str[1])) {
        free(args);
        return (0);
    }
    return (str[1]);
}

void set_team_size(team_t **teams, int nb)
{
    team_t *cur = *teams;

    while (cur) {
        cur->max = nb;
        cur = cur->next;
    }
}

args_t *parse(int ac, char **av, team_t **teams)
{
    char last = 0;
    args_t *args = init_args();

    for (size_t i = 1; i < ac; i++) {
        if (my_strlen(av[i]) == 2 && av[i][0] == '-' &&
        (last = set_last(av[i], args)) == 0) {
            printf("Error in arguments.\n");
            return (NULL);
        } else if (last != 0 && strchr("xyfcp", last)) {
            manage_arg(last, av[i], args);
        } else if (av[i][0] != '-') {
            create_team(av[i], 2, teams);
        }
        last = last;
        if (my_strlen(av[i]) != 2 && last != 'n')
            last = 0;
    }
    set_team_size(teams, args->client_nb);
    return (args);
}