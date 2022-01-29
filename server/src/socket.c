/*
** EPITECH PROJECT, 2020
** Visual Studio Live Share (Workspace)
** File description:
** socket
*/

#include "server.h"

void send_from_socket(int sock, client_t *info, char *buffer, char *app)
{
    fd_set val_fdset;
    struct timeval time;

    time.tv_sec = 0;
    time.tv_usec = 1000;
    FD_ZERO(&val_fdset);
    FD_SET(sock, &val_fdset);
    if (select(FD_SETSIZE, NULL, &val_fdset, NULL, &time) == -1) {
        get_signal(SIGINT);
        return;
    }
    if (FD_ISSET(sock, &val_fdset) && !app) {
        if (write(sock, buffer, strlen(buffer)) <= 0)
            return;
    }
    else if (FD_ISSET(sock, &val_fdset)) {
        if (dprintf(sock, buffer, app) < 0)
            return;
    }
}

char *receive_from_socket(client_t *info)
{
    char *buffer = malloc(sizeof(char) * 1025);
    int sto;

    memset(buffer, 0, 1025);
    sto = read(info->fd, buffer, 1024);
    if (sto <= 0) {
        free(buffer);
        return (NULL);
    } return (buffer);
}