/*
** EPITECH PROJECT, 2020
** PSU_zappy_2019
** File description:
** map
*/

#include "server.h"

void copy_item(tile_t *new, tile_t const *ori)
{
    for (size_t i = 0; i < 7; i++){
        new->items[i].count = ori->items[i].count;
        new->items[i].name = my_strdup(ori->items[i].name);
        new->items[i].token = ori->items[i].token;
    }
}

tile_t **make_copy(map_t const *original)
{
    tile_t **newer = malloc(sizeof(tile_t *) * original->height);

    for (size_t i = 0; i < original->height; i++){
        newer[i] = malloc(sizeof(tile_t) * original->width);
        for (size_t f = 0; f < original->width; f++){
            copy_item(&(newer[i][f]), &(original->origin[i][f]));
        }
    }
    return (newer);
}