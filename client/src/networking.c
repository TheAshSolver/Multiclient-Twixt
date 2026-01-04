#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<unistd.h>
#include<netdb.h>
#include<string.h>
#include<wait.h>
#include<errno.h>



int create_socket(char *ip){
    struct addrinfo hints, *servinfo, *curr;
    int my_socket, yes=1;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    int status = getaddrinfo(ip, "3490", &hints, &servinfo);
    if(status!=0){
        printf("error");
        exit(1);
    }
    for(curr = servinfo;curr!=NULL;curr= curr->ai_next){
        if((my_socket = socket(curr->ai_family, curr->ai_socktype, curr->ai_protocol))==-1){
            continue;
        }
        if(setsockopt(my_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1){
            printf("Error");
            exit(1);
        }
        if(connect(my_socket, curr->ai_addr, curr->ai_addrlen)==-1){
            continue;
        }
        break;
        

    }
    if(curr==NULL){
        printf("Error\n");
        exit(1);
        return -1;
    }
    return my_socket;





}

//method used to connect to the server.
int connection(int my_socket, int game_id, int *turn){
    //first step is to send the client, the game_id
    char game_i[100];
    sprintf(game_i, "%d", game_id);
    if(send(my_socket, game_i, strlen(game_i)+1, 0)==-1){
        printf("Sending error\n");
        exit(1);
        return -1;
    }
    char first_reply[100];
    int n = recv(my_socket, first_reply, 2, MSG_WAITALL);
    printf("first reply is %s\n", first_reply);
    if(n<=0){
        printf("Error in first reply");
        exit(1);
        return -1;
    }
    if(first_reply[0]=='R'){
        printf("Connection Rejected\n");
        exit(1);
        return -1;

    }
    if(first_reply[0]=='F'){
        *turn = 1;
    }
    else if(first_reply[0]=='S'){
        *turn = 2;
    }
    char begin_reply[100];
    int n2 = recv(my_socket, begin_reply, 2, MSG_WAITALL);
    printf("%s\n", begin_reply);
    if(n2<=0){
        printf("Receiving begin command error\n");
        exit(1);
    }
    if(begin_reply[0]!='B'){
        printf("NOT B");
        exit(1);
    }
    return 1;
    

}


int send_points(int x, int y, int my_socket){
    char message[100];
    sprintf(message, "%d %d", x,y);
    if(send(my_socket, message, strlen(message)+1, 0)==-1){
        printf("Error sending points");
        exit(1);
        return 0;
    }

}        
int receive_points(int my_socket, int *x, int *y){
    char message[100];
    int n = recv(my_socket, message, sizeof(message), 0);
    if(n<=0){
        printf("receiving error");
        exit(1);
        return 0;
        
    }
    printf("%s\n", message);
    if(sscanf(message,"%d %d", x, y )!=2){
        printf("parsing error");
        exit(1);
        return 0;
    }

    
}



