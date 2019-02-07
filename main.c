
/*
 ============================================================================
 Name        : pinball.c
 Author      : Tyler Schmidt
 Class       : CS 2060 Sec 01
 Date        : 4/15/2018
 HW          : Chap 17 HW02
 Description : Description: This game was originally titled "ASCII Pinball", but once I got gravity to work I didn't like it.
                So therefore the game was reworked into Anti-gravity "Space pinball

 NOTE: This game is meant to be played in the console size of 120 columns and 30 rows
  ============================================================================
 */

#include "Windows.h"

#include <curses.h>

#define true 1
#include <stdlib.h>
#define false 0
#include <stdio.h>

//#define usleep Sleep

#include <unistd.h>
#include "pinballFunctions.h"



/*Game pieces prototypes */
void makeXWall(int y,int x, int count, struct ball * ball);
void makeYWall(int y,int x, int count, struct ball * ball);
void makeDiagonalWallDown(int y,int x, int count, struct ball * ball);
void makeDiagonalWallUp(int y,int x, int count, struct ball * ball);
void makeGoalPost(int y, int x, long score, struct player * player, struct ball * ball);
void outOfBounds(int y, int x, int count , struct ball * ball, struct player * player);
void makeXBorder(int y,int x, int count);
void makeYBorder(int y,int x, int count);
void drawFlippers(int y, int x, struct ball * ball);

/*Movement prototypes*/
void ballMovementX(struct ball * ball, int max_x);
void ballMovementY(struct ball * ball, int max_y);

/*Menu prototypes*/

void storyMenu();
void controlsMenu();
void pause(struct ball * ball, struct player * player);
void gameOver(struct player * player);
void menu();
 /*Settings prototypes*/
void setCursesSettings();
void initializeColors();
void processColor(char c, struct player * player);

/*Game colors prototypes*/
int setMapColor(struct player *  player);
int setMapBorderColor(struct player *  player);
int setFlipperColor(struct player *  player);

/*Map prototypes*/
void setMapA(struct ball * ball, struct player * player);
void setMapB(struct ball * ball, struct player * player);
void setMapC(struct ball * ball, struct player * player);
void setMapD(struct ball * ball, struct player * player);
void setMapBorders(struct ball * ball, struct player * player, int max_x, int max_y);
void drawStart(struct player * player, struct ball * ball, int max_x, int max_y);
/*Game loop prototypes*/
void gameLoop(struct ball * ball, struct player *player);

//holds how many times game loop has been called
unsigned long gameTime;

int max_y = 0, max_x = 0;
/* The main function handles user input, the game visuals, and checks for win/loss conditions */
int main() {
    menu();
   struct ball gameBall;


   //set initialize max x and y

    struct ball *ballPtr;
ballPtr = &gameBall;
//initial player settings
struct player player1;
struct player *playerPtr;
playerPtr = &player1;
player1.score=0;
player1.lives=3;
player1.level=1;

clear();
curs_set(TRUE);
mvprintw(5,10,"What is your name? ");
getstr(player1.name);

clear();
mvprintw(5,10,"What color would you like your ball to be? ");
mvprintw(6,10,"Enter '1' for green, '2' for red, '3' for yellow");
mvprintw(7,10,"'4' for blue, '5' for magenta, '6' for cyan, or '7' for white ");

char c = getch();




processColor(c, playerPtr);

clear();


//initialize curses mode & settings

   start_color();
   setCursesSettings();
    initializeColors();

    gameLoop(ballPtr, playerPtr);

   return 0;
}

