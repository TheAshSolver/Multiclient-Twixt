#include<stdio.h>
#include "grid.h"
#include "edge.h"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"
#define BLUE "\x1b[34m"
int print_grid(int (*grid)[24], int length, Edge *total, int length2) {

    for(int i =0; i<length; i++) {
        if(i==0) {
            printf("  ");
            for(int k=0; k<length; k++) {
                if(k==1||k==23) {
                    printf("  ");
                }
                if(k<10)
                    printf(" %d ", k);
                else
                    printf(" %d", k);
            }
            printf("\n");
        }
        if(i==length-1) {
            printf("  ");
            for(int k =0; k<length; k++) {
                printf("———");
            }
        }
        if(i==length-1) {
            printf("\n");
        }
        if(i<10) {
            printf("%d  ",i);
        }
        else  printf("%d ",i);
        for(int j =0; j<length; j++) {
       
            if(j==length-1) {
                printf("|  ");
            }     
            if((i==0&&j==0)||(i==0&&j==23)||(i==23&&j==23)||(i==23&&j==0)){printf("   ");}
                  else{
            if(grid[i][j]==1) {
                if(check_point(i, j, grid, total, length2)==1) {
                    printf(RED "F  "RESET);

                }
                else {
                    printf("F  ");
                }

            }
            else if(grid[i][j]==0) {
                printf(".  ");
            }
            else {
                if(check_point(i, j, grid, total, length2)==1) {
                    printf(BLUE "S  "RESET);

                }
                else {
                    printf("S  ");
                }

            }
        }
            if(j==0) {
                printf("| ");
            }
        
        }



        printf("\n");

        if(i==0) {
            printf("   ");
            for(int k =0; k<length; k++) {
                printf("———");

            }

        }
        if(i==0) {
            printf("\n");
        }

        printf("\n");

    }
    return 1;
}

