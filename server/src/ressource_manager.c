/*
** EPITECH PROJECT, 2020
** PSU_zappy_2019
** File description:
** ressource_manager
*/

#include "server.h"

void place_sibur(tile_t **map, int nb, int height, int width)
{
    int random = rand() % (height * width);
    int randx = random % width;
    int randy = (int)(random / width);

    while (nb != 0){
        if (map[randy][randx].items[3].count == 0) {
            map[randy][randx].items[3].count += 1;
            nb--;
        }
        else {
            random = rand() % (height * width);
            randx = random % width;
            randy = (int)(random / width);
        }
    }
}

void place_deraumere(tile_t **map, int nb, int height, int width)
{
    int random = rand() % (height * width);
    int randx = random % width;
    int randy = (int)(random / width);

    while (nb != 0){
        if (map[randy][randx].items[2].count == 0) {
            map[randy][randx].items[2].count += 1;
            nb--;
        }
        else {
            random = rand() % (height * width);
            randx = random % width;
            randy = (int)(random / width);
        }
    }
}

void place_linemate(tile_t **map, int nb, int height, int width)
{
    int random = rand() % (height * width);
    int randx = random % width;
    int randy = (int)(random / width);

    while (nb != 0){
        if (map[randy][randx].items[1].count == 0) {
            map[randy][randx].items[1].count += 1;
            nb--;
        }
        else {
            random = rand() % (height * width);
            randx = random % width;
            randy = (int)(random / width);
        }
    }
}

void place_fruit(tile_t **map, int nb, int height, int width)
{
    int random = rand() % (height * width);
    int randx = random % width;
    int randy = (int)(random / width);

    while (nb != 0){
        if (map[randy][randx].items[0].count == 0) {
            map[randy][randx].items[0].count += 1;
            nb--;
        }
        else {
            random = rand() % (height * width);
            randx = random % width;
            randy = (int)(random / width);
        }
    }
}

void place_ressource(tile_t **map, int width, int height)
{
    place_fruit(map, ceil(FRUIT_RATIO * (width * height)), height, width);
    place_linemate(map, ceil(LINEMATE_RATIO * (width * height)), height, width);
    place_deraumere(map, ceil(DERAUMERE_RATIO *
    (width * height)), height, width);
    place_sibur(map, ceil(SIBUR_RATIO * (width * height)), height, width);
    place_mendiane(map, ceil(MENDIANE_RATIO * (width * height)), height, width);
    place_phiras(map, ceil(PHIRAS_RATIO * (width * height)), height, width);
    place_thystame(map, ceil(THYSTAME_RATIO * (width * height)), height, width);
}
