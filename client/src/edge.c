#include<stdio.h>
#include "edge.h"
#include <stdlib.h>
#include<math.h>
typedef struct Point {
    int x;
    int y;
} Point;
//create a struct called edge. Each edge
typedef struct Edge {
    int id;
    Point begin;
    Point end;
    int player;
} Edge;
//returns the left most point
Point right_point(Point one, Point two){
    if(one.x>two.x){
        return one;
    }
    else{
        return two;
    }
}
Point left_point(Point one, Point two){
    if(one.x<=two.x){
        return one;
    }
    else{
        return two;
    }
}
//first player has 1 and second player has 2
//we will have a list of total edges and a list of edges of each player


int overlap_edges(Edge *total, Point begin2, Point end2, int length) {
    //how to check overlap
    //loop through all the edges in total
    //check for this. We need begin and end, y positions to cross
    //so first figure out the orientiation of the edge. We identify the middle position of the edge
    //we need both the x and y to cross the middle of the edge.
    //loop through each edge to figure that out
    float middle_x, middle_y;

    float gradient_1 = (float)((float)left_point(begin2, end2).y-(float)right_point(begin2, end2).y)/((float)left_point(begin2, end2).x- (float)right_point(begin2, end2).x);
    middle_x = (float)(begin2.x +end2.x)/2;
    middle_y=(float)(begin2.y+end2.y)/2;
    for(int i=0; i<length; i++) {

        Point begin = total[i].begin;
        Point end = total[i].end;
        float gradient_2 = (float)((float)left_point(begin, end).y-(float)right_point(begin, end).y)/((float)left_point(begin, end).x- (float)right_point(begin, end).x);
        //firstc case. Begin is at the top right of the middle
        if(gradient_1==gradient_2){
           // printf("gradient is %f\n", gradient_1);
            
        }else{
        if(begin.x>=middle_x&&begin.y>=middle_y) {
            if(end.x<=middle_x&&end.y<=middle_y) {
                
                return 0;
            }
        }
        //top left
        if(begin.x<=middle_x&&begin.y>=middle_y) {
            if(end.x>=middle_x&&end.y<=middle_y) {
                return 0;
            }
        }
        //bottom right
        if(begin.x>=middle_x&&begin.y<=middle_y) {
            if(end.x<=middle_x&&end.y>=middle_y) {
                return 0;
            }

        }
        //bottom left
        if(begin.x<=middle_x&&begin.y<=middle_y) {
            if(end.x>=middle_x&&end.y>=middle_y) {
                return 0;
            }
        }

    }
    }

    return 1;
}


//checks if a particular point belongs to an edge or not.
int check_point(int x, int y, int (*grid)[24], Edge *total, int length) {
    for(int i =0; i<length; i++) {
        if(total[i].begin.x==x&&total[i].begin.y==y) {
            // printf("FOUND EDGE\n");
            return 1;
        }
        if(total[i].end.x==x&&total[i].end.y==y) {
            //printf("FOUND EDGE \n");
            return 1;
        }
    }
//printf("%d\n", 0);
    return 0;

}
//check if edge is possible or not
//check if the grid point on the required points are allowed or not
//check if point nearby has the same turn or not
//check if edge exists or not
//add to edge
//assume it has already been checked if the point is valid or not
int new_point(Edge *total, Point current, int (*grid)[24], int turn, int *length) {
    //go through all nearby points and add edges
    int conditions[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, 2}, {1, -2}, {2, 1}, {2, -1}};
    for(int i =0; i<8; i++) {
        int x = current.x+conditions[i][0];
        int y = current.y+conditions[i][1];
        if(x>=0&&x<24&&y>=0 &&y<24) {
            if(grid[x][y]==turn) {
                if(overlap_edges(total, current, (Point) {
                x, y
            }, *length)==0) {
                    continue;
                }
                int id = *length+1;
                Edge new = {.begin = (Point){x, y}, .end = current, .id = id,.player = turn };
                total[*length] = new;
                *length = *length +1;

            }
        }
    }
    return 0;
}



int win_first(Edge *total, int (*grid)[24], int length) {
    Point current, next;
    int id=0;
    int considered[1000000];
    int answer = 0;
    int consider_length=0;
//in last row, go throug for(int i =0;i<length;i++){h each element. For each one, find the next
    for(int i =0; i<24; i++)
    {
        if(grid[23][i]==1) {
            for(int j =0; j<length; j++) {

                if(total[j].begin.x==23&&total[j].begin.y==i) {
                    // printf("FOUND EDGE\n"); for(int i =0;i<length;i++){
                    next = total[j].end;
                    id = total[j].id;
                    considered[0] = id;
                    consider_length=1;

                }
                else if(total[j].end.x==23&&total[j].end.y==i) {
                    //printf("FOUND EDGE \n");
                    next = total[j].begin;
                    id = total[j].id;
                    considered[0] = id;
                    consider_length=1;

                }
                else {
                    continue;
                }
                //use recursive function to consider all possibilities

                answer =  recursive(next, considered, consider_length, total, length,1);
                if(answer>0) {
                    return 1;
                }
            }




        }


    }
    return 0;

}


int win_second(Edge *total, int (*grid)[24], int length) {
    Point current, next;
    int id=0;
    int considered[100000];
    int answer = 0;
    int consider_length=0;
//in last row, go throug for(int i =0;i<length;i++){h each element. For each one, find the next
    for(int i =0; i<24; i++)
    {
        if(grid[i][23]==2) {
            for(int j =0; j<length; j++) {

                if(total[j].begin.y==23&&total[j].begin.x==i) {
                    // printf("FOUND EDGE\n"); for(int i =0;i<length;i++){
                    next = total[j].end;
                    id = total[j].id;
                    considered[0] = id;
                    consider_length=1;

                }
                else if(total[j].end.y==23&&total[j].end.x==i) {
                    //printf("FOUND EDGE \n");
                    next = total[j].begin;
                    id = total[j].id;
                    considered[0] = id;
                    consider_length=1;

                }
                else {
                    continue;
                }
                //use recursive function to consider all possibilities

                answer =  recursive(next, considered, consider_length, total, length, 2);
                if(answer>0) {
                    return 1;
                }
            }




        }


    }
    return 0;

}



int recursive(Point curr, int considered[], int consider_length, Edge *total, int length, int turn) {
    if(turn ==1) {
        if(curr.x==0) {
            return 1;
        }
    }
    else {
        if(curr.y==0) {
            return 1;
        }
    }
    int answer= 0;

    for(int i =0; i<length; i++ ) {
        int flag = 0;
        for(int j=0; j<consider_length; j++) {
            if(considered[j] == total[i].id) {
                flag =1;
            }
        }
        if(flag) {
            continue;
        }
        if(total[i].begin.x == curr.x &&total[i].begin.y==curr.y) {


            considered[consider_length] = total[i].id;
            answer+=recursive(total[i].end, considered, consider_length+1, total, length, turn);
        }
        else if(total[i].end.x==curr.x&&total[i].end.y==curr.y) {
            considered[consider_length] = total[i].id;
            answer+=recursive(total[i].begin, considered, consider_length+1, total, length, turn);
        }
    }
    return answer;
}
