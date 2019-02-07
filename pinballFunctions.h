#ifndef PINBALLFUNCTIONS_H_INCLUDED
#define PINBALLFUNCTIONS_H_INCLUDED

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>


#endif // PINBALLFUNCTIONS_H_INCLUDED


 /* Structure to hold ball object */
struct ball {
int x;
int y;

 int xDirection;
 int yDirection;
 int next_x;
 int next_y;
};


/*Structure to hold player object */
struct player{
    unsigned long score;
    char name[20];
    char lives;
    int level;
    int color;
};

