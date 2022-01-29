/*
** EPITECH PROJECT, 2020
** PSU_zappy_2019
** File description:
** main
*/
#include "server.h"

void inc_timer(struct timeval *timer, int value, int f)
{
    gettimeofday(timer, NULL);
    timer->tv_sec += value / f;
    timer->tv_usec += (int)((((float)(value) / (float)(f)) -
    (float)((int)((float)(value) / (float)(f)))) * 1000000.0);
    if (timer->tv_usec >= 1000000) {
        timer->tv_usec -= 1000000;
        timer->tv_sec++;
    }
}

void set_signal_handling(void)
{
    struct sigaction act;

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);
    act.sa_flags = SA_SIGINFO;
    act.sa_handler = signal_handler;
    sigaction(SIGINT, &act, NULL);
}

int main(int ac, char **av)
{
    map_t *map = (map_t *)malloc(sizeof(map_t));
    team_t *teams = NULL;
    args_t *args = parse(ac, av, &teams);
    client_t *clients = NULL;

    srand(time(NULL));
    set_signal_handling();
    if (!args) {
        return (84);
    } map->origin = generation(args->x, args->y);
    map->width = args->x;
    map->height = args->y;
    map->map = make_copy(map);
    printf("Server is now listening on port %i\n", args->port);
    start(&clients, args, &map, &teams);
    destroy_all(&clients, &map, &teams, args);
    return (0);
}