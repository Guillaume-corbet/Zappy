/*
** EPITECH PROJECT, 2020
** PSU_zappy_2019
** File description:
** server
*/

#ifndef SERVER_H_
#define SERVER_H_
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <math.h>

#ifndef MACRO_
#define MACRO_

#define INC_TIMER 100

#define FRUIT_RATIO 0.2
#define LINEMATE_RATIO 0.12
#define DERAUMERE_RATIO 0.10
#define SIBUR_RATIO 0.13
#define MENDIANE_RATIO 0.07
#define PHIRAS_RATIO 0.07
#define THYSTAME_RATIO 0.02

#define OK "ok\n"
#define KO "ko\n"

#define INV "[food %i,linemate %i,deraumere %i,sibur %i,mendiane %i,phiras \
%i,thystame %i]\n"
#define NEW_IA "pnw %i %i %i %i %i %s\n"
#define NEW_PLAYER "%i\n%i %i\n"
#define BROAD "message %i, %s\n"
#define INCANTATION "Elevation underway Current level: %i"
static char placeable_name[7][10]= {"Food", "Linemate", "Deraumere", "Sibur",
"Mendiane", "Phiras", "Thystame"};

#define INCANT (!strncmp(tab[i], "Incantation", 11) && cur->ia) ?\
(incantation_notif(&cur)) : (0)

#endif /* !MACRO_ */

#ifndef STRUCT_
#define STRUCT_

typedef struct placeable_s
{
    char token;
    char *name;
    int count;
}placeable_t;

typedef struct tile_s
{
    placeable_t items[7];
}tile_t;

typedef struct action_s
{
    char *cmd;
    struct action_s *next;
}action_t;

typedef struct queue_s
{
    int count;
    action_t *front;
    action_t *tail;
}queue_t;

typedef struct IA_s
{
    struct timeval *next_action;
    struct timeval *life;
    int posx;
    int posy;
    int level;
    int number;
    int incan;
    char *team_name;
    int orientation;
    queue_t *queued;
    placeable_t inv[7];
}IA_t;

typedef struct graphic_s
{
    char *follow;
}graphic_t;

typedef struct client_s
{
    int fd;
    IA_t *ia;
    graphic_t *graph;
    struct client_s *next;
    struct client_s *prev;
}client_t;

typedef struct map_s
{
    tile_t **map;
    tile_t **origin;
    int width;
    int height;
}map_t;

typedef struct event_s
{
    char *evt;
    void (*func)(char *cmd, client_t *cli, map_t *map);
}t_event;

typedef struct event_graph
{
    char *evt;
    void (*func)(char *cmd, client_t *cli);
}t_graph;

typedef struct time_s
{
    char *evt;
    int val;
}t_time;

typedef struct team_s
{
    char *name;
    int count;
    int max;
    struct team_s *next;
    struct team_s *prev;
}team_t;

typedef struct args_s
{
    int port;
    int x;
    int y;
    int client_nb;
    int freq;
}args_t;

static t_time timer_cmd[] =
{
    {"Forward", 7},
    {"Right", 7},
    {"Left", 7},
    {"Look", 7},
    {"Inventory", 1},
    {"Broadcast ", 7},
    {"Connect_nbr", 0},
    {"Fork", 42},
    {"Eject", 7},
    {"Take ", 7},
    {"Set ", 7},
    {"Incantation", 300}
};

static size_t needs_for_incantation[7][7] =
{
    {1, 1, 0, 0, 0, 0, 0},
    {2, 1, 1, 1, 0, 0, 0},
    {2, 2, 0, 1, 0, 2, 0},
    {4, 1, 1, 2, 0, 1, 0},
    {4, 1, 2, 1, 3, 0, 0},
    {6, 1, 2, 3, 0, 1, 0},
    {6, 2, 2, 2, 2, 2, 1}
};

#endif /* !STRUCT_ */

#ifndef INC_
#define INC_

/* src/parse.c */

void manage_arg(char last, char *str, args_t *args);
args_t *init_args(void);
char set_last(char *str, args_t *args);
void set_team_size(team_t **teams, int nb);
args_t *parse(int ac, char **av, team_t **teams);

/* src/string_manip.c */

int my_strlen(char *str);
char *my_strdup(char *src);
char *my_strndup(char *src, unsigned int nb);
char *my_strcat(char *str1, char *str2, int check);
char *read_fic(char *name);

/* src/resources_cmd.c */

void eject(char *cmd, client_t *cli, map_t *map);
void change_inv_value(map_t *map, client_t *cli, int index, int value);
void take(char *cmd, client_t *cli, map_t *map);
void set(char *cmd, client_t *cli, map_t *map);
void fork_player(char *cmd, client_t *cli, map_t *map);

/* src/graphic.c */

void delete_graphic(graphic_t **to_delete);
graphic_t *create_graphic(char *follow);
void addin_graphic(char *cmd, client_t *cli, map_t *map);

/* src/cut_str.c */

