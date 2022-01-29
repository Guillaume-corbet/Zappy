/*
** EPITECH PROJECT, 2020
** Visual Studio Live Share (Workspace)
** File description:
** movements_cmd
*/

#include "server.h"

void connect_nbr(char *cmd, client_t *cli, map_t *map)
{
    char *nb;
    send_from_socket(cli->fd, cli, "0\n", NULL);
}

int move_client(client_t *cli)
{
    if (cli->ia->orientation == 1) {
        cli->ia->posy--;
    } else if (cli->ia->orientation == 2) {
        cli->ia->posx++;
    } else {
        if (cli->ia->orientation == 3) {
            cli->ia->posy++;
        } else if (cli->ia->orientation == 4) {
            cli->ia->posx--;
        } else {
            return (-1);
        }
    }
    return (0);
}

void forward(char *cmd, client_t *cli, map_t *map)
{
    char *str = malloc(2048);

    if (move_client(cli) == -1) {
        send_from_socket(cli->fd, cli, KO, NULL);
        return;
    }
    if (cli->ia->posy < 0)
        cli->ia->posy = map->height - 1;
    else if (cli->ia->posy > map->height - 1)
        cli->ia->posy = 0;
    send_from_socket(cli->fd, cli, OK, NULL);
    if (cli->ia->posx < 0)
        cli->ia->posx = map->width - 1;
    else if (cli->ia->posx > map->width - 1)
        cli->ia->posx = 0;
    str[sprintf(str, "ppo %i %i %i %i\n", cli->ia->number, cli->ia->posx,
    cli->ia->posy, cli->ia->orientation)] = 0;
    notif_graph(str, cli);
    free(str);
}

void right(char *cmd, client_t *cli, map_t *map)
{
    char *str = malloc(2048);

    cli->ia->orientation++;
    if (cli->ia->orientation > 4)
        cli->ia->orientation = 1;
    send_from_socket(cli->fd, cli, OK, NULL);
    str[sprintf(str, "ppo %i %i %i %i\n", cli->ia->number, cli->ia->posx,
    cli->ia->posy, cli->ia->orientation)] = 0;
    notif_graph(str, cli);
    free(str);
}

void left(char *cmd, client_t *cli, map_t *map)
{
    char *str = malloc(2048);

    cli->ia->orientation--;
    if (cli->ia->orientation < 1)
        cli->ia->orientation = 4;
    send_from_socket(cli->fd, cli, OK, NULL);
    str[sprintf(str, "ppo %i %i %i %i\n", cli->ia->number, cli->ia->posx,
    cli->ia->posy, cli->ia->orientation)] = 0;
    notif_graph(str, cli);
    free(str);
}