void gameLoop(struct ball * ball, struct player *player)
{
    getmaxyx(stdscr, max_y, max_x);
    nodelay(stdscr,TRUE);

 //initial game settings
 ball->x=51;
 ball->y=29;
 ball->next_x=10;
 ball->next_y=0;
 ball->xDirection=0;
 ball->yDirection=-1;
 gameTime=0;
 drawStart(player, ball, max_x, max_y);
 long speed = 0;
    while(1) {

        getmaxyx(stdscr, max_y, max_x);
       clear();

        setMapBorders(ball, player, max_x, max_y);


        //change ball speed depending on player score
        if (player->score >=100000){
        speed = 70000;
        }
        else if(player->score <= 999999 && player->score >=75000)
        {
        speed = 75000;
        }
        else if(player->score <= 74999 && player->score >=45000)
        {
        speed = 80000;
        }
        else if(player->score <= 44999 && player->score >=15000)
        {
        speed = 85000;
        }
        else
        {
        speed = 90000;
        }





        attron(A_BOLD);
        attron(COLOR_PAIR(player->color)); //on color
        mvprintw(ball->y, ball->x, "O"); //draw game ball
        usleep(speed); //ball speed
        attroff(COLOR_PAIR(player->color));
         attroff(A_BOLD);
        //logic for cycling through levels
        if (player->score >90000){
        attron(COLOR_PAIR(setMapColor(player))); //on color
        setMapD(ball, player);
        attroff(COLOR_PAIR(setMapColor(player))); //off color
        attron(COLOR_PAIR(setFlipperColor(player))); //on color
        drawFlippers(27,30, ball); //draw flippers
        attroff(COLOR_PAIR(setFlipperColor(player))); //off color
        }
        else if (player->score >=90000 && player->score<91000){
        player->score+=1000;
        player->lives++; //extra life for level completion
        player->level++;
        pause(ball, player);
        }
        else if (player->score >60000){
        attron(COLOR_PAIR(setMapColor(player))); //on color
        setMapA(ball, player);
        attroff(COLOR_PAIR(setMapColor(player))); //off color
        attron(COLOR_PAIR(setFlipperColor(player))); //on color
        drawFlippers(27,30, ball); //draw flippers
        attroff(COLOR_PAIR(setFlipperColor(player))); //off color
        }
        else if (player->score >=60000 && player->score<61000){
        player->score+=1000;
        player->lives++; //extra life for level completion
        player->level++;
        pause(ball, player);
        }
        else if (player->score >30000){
        attron(COLOR_PAIR(setMapColor(player))); //on color
        setMapC(ball, player);
        attroff(COLOR_PAIR(setMapColor(player))); //off color
        attron(COLOR_PAIR(setFlipperColor(player)));  //on color
        drawFlippers(27,30, ball);  //draw flippers
        attroff(COLOR_PAIR(setFlipperColor(player)));  //off color
        }
        else if (player->score >=30000 && player->score<31000){
        player->score+=1000;
        player->lives++; //extra life for level completion
        player->level++;
        pause(ball, player);
        }
        else {
        attron(COLOR_PAIR(setMapColor(player))); //on color
        setMapB(ball, player);
        attroff(COLOR_PAIR(setMapColor(player))); //off color
        attron(COLOR_PAIR(setFlipperColor(player))); // on color
       drawFlippers(27,30, ball); //draw flippers
       attroff(COLOR_PAIR(setFlipperColor(player))); //off color
    }
//move ball

ballMovementX(ball, max_x);
ballMovementY(ball, max_y);
gameTime++; //advance time counter by 1
refresh();

   }

}


/*makes Vertical border (no bounce) "|" */
void makeXBorder(int y,int x, int count)
{
    for (int i=0; i <count; i++)
    {
        mvaddch(y+i,x,'|');
    }
}
/*makes Horizontal border (no bounce) "_" */
void makeYBorder(int y,int x, int count)
{
    for (int i=0; i <count; i++)
    {
        mvaddch(y,x+i,'_');
    }
}






/*makes Vertical wall "|" */
void makeXWall(int y,int x, int count, struct ball * ball)
{
    for (int i=0; i <count; i++)
    {
        mvaddch(y+i,x,'|');
        if (ball->xDirection == -1) //up direction
        {
            if((ball->next_x == x+1 && ball->next_y == y+i) || (ball->next_x == x && ball->next_y == y+i))
                ball->xDirection *= -1;

        }
        else //right direction
        {
            if((ball->next_x == x-1 && ball->next_y == y+i) || (ball->next_x == x && ball->next_y == y+i))
                ball->xDirection *= -1;

                }
    }
}

/*makes Horizontal wall "_" */
void makeYWall(int y,int x, int count, struct ball * ball)
{
    for (int i=0; i <count; i++){
            mvaddch(y,x+i,'_');
            if(ball->yDirection==-1){ //up direction
                 if(ball->next_y -1 == y && ball->next_x==x+i )
                {
                ball->yDirection *=-1;

                }
            }
            else //down direction
            {
                if(ball->next_y  == y && ball->next_x==x+i  )
                {
                ball->yDirection *=-1;

                }
            }

    }

}




