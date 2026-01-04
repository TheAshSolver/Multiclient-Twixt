#ifndef EDGE
#define EDGE

typedef struct Point {
    int x;
    int y;
} Point;
typedef struct Edge {
    int id;
    Point begin;
    Point end;
    int player;
} Edge;

int new_point(Edge *total, Point current, int (*grid)[24], int turn, int *length);
int overlap_edges(Edge *total, Point begin2, Point end2, int length);
int check_point(int x, int y, int (*grid)[24], Edge *total, int length);
int new_point(Edge *total, Point current, int (*grid)[24], int turn, int *length);
int win_first(Edge *total, int (*grid)[24], int length) ;
int win_second(Edge *total, int (*grid)[24], int length) ;
int recursive(Point curr, int considered[], int consider_length, Edge *total, int length, int turn);

#endif 