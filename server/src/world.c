/*
** EPITECH PROJECT, 2020
** PSU_zappy_2019
** File description:
** world
*/

#include "server.h"

void free_items(tile_t *tile)
{
    for (size_t j = 0; j < 7; j++) {
        free(tile->items[j].name);
    }
}

void destroy_map(map_t *array, int fullclear)
{
    int i = 0;
    int f = 0;

    while (i < array->height) {
        while (f < array->width) {
            free_items(&(array->map[i][f]));
            f++;
        }free(array->map[i]);
        i++;
        f = 0;
    }free(array->map);
    if (fullclear == 1){
        for (i = 0; i < array->height; i++) {
            while (f < array->width) {
                free_items(&(array->origin[i][f]));
                f++;
            }free(array->origin[i]);
            f = 0;
        }free(array->origin);
        free(array);}
}

void initialise_items(tile_t *tile)
{
    for (size_t i = 0; i < 7; i++) {
        tile->items[i].count = 0;
        tile->items[i].name = strdup(placeable_name[i]);
        tile->items[i].token = placeable_name[i][0];
    }
}

tile_t **fill_map(int width, int height)
{
    tile_t **map = malloc(sizeof(tile_t*) * (height));
    int f = 0;
    int i = 0;

    while (i < height) {
        map[i] = malloc(sizeof(tile_t) * (width));
        while (f < width) {
            initialise_items(&(map[i][f]));
            f++;
        }
        f = 0;
        i++;
    }
    return (map);
}

tile_t **generation(int width, int height)
{
    tile_t **map;
    int i = 0;
    int f = 0;

    printf("Generating the world... Please Wait...\n");
    map = fill_map(width, height);
    place_ressource(map, width, height);
    printf("Generation complete.\nLaunching server...\n");
    return (map);
}