/*makes Diagonal down wall "\" */
void makeDiagonalWallDown(int y,int x, int count, struct ball * ball)
{
for (int i=0; i <count; i++)
    {
        mvaddch(y+i,x+i, '\\');

                //directly up
                if(ball->xDirection==0 && ball->yDirection==-1)
                {

                    if((ball->next_y==y+i+1 && ball->next_x ==x+i))
                    {
                            ball->yDirection*=-1;
                            ball->xDirection=-1;
                    }
                }
                //up and to the left
                else if(ball->xDirection==-1 && ball->yDirection==-1)
                {

                    if((ball->next_y==y+i+1 && ball->next_x ==x+i+1))
                    {
                            ball->yDirection*=-1;
                    }
                }
                 //down and to the right
                else if(ball->xDirection==1&& ball->yDirection==1)
                {
                    if((ball->next_y==y+i-1 && ball->next_x ==x+i-1))
                    {
                            ball->yDirection*=-1;
                    }
                }
                //down and to the left
                else if (ball->xDirection==-1 && ball->yDirection==1)
                {

                    if((ball->next_y==y+i-1 && ball->next_x ==x+i) || (ball->next_y==y+i-1 && ball->next_x ==x+i+1))
                    {
                    ball->yDirection*=-1;
                    ball->xDirection*=-1;
                            if(gameTime % 2 == 0)
                            {
                            ball->x++;
                            ball->y--;
                            }

                    }
                }
                //up and to the right
               else if ((ball->xDirection==1 && ball->yDirection==-1))
              {
                    if((ball->next_y==y+i+1 && ball->next_x ==x+i) || (ball->next_y==y+i+1 && ball->next_x ==x+i-1))
                    {
                    ball->yDirection*=-1;
                    ball->xDirection*=-1;
                            if(gameTime % 2 == 0) //even
                            {
                            ball->x--;
                            ball->y++;
                            }

                }
    }

}}





/*makes Diagonal up wall "/" */
void makeDiagonalWallUp(int y,int x, int count, struct ball * ball)
{
for (int i=0; i <count; i++)
    {
         mvaddch(y+i,x-i, '/');

                //up and to the left
                if(ball->xDirection==-1 && ball->yDirection==-1)
                {

                    if((ball->next_y==y+i+1 && ball->next_x ==x-i) || (ball->next_y==y+i+1 && ball->next_x ==x-i+1))
                    {
                            ball->xDirection*=-1;
                            ball->yDirection*=-1;
                            if(gameTime % 2 == 0)
                            {
                            ball->x++;
                            ball->y++;
                            }
                    }
                }
                 //down and to the right
                else if(ball->xDirection==1&& ball->yDirection==1)
                {
                    if((ball->next_y==y+i-1 && ball->next_x ==x-i-1) || (ball->next_y==y+i-1 && ball->next_x ==x-i)  )
                    {
                            ball->xDirection*=-1;
                            ball->yDirection*=-1;
                            if(gameTime % 2 == 0)
                            {
                            ball->x--;
                            ball->y--;
                            }



                    }
                }
                //down and to the left
                else if (ball->xDirection==-1 && ball->yDirection==1)
                {

                    if((ball->next_y==y+i-1 && ball->next_x ==x-i+1) )
                    {
                    ball->yDirection*=-1;
                    }
                }
                //up and to the right
               else if ((ball->xDirection==1 && ball->yDirection==-1))
              {


                    if((ball->next_y==y+i+1 && ball->next_x ==x-i-1))
                    {
                    ball->yDirection*=-1;
                    }
                }
    }
}




void drawStart(struct player * player, struct ball * ball, int max_x, int max_y)
{

    setMapBorders(ball, player, max_x, max_y);




      attron(COLOR_PAIR(setFlipperColor(player)));  //on color
       drawFlippers(27,30, ball); //draw flippers
      attroff(COLOR_PAIR(setFlipperColor(player)));  //off color

        attron(COLOR_PAIR(player->color)); //on color
        mvprintw(12, 62, "Press 'SPACEBAR' to start");

        attron(A_BOLD);
        mvprintw(29, 51, "O"); //draw game ball
        attroff(COLOR_PAIR(player->color));
        attroff(A_BOLD);
    if (player->score >=90000){
         attron(COLOR_PAIR(setMapColor(player)));
        setMapD(ball, player);
       attroff(COLOR_PAIR(setMapColor(player)));
        }
        else if(player->score >=60000)
        {
        attron(COLOR_PAIR(setMapColor(player)));
        setMapA(ball, player);
        attroff(COLOR_PAIR(setMapColor(player)));
        }
        else if(player->score >= 30000)
        {
        attron(COLOR_PAIR(setMapColor(player)));
        setMapC(ball, player);
        attroff(COLOR_PAIR(setMapColor(player)));
        }
        else
        {
        attron(COLOR_PAIR(setMapColor(player)));
        setMapB(ball, player);
        attroff(COLOR_PAIR(setMapColor(player)));
        }




    refresh();
    int c = getchar();

	switch (c)
	{
    case ' ':

            setCursesSettings();

        break;
	 default:
	    drawStart(player, ball, max_x, max_y);
	    break;
	 }

}


