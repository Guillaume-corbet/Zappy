/*
** EPITECH PROJECT, 2020
** Visual Studio Live Share (Workspace)
** File description:
** cut_str
*/

#include "server.h"

int arraylen(char **array)
{
    int i = 0;

    if (!array)
        return (0);
    while (array[i])
        i++;
    return (i);
}

void destroy_array(char **array)
{
    for (int i = 0; i < arraylen(array); i++) {
        free(array[i]);
    }
    free(array);
}

int count_space(char *str, char c)
{
    int count = 1;
    int i = 0;

    if (strlen(str) == 0)
        return (1);
    while (str[i]) {
        if (str[i] == c)
            count++;
        i++;
    }
    return (count);
}

char **cut_str(char *str, char c)
{
    int size = 0;
    char **res;
    int nb = 0;

    if ((res = malloc(sizeof(char *) * (count_space(str, c) + 1))) == NULL)
        return (NULL);
    while (str && str[size]) {
        while (str[size] != c && str[size])
            size++;
        if ((res[nb] = malloc(sizeof(char) * size + 1)) == NULL)
            return (NULL);
        for (int k = 0; k < size; k++)
            res[nb][k] = str[k];
        res[nb][size] = 0;
        while ((str[size] == c) && str[size])
            size++;
        str += size;
        size = 0;
        nb++;
    } res[nb] = NULL;
    return (res);
}

char **ncut_str(char *str, char c, int max)
{
    int size = 0;
    char **res;
    int nb = 0;

    res = malloc(sizeof(char *) * (max + 1));
    while (str[size]) {
        while ((str[size] != c || nb == max - 1) && str[size])
            size++;
        res[nb] = malloc(sizeof(char) * size + 1);
        for (int k = 0; k < size; k++)
            res[nb][k] = str[k];
        res[nb][size] = 0;
        while ((str[size] == c) && str[size])
            size++;
        str += size;
        size = 0;
        nb++;
    } res[nb] = NULL;
    return (res);
}