#include<stdio.h>
#include<stdlib.h>
#include "edge.h"
#include "grid.h"
#include "input.h"
#include "networking.h"

#define RED "\x1b[31m"
#define RESET "\x1b[0m"
#define BLUE "\x1b[34m"
#define PORT "3490"
#define IP "127.0.0.1"






int main(int argc, char *argv[]) {
    int my_turn= 1;
    int current_turn =1;

    //starting setup. Accept the game number through the command line
    int game_id;
    scanf("%d", &game_id);
    int my_socket = create_socket(IP);
    connection(my_socket, game_id, &my_turn);
    int grid[24][24]= {};
    int x, y;
    Edge *total = calloc(1000000,sizeof(Edge));

    int length = 0;
    print_grid(grid, 24, total, length);
    printf("%d\n", my_turn);
    while(1){
       
        if(my_turn==current_turn){
            //accept user input
            if(current_turn==1){
            printf(RED"First player move ->"RESET);
        }
        else{
            printf(BLUE"Second player move->"RESET);
        }
        while(scanf("%d %d", &x, &y)!=2){
            printf("Please enter a pair of integers!\n");
            if(current_turn==1){
            printf(RED"First player move ->"RESET);
        }
        else{
            printf(BLUE"Second player move->"RESET) ;
        }
         while(getchar()!='\n');   
        };
                int flag;
        while(( flag = check_input(grid, x,y, my_turn))<=0) {
            switch (flag)
            {
            case -2:
                /* code */
                printf("Give within boundary \n");
                 if(my_turn==1){
                    printf(RED"First player move ->"RESET);
                }
                else{
                 printf(BLUE"Second player move->"RESET);
             }
                break;

            case -1:
                printf("Point already marked\n");
                   printf("Give within boundary \n");
                 if(my_turn==1){
                    printf(RED"First player move ->"RESET);
                }
                else{
                 printf(BLUE"Second player move->"RESET);
             }
                break;
            default:
                printf("Not valid for your turn\n");
                   printf("Give within boundary \n");
                 if(my_turn==1){
                    printf(RED"First player move ->"RESET);
                }
                else{
                 printf(BLUE"Second player move->"RESET);
             }
                break;
            }
            while(scanf("%d %d", &x, &y)!=2){
                   printf("Give within boundary \n");
                 if(my_turn==1){
                    printf(RED"First player move ->"RESET);
                }
                else{
                 printf(RED"Second player move->"RESET);
             }
         while(getchar()!='\n');   
        };

        }
        if(my_turn==1) {
            grid[x][y] = 1;

        }
        else {
            grid[x][y] = 2;
        }
        new_point(total,(Point) {
            x,y
        }, grid, my_turn, &length );
        if(current_turn ==1) {
            current_turn = 2;
        }
        else current_turn =1;
        send_points(x,y, my_socket);
        print_grid(grid, 24, total, length);
     
    }




        
        else{
        
            //wait for server response
            printf("Waiting for opponents move:\n");
            receive_points(my_socket, &x, &y);
            if(x==-1 && y==-1){
                if(my_turn==1 ){
                    printf("FIRST PLAYER WON!!!!\n");
                }
                else{
                    printf("SECOND PLAYER WON!!!");
                }
            }
            if(current_turn==1){
                printf(RED"Opponent's moves: %d %d\n"RESET, x,y);
                grid[x][y] = 1;
                new_point(total,(Point) {
            x,y
        }, grid, current_turn, &length );
                current_turn =2;

            }
            else{
                printf(BLUE"Opponent's moves: %d %d\n"RESET,x ,y );
                grid[x][y] =2;
                new_point(total,(Point) {
            x,y
        }, grid, current_turn, &length );
                current_turn = 1;
            }
            
            
            
            print_grid(grid, 24, total, length );

        }
        if(win_first(total, grid, length)>=1) {
            printf("FIRST PLAYER WON!!!");
            break;
        }
        if(win_second(total, grid, length)>=1) {
            printf("SECOND PLAYER WON!!!");
            break;
        }





    }

    











    return 0;
}