/*makes goal post "*" with inputted score */
void makeGoalPost(int y, int x, long score, struct player * player, struct ball * ball)
{

    if((ball->y ==y && ball->x ==x) || (ball->y ==y && ball->x ==x-1)|| (ball->y ==y && ball->x ==x+1)){
        player->score +=score;
    }
    else
    {
        mvaddch(y,x,'*');
    }


}


/*Draws invisible wall where out of bounds is*/
void outOfBounds(int y, int x, int count , struct ball * ball, struct player * player)
{
    for (int i= 0; i <count; i++){
        if(ball->y ==y && ball->x ==x+i){
        player->lives--;
        if (player->lives !=0){
            endwin();
            pause(ball, player);
        }
        else{
            gameOver(player);
        }
    }
}
}


/* makes ball move on X-Axis     Found funcion for basic wall movement here: https://www.viget.com/articles/game-programming-in-c-with-the-ncurses-library/*/
void ballMovementX(struct ball * ball, int max_x)
{
    ball->next_x = ball->x + ball->xDirection;
  if (ball->x == max_x || ball->x == 0 ) {
 ball->xDirection*= -1;
 } else {
 ball-> x+= ball->xDirection;
 }
}


/* makes ball move on Y-Axis*/
void ballMovementY(struct ball * ball , int max_y)
{
    ball->next_y = ball->y + ball->yDirection;

  if (ball->next_y == max_y || ball->next_y == 0) {
 ball->yDirection*= -1;
    }
    else {
 ball->y += ball->yDirection;
 }

}


/*Sets initial borders and text*/
void setMapBorders(struct ball * ball, struct player * player, int max_x, int max_y)
{
    attron(COLOR_PAIR(setMapBorderColor(player)));
    makeXWall(2, 2, 29, ball);
    makeXWall(7, max_x-70, 30, ball);
    makeXWall(2, max_x-68, 29, ball);
    makeYWall(1, 2, 50, ball);
    makeYWall(max_y-3, 3, 16, ball);
    //makeYWall(max_y-3, 3, 15, ball);
    makeYWall(max_y-3, max_x-89, 19, ball);
    makeDiagonalWallDown(2,max_x-71, 3, ball);
    makeDiagonalWallDown(25,3,3,ball);
    makeDiagonalWallUp(25,49,3,ball);
    makeDiagonalWallUp(2, 5, 3, ball);

    // close wall off from starting zone
    if (gameTime >= 27)
{
    makeXWall(4, max_x-70,3, ball);
}


    outOfBounds(max_y-1, 15  , 20, ball, player);

    //print out player's score and lives
    mvprintw(6, 65, "%s's score: %d",player->name, player->score);
    mvprintw(7, 65, "%s's lives: %d",player->name, player->lives);
    mvprintw(8, 65, "Game level: %d", player->level);
    attroff(COLOR_PAIR(setMapBorderColor(player)));
    attron(COLOR_PAIR(3));
    //score board
    makeYWall(4,58,34, ball);
    makeYWall(9,58,34,ball);
    makeXWall(5,57,5,ball);
    makeXWall(5,92,5,ball);
    attroff(COLOR_PAIR(3));

    //set horizontal level walls here to fix flicker
    if(player->level==1)
    {
       attron(COLOR_PAIR(setMapColor(player)));
        makeYWall(4,7,7, ball);
        makeYWall(4,22,7, ball);
        makeYWall(4,39,7, ball);
        attroff(COLOR_PAIR(setMapColor(player)));
    }
    else if(player->level==2)
    {
        attron(COLOR_PAIR(setMapColor(player)));
        makeYWall(20,35,5, ball);
         makeYWall(20,15,5, ball);
        attroff(COLOR_PAIR(setMapColor(player)));
    }

    else if(player->level==3)
    {
        attron(COLOR_PAIR(setMapColor(player)));
        makeYWall(4,22,7, ball);
        makeYWall(4,39,7, ball);
        attron(COLOR_PAIR(setMapColor(player)));
    }





}


