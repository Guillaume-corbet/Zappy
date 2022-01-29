/*
** EPITECH PROJECT, 2020
** PSU_zappy_2019
** File description:
** launching_server
*/

#include "server.h"

void send_map(int fdconnect, map_t *map)
{
    char *str = malloc(2048);
    for (size_t i = 0; i < map->height; i++) {
        for (size_t f = 0; f < map->width; f++) {
            str[sprintf(str, "bct %li %li %i %i %i %i %i %i %i\n", f, i,
            map->origin[i][f].items[0].count, map->origin[i][f].items[1].count,
            map->origin[i][f].items[2].count, map->origin[i][f].items[3].count,
            map->origin[i][f].items[4].count, map->origin[i][f].items[5].count,
            map->origin[i][f].items[6].count)] = 0;
            send_from_socket(fdconnect, NULL, str, NULL);
            memset(str, 0, 2047);
        }
    }
}

void new_connection(int socket, client_t **head)
{
    int fdconnect;
    fd_set server;
    struct sockaddr_in connection;
    unsigned int size = sizeof(connection);
    struct timeval timer;

    timer.tv_sec = 0;
    timer.tv_usec = 100;
    FD_ZERO(&server);
    FD_SET(socket, &server);
    if (select(FD_SETSIZE, &server, NULL, NULL, &timer) > 0) {
        fdconnect = accept(socket, (struct sockaddr *)&connection, &size);
        if (fdconnect != -1){
            printf("New client is connected\n");
            add_client(fdconnect, head);
            write(fdconnect, "WELCOME\n", 9);
        }
    }
}

int open_server(int port, map_t *map)
{
    int soc;
    struct sockaddr_in add;

    add.sin_port = htons(port);
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((soc = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        destroy_map(map, 1);
        exit(84);
    }
    if (bind(soc, (struct sockaddr *) &add, sizeof(add)) == -1) {
        perror("Bind");
        destroy_map(map, 1);
        exit(84);
    }
    if (listen(soc, 100) == -1) {
        perror("Listen");
        destroy_map(map, 1);
        exit(84);
    } return (soc);
}

int is_action_valid(char *cmd)
{
    int i = 0;
    char *tmp = NULL;

    while (i < 12) {
        if (!strncmp(cmd, timer_cmd[i].evt, my_strlen(timer_cmd[i].evt)))
            break;
        i++;
    } if (i == 12)
        return (0);
    if (!strncmp(cmd, "Take ", 5) || !strncmp(cmd, "Set ", 4))
        tmp = my_strdup(cmd + my_strlen(timer_cmd[i].evt));
    if (tmp && tmp[0] >= 97 && tmp[0] <= 122)
        tmp[0] -= 32;
    for (int j = 0; tmp && j < 7; j++) {
        if (!strcmp(tmp, placeable_name[j])) {
            free(tmp);
            tmp = NULL;
        }} if (tmp) {
        free(tmp);
        return (0);
    } return (1);
}

void start(client_t **clients, args_t *argument, map_t **map, team_t **teams)
{
    int soc = open_server(argument->port, *map);
    struct timeval regen;
    struct timeval temp;

    (*map)->map = make_copy(*map);
    inc_timer(&regen, INC_TIMER, argument->freq);
    while (1) {
        if (get_signal(-1) != -1) {
            break;
        } new_connection(soc, clients);
        if (clients)
            run(clients, map, teams, argument->freq);
        gettimeofday(&temp, NULL);
        if (temp.tv_sec >= regen.tv_sec && temp.tv_usec >= regen.tv_usec){
            destroy_map(*map, 0);
            (*map)->map = make_copy(*map);
            inc_timer(&regen, INC_TIMER, argument->freq);
            notif_graph("rfm\n", *clients);
        }
    }
}