int arraylen(char **array);
void destroy_array(char **array);
int count_space(char *str, char c);
char **cut_str(char *str, char c);
char **ncut_str(char *str, char c, int max);

/* src/world.c */

void free_items(tile_t *tile);
void destroy_map(map_t *array, int fullclear);
void initialise_items(tile_t *tile);
tile_t **fill_map(int width, int height);
tile_t **generation(int width, int height);

/* src/socket.c */

void send_from_socket(int sock, client_t *info, char *buffer, char *app);
char *receive_from_socket(client_t *info);

/* src/look.c */

void update_pos(int *x, int *y, map_t *map);
char *set_tile_cnt(map_t *map, int x, int y, client_t *cli);
char *get_tile_look(map_t *map, int i, int j, client_t *cli);
void look(char *cmd, client_t *cli, map_t *map);
void inventory(char *cmd, client_t *cli, map_t *map);

/* src/client.c */

client_t *find_client_by_fd(int fd, client_t *head);
int is_death(client_t *current, int f);
client_t *remove_client(client_t *to_delete, client_t **head);
client_t *clean_client(client_t *to_delete, client_t **head);
void add_client(int fd, client_t **head);

/* src/map.c */

void copy_item(tile_t *new, tile_t const *ori);
tile_t **make_copy(map_t const *original);

/* src/queue.c */

void pop_action(queue_t **head);
void create_action(char *cmd, queue_t **to_add, client_t *cur, int f);
void add_action(queue_t **to_add, client_t *cur, int f);
void delete_queue(queue_t **to_delete);
queue_t *create_queue(void);

/* src/notifgraph.c */

void plv(char *cmd, client_t *cur);
void pin(char *cmd, client_t *cur);
void incantation_notif(client_t **cli);
void notif_graph(char *cmd, client_t *cur);

/* src/incantation.c */

int have_resources(client_t *cli);
void dec_resources(client_t *cli);
void set_all_client_state(client_t *cli);
int count_incanting_players(client_t *cli);
void incantation(char *cmd, client_t *cli, map_t *map);

/* src/ia.c */

void clean_ia(IA_t **to_delete);
void set_values_ia(IA_t **ia, char *team_name, int f);
IA_t *create_ia(int posx, int posy, char *team_name, int f);
void addin_ia(team_t *teams, client_t *cli, map_t *map, int f);

/* src/main.c */

void inc_timer(struct timeval *timer, int value, int f);
void set_signal_handling(void);
int main(int ac, char **av);

/* src/signal.c */

void destroy_clients(client_t **all);
void destroy_teams(team_t **all);
void destroy_all(client_t **clients, map_t **map, team_t **teams, args_t *arg);
int get_signal(int sig);
void signal_handler(int sig);

/* src/team.c */

team_t *find_team_by_name(char *name, team_t **origin);
int add_client_to_team(team_t **team);
void create_team(char *team_name, int max, team_t **origin);

/* src/communication_cmd.c */

char *get_text(char *cmd);
int get_all_dir(client_t *sender, client_t *receiver, int sizex, int sizey);
int update_dir(int dir, int x, int y);
int get_direction(client_t *sender, client_t *receiver, map_t *map);
void broadcast(char *cmd, client_t *cli, map_t *map);

/* src/server.c */

int manage_event_server(client_t *info, char *buffer, map_t *map);
void checkqueue(client_t *cur, client_t **head, map_t **map, int t);
client_t *loop(client_t *cur, client_t **chain, map_t **map, int f);
int manage_new_client(client_t **cur, map_t **map, team_t ** origin, int f);
void run(client_t **chain, map_t **map, team_t **origin, int f);

/* src/graph_loop.c */

int manage_event_graph(client_t *info, char *buffer, map_t **map);
void graph_cut(client_t *info, map_t **map);
client_t *graph_loop(client_t *cur, client_t **chain, map_t **map);

/* src/launching_server.c */

void send_map(int fdconnect, map_t *map);
void new_connection(int socket, client_t **head);
int open_server(int port, map_t *map);
int is_action_valid(char *cmd);
void start(client_t **clients, args_t *argument, map_t **map, team_t **teams);

/* src/place_ressource.c */

void place_mendiane(tile_t **map, int nb, int height, int width);
void place_phiras(tile_t **map, int nb, int height, int width);
void place_thystame(tile_t **map, int nb, int height, int width);

/* src/ressource_manager.c */

void place_sibur(tile_t **map, int nb, int height, int width);
void place_deraumere(tile_t **map, int nb, int height, int width);
void place_linemate(tile_t **map, int nb, int height, int width);
void place_fruit(tile_t **map, int nb, int height, int width);
void place_ressource(tile_t **map, int width, int height);

/* src/movements_cmd.c */

void connect_nbr(char *cmd, client_t *cli, map_t *map);
int move_client(client_t *cli);
void forward(char *cmd, client_t *cli, map_t *map);
void right(char *cmd, client_t *cli, map_t *map);
void left(char *cmd, client_t *cli, map_t *map);

#endif /* !INC_ */

#endif /* !SERVER_H_ */