/*Draws Map A*/
void setMapA(struct ball * ball, struct player * player)
{
    makeGoalPost(6,20, 1000, player, ball);
    makeGoalPost(12,24, 1000, player, ball);
    makeGoalPost(10, 45, 1000, player, ball);
    makeDiagonalWallUp(10,8,3, ball);
    makeDiagonalWallUp(10,40,2,ball);
    makeDiagonalWallDown(15,45,3,ball);
    makeDiagonalWallDown(5, 12, 5, ball);
    makeDiagonalWallUp(11,33,3,ball);
    makeXWall(12,39,2, ball);
     makeDiagonalWallDown(18, 7, 2, ball);
    makeDiagonalWallUp(18, 20, 2, ball);

    makeDiagonalWallUp(20, 46, 2, ball);
     makeDiagonalWallUp(21, 10, 2, ball);
    makeDiagonalWallUp(24, 36, 2, ball);


}

/*Draws map B*/
void setMapB(struct ball * ball, struct player * player)
{

    makeXWall(5,7,2, ball);
    makeXWall(5,13,2, ball);
    makeXWall(5,22,2, ball);
    makeXWall(5,28,2, ball);
    makeXWall(5,39,2, ball);
    makeXWall(5,45,2, ball);
    makeDiagonalWallDown(7,7, 2 , ball);
    makeDiagonalWallDown(7,13, 2 , ball);
    makeDiagonalWallUp(7, 39, 2, ball);
    makeDiagonalWallUp(7, 45, 2, ball);
    makeDiagonalWallUp(7, 22, 2, ball);
    makeDiagonalWallDown(7,28, 2 , ball);

    makeDiagonalWallUp(12,5, 2 , ball);
    makeDiagonalWallUp(12,18, 2 , ball);
    makeDiagonalWallUp(12,32, 2 , ball);
    makeDiagonalWallUp(12,45, 2 , ball);

    makeDiagonalWallDown(16, 7, 2, ball);
    makeDiagonalWallDown(16, 20, 2, ball);
    makeDiagonalWallDown(16, 33, 2, ball);
    makeDiagonalWallDown(16, 46, 2, ball);


    makeDiagonalWallUp(21,5, 2 , ball);
    makeDiagonalWallUp(21,18, 2 , ball);
    makeDiagonalWallUp(21,32, 2 , ball);
    makeDiagonalWallUp(21,45, 2 , ball);



    makeGoalPost(6, 25, 1000, player, ball);
    makeGoalPost(6, 10, 1000, player, ball);
    makeGoalPost(6, 42, 1000, player, ball);


}

/* Draws map C*/
void setMapC(struct ball * ball, struct player * player)
{

    makeGoalPost(7, 25, 1000, player, ball);
    makeXWall(6, 23, 2, ball);
    makeXWall(6, 27, 2, ball);
    makeXWall(6, 38, 2, ball);
    makeGoalPost(7, 40, 1000, player, ball);
    makeGoalPost(7, 10, 1000, player, ball);
    makeXWall(6, 12, 2, ball);

    makeGoalPost(11, 25, 1000, player, ball);
    makeGoalPost(11, 40, 1000, player, ball);
    makeGoalPost(11, 10, 1000, player, ball);
    makeXWall(10, 38, 2, ball);
    makeXWall(10, 12, 2, ball);
    makeXWall(10, 23, 2, ball);
    makeXWall(10, 27, 2, ball);


     makeGoalPost(15, 25, 1000, player, ball);
    makeGoalPost(15, 40, 1000, player, ball);
    makeGoalPost(15, 10, 1000, player, ball);
    makeXWall(14, 38, 2, ball);
    makeXWall(14, 12, 2, ball);
    makeXWall(14, 23, 2, ball);
    makeXWall(14, 27, 2, ball);



}

