#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<poll.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<wait.h>
#include<unistd.h>
#include<sys/types.h>

#define PORT "3490"
#define MAXCLIENT 300
#define MAXGAME 150


//have a linked list of games
//have a linked list of clients.

typedef struct Game {
    int game_id;
    int number_participants;
    int red_player_fd;
    int black_player_fd;
    struct Game *next;
    struct Game *prev;
} Game;


typedef struct Client {
    int socket;
    int game_id;
    Game *game;
    struct Client *next;
    struct Client *prev;

} Client;



Game *head_game= NULL;
Client *client_head= NULL;
struct pollfd connections[MAXCLIENT+1];
int num_connections;

int first_socket() {
    int status, my_socket;
    struct addrinfo hint, *servinof, *curr;
    memset(&hint,0, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_flags = AI_PASSIVE;
    hint.ai_socktype = SOCK_STREAM;

    if((status = getaddrinfo(NULL, PORT, &hint, &servinof ))!=0) {
        perror("Error on addrinfo");
        exit(1);
        return -1;
    }
    for(curr = servinof; curr!=NULL; curr=curr->ai_next) {
        my_socket = socket(curr->ai_family, curr->ai_socktype, curr->ai_protocol);
        if(my_socket==-1) {
            continue;
        }
        int yes = 1;
        if(setsockopt(my_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1) {
            perror("sock opt");
            exit(1);
        }

        if(bind(my_socket, curr->ai_addr, curr->ai_addrlen)==-1) {
            continue;
        }
        break;




    }
    freeaddrinfo(servinof);
    if(curr==NULL) {
        perror("no more valid addrinfo");
        exit(1);
        return -1;
    }

    listen(my_socket,10);



    return my_socket;







}


//handling a new client

int handle_new_client(int listener) {

    struct sockaddr_storage client_info;
    socklen_t client_info_size = sizeof(client_info);
    int new_fd = accept(listener, (struct sockaddr *)&client_info, &client_info_size);

    char ipm[INET6_ADDRSTRLEN];
    struct sockaddr_in *client_ip4;
    struct sockaddr_in6 *client_ip6;
    if(client_info.ss_family==AF_INET) {
        client_ip4 = (struct sockaddr_in *)&client_info;
        inet_ntop(AF_INET, &client_ip4->sin_addr.s_addr, ipm, INET_ADDRSTRLEN );


    }
    else {
        client_ip6 = (struct sockaddr_in6 *)&client_info;
        inet_ntop(AF_INET6, client_ip6->sin6_addr.s6_addr, ipm, INET6_ADDRSTRLEN );

    }

    printf("New client is attempting to join with ip address %s\n", ipm);
    char room_code[1000];
    //check if client has sent a game code
    int n = recv(new_fd, room_code, sizeof(room_code)-1, 0);
    if(n<=0){
        printf("Client has been rejected\n");
        send(new_fd, "R", 2, 0);
        return 0;
    }
    else {
        printf("Received room number from client \n");
    }
    int game_id;
    if(sscanf(room_code, "%d", &game_id)!=1) {
        printf("Client rejected for giving invalid game id\n");
        send(new_fd, "R", 2, 0);
        return 0;
    }
    //check if game exists
    Game *curr= head_game;
    while(curr!=NULL&&curr->game_id!=game_id) {
        curr= curr->next;
    }
    //accept client and give red role
    if(curr==NULL) {
        if(send(new_fd, "F", 2, 0)==-1) {
            printf("Client connection failed\n");
            return 0;
        }
        else {
            printf("Role F given to client\n");
        }

        Game *new_game = malloc(sizeof(Game));
        new_game->game_id = game_id;
        if(head_game==NULL) {
            head_game = new_game;
            new_game->prev = NULL;
        }
        else {
            curr = head_game;
            while(curr->next!=NULL) {
                curr= curr->next;
            }
            curr->next = new_game;
            new_game->prev = curr;

        }
        new_game->next = NULL;

        //make new client and add to clients linked list
        Client *new_client = malloc(sizeof(Client));
        new_client->socket = new_fd;
        new_client->game_id = game_id;
        new_client->game = new_game;
        new_client->next = NULL;
        if(client_head==NULL) {
            client_head = new_client;
            new_client->prev = NULL;
        }
        else {
            Client *now = client_head;
            while(now->next!=NULL) {
                now = now->next;
            }
            now->next = new_client;
            new_client->prev = now;
        }

        //combine
        new_game->black_player_fd = -1;
        new_game->red_player_fd = new_client->socket;
        new_game->number_participants =1;

    }
    else {
        //accept client with this game id and black role
        if(curr->number_participants==1) {
            if(send(new_fd, "S", 2, 0)==-1) {
                printf("Client connection failed\n");
                return 0;
            }
            else {
                printf("Role S given to client\n");
            }
            Client *new_client = malloc(sizeof(Client));
            new_client->socket = new_fd;
            new_client->game_id = game_id;
            new_client->game = curr;
            new_client->next = NULL;
            if(client_head==NULL) {
                client_head = new_client;
                new_client->prev = NULL;
            }
            else {
                Client *now = client_head;
                while(now->next!=NULL) {
                    now = now->next;
                }
                now->next = new_client;
                new_client->prev = now;
            }
            //change game
            curr->black_player_fd = new_client->socket;
            curr->number_participants= 2;
            if(send(curr->red_player_fd, "B", 2, 0)==-1) {
                printf("Error");
                exit(1);
            }
            printf("Sent begin to F\n");
            if(send(curr->black_player_fd, "B", 2, 0)==-1) {
                printf("Error");
                exit(1);
            }
            printf("Sent begin to S\n");
        }
        //reject client because room is full
        else {
            printf("Client rejected because game is full\n");
            send(new_fd, "R", 2, 0);
            return 0;
        }
    }

    connections[num_connections].fd= new_fd;
    connections[num_connections].events = POLLIN;
    num_connections++;


    printf("Client successfully joined\n");

    return 1;
}



//removing a client
void remove_client(int fd) {

    for(int i =0; i<num_connections; i++)
    {
        if(connections[i].fd ==fd) {
            connections[i] = connections[num_connections-1];
            num_connections--;
            break;
        }
    }
    //remove client from linked list.
    Client *curr = client_head;
    Game *delete_game= NULL;
    while(curr!=NULL) {
        if(curr->socket==fd||(delete_game!=NULL&&(curr->socket==delete_game->black_player_fd||curr->socket==delete_game->red_player_fd))) {
            if(curr->prev==NULL) {
                client_head = curr->next;
                if(client_head!=NULL) {
                    client_head->prev = NULL;
                }
            } else {
                curr->prev->next = curr->next;
                delete_game = curr->game;


            }

        }
        curr = curr->next;
    }
    if(delete_game!=NULL) {
        if (delete_game->prev&&delete_game->next)
            delete_game->prev->next = delete_game->next;
        else
            head_game = delete_game->next;

        if (delete_game->next&&delete_game->prev)
            delete_game->next->prev = delete_game->prev;

    
    //tell other player that they won.
    if(delete_game->black_player_fd==fd) {

        if(delete_game->red_player_fd!=-1&&send(delete_game->red_player_fd, "-1 -1", 6, 0)==-1) {
            printf("error");
            return;
        }
    }
    else {
        if(delete_game->black_player_fd!=-1&&send(delete_game->black_player_fd, "-1 -1", 6, 0)==-1) {
            printf("error");
            return;
        }
    }
    if(delete_game->black_player_fd!=-1)
        shutdown(delete_game->black_player_fd, SHUT_RDWR);
    if(delete_game->red_player_fd!=-1)
        shutdown(delete_game->red_player_fd, SHUT_RDWR);
    free(delete_game);

    }

}




//handling regular data
//when use receive input, from one participant, you receive it and send it to his opponent.
void handle_regular_data(int fd) {
    char message[100];
    int status = recv(fd, message, 100, 0);
    if(status<=0) {
        printf("shutting down client\n");
        remove_client(fd);
        return;
    }
    int x, y;
    if(sscanf(message, "%d %d", &x, &y)!=2) {
        printf("shutting down client for wrong input\n");
        remove_client(fd);
        return;
    }
    //send to opponent player
    Client *curr = client_head;
    while(curr!=NULL) {
        if(curr->socket==fd) {
            Game *game = curr->game;
            if(game->black_player_fd==fd) {
                if(game->red_player_fd!=-1) {
                    if(send(game->red_player_fd, message, strlen(message)+1, 0)==-1) {
                        remove_client(fd);
                        printf("Sending error");
                    }
                }
            } else {
                if(game->black_player_fd!=-1) {
                    if(send(game->black_player_fd, message, strlen(message)+1, 0)==-1) {
                        remove_client(fd);
                        printf("Sending error");
                    }
                }
            }
            break;
        }
        curr = curr->next;

    }



}






//handle everything
void handle_everything(int listener) {
    for(int i=0; i<num_connections; i++) {
        if(connections[i].revents&POLLIN) {
            if(connections[i].fd==listener) {
                handle_new_client(listener);
            }
            else {
                handle_regular_data(connections[i].fd);
            }
        }
    }











}







int main() {
    int listener = first_socket();
    connections[0].fd = listener;
    connections[0].events = POLLIN;
    num_connections = 1;

    while(1) {
        int status = poll(connections, num_connections, -1);
        if(status<0) {
            continue;
        }
        handle_everything(listener);
    }

    return 0;
}