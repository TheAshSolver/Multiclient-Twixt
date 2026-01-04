#include "input.h"
#include<stdio.h>
#include<stdlib.h>
#include "edge.h"


//if given pointb is not valid, return -2, if given point already has been marked, return -1
//if given point not valid boundary at given turn, return 0
//if possible return 1;
int check_input(int (*grid)[24], int x, int y, int turn) {
    //first check if the points have already been marked
    if(y>23|| y<0|| x<0 || x>23) {
        return -2;
    }
    if(grid[x][y]!=0) {
        return -1;
    }
    //if turn is red
    if(turn==1) {
        if(y>=23 || y<=0) {
            return 0;
        }


    }
    else {
        if(x<=0||x>=23) {
            return 0;
        }
    }

    return 1;



}