/*Draws map D*/
void setMapD(struct ball * ball, struct player * player)
{
    makeGoalPost(8, 15, 1000, player, ball); //eyes
    makeGoalPost(8, 32, 1000, player, ball);
    makeDiagonalWallDown(16,9,4,ball);
    makeGoalPost(20, 13, 1000, player, ball);
    makeGoalPost(20, 14, 1000, player, ball);
    makeGoalPost(20, 15, 1000, player, ball);
    makeGoalPost(20, 16, 1000, player, ball);
    makeGoalPost(20, 17, 1000, player, ball);
    makeGoalPost(20, 18, 1000, player, ball);
    makeGoalPost(20, 19, 1000, player, ball);
    makeGoalPost(20, 20, 1000, player, ball);
    makeGoalPost(20, 21, 1000, player, ball);
    makeGoalPost(20, 22, 1000, player, ball);
    makeGoalPost(20, 23, 1000, player, ball);
    makeGoalPost(20, 24, 1000, player, ball);
    makeGoalPost(20, 25, 1000, player, ball);
    makeGoalPost(20, 26, 1000, player, ball);
    makeGoalPost(20, 27, 1000, player, ball);
    makeGoalPost(20, 28, 1000, player, ball);
    makeGoalPost(20, 29, 1000, player, ball);
    makeGoalPost(20, 30, 1000, player, ball);
    makeGoalPost(20, 31, 1000, player, ball);
    makeGoalPost(20, 32, 1000, player, ball);
    makeGoalPost(20, 33, 1000, player, ball);
    makeGoalPost(20, 34, 1000, player, ball);
    makeDiagonalWallUp(16, 38, 4, ball);


}


/* sets map color for various levels*/
int setMapColor(struct player *  player)
{
if (player->level == 1)
	 return 6; //cyan
else if (player->level == 2)
	return 4;  //blue
else if (player->level == 3)
	return 1; //green
else if (player->level == 4)
	return 3; //yellow
}


/* sets border color for various levels*/
int setMapBorderColor(struct player *  player)
{
if (player->level == 1)
	 return 1; //green
    else if (player->level == 2)
	return 2; //red
    else if (player->level == 3)
	return 5; //magenta
    else if (player->level == 4)
	return 4; //blue
}

/* sets flipper color for various levels*/
int setFlipperColor(struct player *  player)
{
if (player->level == 1)
	 return 5; //magenta
    else if (player->level == 2)
	return 3; //yellow
    else if (player->level == 3)
	return 2; //red
    else if (player->level == 4)
	return 1; //green
}

void initializeColors()
{

init_pair(1, COLOR_GREEN,COLOR_BLACK); // 1- green
init_pair(2, COLOR_RED,COLOR_BLACK); //2-red
init_pair(3, COLOR_YELLOW,COLOR_BLACK); //3-yellow
init_pair(4, COLOR_BLUE,COLOR_BLACK); //4- blue
init_pair(5, COLOR_MAGENTA,COLOR_BLACK); //5-magenta
init_pair(6, COLOR_CYAN,COLOR_BLACK); //6-cyan
init_pair(7, COLOR_WHITE,COLOR_BLACK); //7-white

}
/*Sets initial curses settings*/
 void setCursesSettings()
 {
    noecho();
    cbreak();
   nodelay(stdscr,1);
   keypad(stdscr,1);
   srand(time(NULL));
   curs_set(FALSE);
 }


/*controls menu*/
 void controlsMenu()
{
    //initial settings
    clear();

    attron(COLOR_PAIR(3));
    mvprintw(4,10,"The left arrow key controls the left flipper and the right arrow controls the right flipper");
    mvprintw(6,10,"Press '1' to go back to the main menu, press '2' to see the story.");
    mvprintw(8,10,"You could also press '3' play the game or '4' to quit.");
    mvprintw(10,10,"Note: This game is meant to be played in the console size of 120 columns and 30 rows");
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(2));
        //draws box
    makeYBorder(2,7,108);
    makeXBorder(3,6,10);
    makeXBorder(3,115,10);
    makeYBorder(12,7,108);

    attroff(COLOR_PAIR(2));

    refresh();
    int c = getchar();
	switch (c)
	{
    case'1':
    clear();
    menu();
    break;
	 break;
	 case'2':
    clear();
    storyMenu();
    break;
    case'3':
    clear();
    break; //exits to game
    case'4':
      exit(EXIT_SUCCESS);
    break;
	 default:
	     controlsMenu();
	 }


}

