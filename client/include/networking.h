#ifndef NET
#define NET

int connection(int my_socket, int game_id, int *turn);
int create_socket(char *ip);
int send_points(int x, int y, int my_socket);
int receive_points(int my_socket, int *x, int *y);

#endif