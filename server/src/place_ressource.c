/*
** EPITECH PROJECT, 2020
** PSU_zappy_2019
** File description:
** place_ressource
*/

#include "server.h"

void place_mendiane(tile_t **map, int nb, int height, int width)
{
    int random = rand() % (height * width);
    int randx = random % width;
    int randy = (int)(random / width);

    while (nb != 0){
        if (map[randy][randx].items[4].count == 0) {
            map[randy][randx].items[4].count += 1;
            nb--;
        }
        else {
            random = rand() % (height * width);
            randx = random % width;
            randy = (int)(random / width);
        }
    }
}

void place_phiras(tile_t **map, int nb, int height, int width)
{
    int random = rand() % (height * width);
    int randx = random % width;
    int randy = (int)(random / width);

    while (nb != 0){
        if (map[randy][randx].items[5].count == 0) {
            map[randy][randx].items[5].count += 1;
            nb--;
        }
        else {
            random = rand() % (height * width);
            randx = random % width;
            randy = (int)(random / width);
        }
    }
}

void place_thystame(tile_t **map, int nb, int height, int width)
{
    int random = rand() % (height * width);
    int randx = random % width;
    int randy = (int)(random / width);

    while (nb != 0){
        if (map[randy][randx].items[6].count == 0) {
            map[randy][randx].items[6].count += 1;
            nb--;
        }
        else {
            random = rand() % (height * width);
            randx = random % width;
            randy = (int)(random / width);
        }
    }
}