/*story menu*/
void storyMenu()
{
    //initial settings
    clear();

     attron(COLOR_PAIR(1));
     mvprintw(4,10,  "Astronauts need pinball too! But pinball in space is a little different... There's no gravity!");
     mvprintw(6,10, "You are aboard the Mars-11 in route to spend the rest of your life on an uninhabited planet.");
     mvprintw(8,10, "Bummer!!!!!!!!!! ...At least we have pinball!!!!");
     mvprintw(10,10,"Through the power of science, we have figured out how to play pinball on the spaceship using a vacuum.");
     mvprintw(12,10,"Don't worry about speed...it gets fast...too fast for a lot of us to keep up.. can you?");
     mvprintw(14,10,"Press '1' to go back to the main menu or press '2' to see the controls.");
     mvprintw(16,10,"You could also press '3' to start playing or '4' to quit.");
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(2));
        //draws box
    makeYBorder(2,7,108);
    makeXBorder(3,6,15);
    makeXBorder(3,115,15);
    makeYBorder(17,7,108);

    attroff(COLOR_PAIR(2));


    refresh();
    int c = getchar();
	switch (c)
	{
    case'1':
    clear();
    menu();
    break;

    case '2':
        clear();
        controlsMenu();
        break;
    case'3':
        clear();
    break; //exits to game

    case'4':
    exit(EXIT_SUCCESS);
    break;

    default:
    storyMenu();
    break;
    }
}


/*Pause function */
void pause(struct ball * ball, struct player * player){
    //initscr();

    clear();
    mvprintw(5,40,"%s's lives: %d",player->name,  player->lives);
    mvprintw(7,40, "%s's score: %d",player->name, player->score);
    mvprintw(9,40, "Game level: %d", player->level);
    mvprintw(11,30, "Press '1' to continue or '4' to quit");

     //draws box
    makeYBorder(3,26,44);
    makeXBorder(4,25,9);
    makeXBorder(4,70,9);
    makeYBorder(12,26,44);

    refresh();
    int c = getchar();
    endwin();
	switch (c)
	{
    case'1':
        initscr();
        setCursesSettings();
        initializeColors();
        gameLoop(ball, player);
        break;
    break;
    case'4':
    exit(EXIT_SUCCESS);
    break;
	 default:
	     pause(ball, player);
	 }


 }



/*Game over screen*/
void gameOver(struct player * player) {



      clear();
    mvprintw(5,45,"Game Over =[");
     mvprintw(7,42,"%s's end score: %d",player->name, player->score);
     mvprintw(9,45,"Game level: %d", player->level);
    mvprintw(11,30,"Press '1' to go to the main menu or '4' to quit");


    //draws box
    makeYBorder(3,26,52);
    makeXBorder(4,25,9);
    makeXBorder(4,78,9);
    makeYBorder(12,26,52);

      refresh();

      switch(getchar())
      {
    case'1':
    endwin();
    main();
    break;
    case'4':
    endwin();
    exit(EXIT_SUCCESS);
    break;
	 default:
	     gameOver(player);
      }

}







/*Draws move-able flippers onto the screen*/
 void drawFlippers(int y, int x, struct ball * ball)
 {
     //char input = 0;
     switch (getch())
 {
     case KEY_LEFT:
         mvaddch(y,x-11, '-');
             mvaddch(y,x-10 , '-');
             mvaddch(y,x -9, '-');
             mvaddch(y,x -8, '-');
             mvaddch(y,x -7, '-');
             mvaddch(y,x-6 , '-');

           mvaddch(y,x, '/');
                 mvaddch(y+1,x-1, '/');
                 mvaddch(y+2,x-2, '/');
                 if((ball->next_y == y && ball->next_x == x-11) || (ball->next_y == y && ball->next_x == x-10) || (ball->y == y && ball->x == x-11) || (ball->y == y && ball->x == x-10))
             {
                ball->yDirection*=-1;
                ball->xDirection*=-1;
                 ball->x--;
                ball->y--;
             }
                else if((ball->next_y == y && ball->next_x == x-9) ||(ball->next_y == y && ball->next_x == x-8) || (ball->next_y == y && ball->next_x == x-7) ||(ball->next_y == y && ball->next_x == x-6) ||
                            (ball->y == y && ball->x == x-9) ||(ball->y == y && ball->x == x-8) || (ball->y == y && ball->x == x-7) ||(ball->y == y && ball->x == x-6) ||(ball->y == y && ball->x == x-6))
             {
                ball->yDirection*=-1;
                 ball->x++;
              ball->y--;
             }
            break;
         case KEY_RIGHT:

                mvaddch(y,x+1 , '-');
                mvaddch(y,x , '-');
             mvaddch(y,x -1, '-');
             mvaddch(y,x -2, '-');
             mvaddch(y,x -3, '-');
            mvaddch(y,x -4, '-');

             mvaddch(y,x-11, '\\');
                 mvaddch(y+1,x-10, '\\');
                 mvaddch(y+2,x-9, '\\');


             if((ball->next_y== y&& ball->next_x == x+1) || (ball->next_y == y&& ball->next_x == x) || (ball->y== y&& ball->x == x+1) || (ball->y == y&& ball->x == x))

             {
                ball->yDirection*=-1;
                ball->xDirection*=-1;
                 ball->x++;
                ball->y--;


             }
                else if ((ball->next_y == y&& ball->next_x == x-1) ||(ball->next_y == y && ball->next_x == x-2) ||(ball->next_y == y && ball->next_x == x-3)||(ball->next_y == y && ball->next_x == x-4) ||
                            (ball->y == y&& ball->x == x-1) ||(ball->y == y && ball->x == x-2) ||(ball->y == y && ball->x == x-3)||(ball->y == y && ball->x == x-4))

         {

              ball->yDirection*=-1;
              ball->x--;
              ball->y--;


         }
            break;
        default:
                mvaddch(y,x, '/');
                 mvaddch(y+1,x-1, '/');
                 mvaddch(y+2,x-2, '/');

                 mvaddch(y,x-11, '\\');
                 mvaddch(y+1,x-10, '\\');
                 mvaddch(y+2,x-9, '\\');
         break;
 }
 }




