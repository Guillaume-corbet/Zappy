/*
** EPITECH PROJECT, 2020
** Visual Studio Live Share (Workspace)
** File description:
** string_manip
*/

#include "server.h"

int my_strlen(char *str)
{
    int i = 0;

    while (str && str[i])
        i++;
    return (i);
}

char *my_strdup(char *src)
{
    char *dest = malloc(sizeof(char) * my_strlen(src) + 1);

    if (!src) {
        dest[0] = 0;
        return (dest);
    }
    for (unsigned int i = 0; i < strlen(src); i++) {
        dest[i] = src[i];
    }
    dest[strlen(src)] = 0;
    return (dest);
}

char *my_strndup(char *src, unsigned int nb)
{
    char *dest = malloc(sizeof(char) * nb + 1);
    unsigned int i = 0;

    while (i < strlen(src) && i < nb) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
    return (dest);
}

char *my_strcat(char *str1, char *str2, int check)
{
    int i = 0;
    int j = 0;
    char *new = malloc(sizeof(char) * my_strlen(str1) + my_strlen(str2) + 1);

    while (str1 && str1[i]) {
        new[i] = str1[i];
        i++;
    }
    while (str2 && str2[j]) {
        new[i] = str2[j];
        i++;
        j++;
    }
    new[i] = 0;
    if (check % 2 == 1 && str1 && strlen(str1) != 0)
        free(str1);
    if (check >= 2 && str2 && strlen(str2) != 0)
        free(str2);
    return (new);
}

char *read_fic(char *name)
{
    int fic = open(name, O_RDONLY);
    int len = 10;
    char *buff = malloc(sizeof(char) * len + 1);
    char *res = my_strdup("");

    if (fic == -1)
        return (NULL);
    while ((len = read(fic, buff, 10)) > 0) {
        buff[len] = 0;
        res = my_strcat(res, buff, 3);
        buff = malloc(sizeof(char) * 11);
        memset(buff, 0, 10);
    }
    free(buff);
    close(fic);
    return (res);
}