/*main menu*/
void menu()
{
initscr();
keypad(stdscr,TRUE);
curs_set(FALSE);
int code = 0;

//initial settings
start_color();
initializeColors();

//print
attron(COLOR_PAIR(6));
mvprintw(4,6,       ".d8888b.                                          8888888b.  d8b          888               888 888");
mvprintw(5,5,      "d88P  Y88b                                         888   Y88b Y8P          888               888 888");
mvprintw(6,5,      "Y88b.                                              888    888              888               888 888");
mvprintw(7,6,       "'Y888b.   88888b.   8888b.   .d8888b .d88b.       888   d88P 888 88888b.  88888b.   8888b.  888 888");
mvprintw(8,9,          "'Y88b. 888 '88b     '88b d88P'   d8P  Y8b      8888888P'  888 888 '88b 888 '88b     '88b 888 888");
mvprintw(9,11,           "'888 888  888 .d888888 888     88888888      888        888 888  888 888  888 .d888888 888 888");
mvprintw(10,5,     "Y88b  d88P 888 d88P 888  888 Y88b.   Y8b.          888        888 888  888 888 d88P 888  888 888 888");
mvprintw(11,6,      "'Y8888P'  88888P'  'Y888888  'Y8888P 'Y8888       888        888 888  888 88888P'  'Y888888 888 888");
mvprintw(12,16,               "888");
mvprintw(13,16,               "888");
mvprintw(14,16,               "888");

mvprintw(16, 40, "Tyler Schmidt");
mvprintw(17, 40, "Spring 2018 CS 2060 SEC 01");
mvprintw(20, 7, "Press '1' to enter game press '2' for controls and press '3' for story and press '4' to  quit");

 attroff(COLOR_PAIR(6));
  attron(COLOR_PAIR(5));
    //draws box
    makeYBorder(14,39,28);
    makeXBorder(15,38,4);
    makeXBorder(15,67,4);
    makeYBorder(18,39,28);
  attroff(COLOR_PAIR(5));

 refresh();

        int c = getchar();

	switch (c)
	{
	case '1':
		break;

	case '2':
        controlsMenu();
		break;

    case '3':
        storyMenu();
        break;

	case '4':
        exit(EXIT_SUCCESS);
		break;

	default:
	    menu(); //default: recursion of menu
	    break;

	}

	return code;
}


/*Processes color into ball color from user input*/
void processColor(char c, struct player * player)
{

switch (c)
	{
  	case '1':
	player->color=1; // 1- green
	break;

     	case '2':
	player->color=2; //2-red
	break;

	case '3':
        player->color=3; //3-yellow
	break;

	case '4':
	player->color=4; //4- blue
	break;

	case '5':
	player->color=5; //5-magenta
	break;

	case '6':
	player->color=6; //6-cyan
	break;

        case '7':
	player->color=7; //7-white
	break;

	default:
	player->color=7; //default- white
    break;
        }
}




