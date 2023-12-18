#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <ncurses.h>
#include <cstring>

class Ball{
public:
    double x;
    double y;
    double dir;
    int vel;

    Ball(int velocity)
        :  vel(velocity){
        int randomAngle = rand() % 360;
        x = 40;
        y = rand() % 20 + 1;
        while ((randomAngle > 60 && randomAngle < 120) || (randomAngle > 240 && randomAngle < 300)){
            randomAngle = rand() % 360;
        }
        dir = randomAngle * M_PI / 180.0;
    }
};

class Paddle{
public:
    int top;
    int x;
    int length;
    Paddle(int initialTop, int initialX, int initialLength) 
        : top(initialTop), x(initialX), length(initialLength) {
    }

    int moveUp(){
        if (top > 1){
            top -= 1;
            mvaddch(top + length, x, ' ');
        }
        return 0;
    }

    int moveDown(){
        if (top + length < 21){
            top += 1;
            mvaddch(top - 1, x, ' ');
        }
        return 0;
    }
    int draw(){
        int i;
        for (i = 0; i < length; i ++){
            mvaddch(top + i, x, '|');
        }
    return 0;
    }
};

class Player{
public:
    int numPoints;
};

/*
This function uses ncurses to draw a red box for the 
*/
int drawBox(){
    int i;
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);   //Color pair 1; Red on Black

    attron(COLOR_PAIR(1));      // Turn on color pair
    for (int i = 0; i <= 79; ++i) {
        mvaddch(0, i, ACS_HLINE);   // Top horizontal line
        mvaddch(21, i, ACS_HLINE);  // Bottom horizontal line
    }

    for (int i = 0; i <= 21; ++i) {
        mvaddch(i, 0, ACS_VLINE);   // Left vertical line
        mvaddch(i, 79, ACS_VLINE);  // Right vertical line
    }

    mvaddch(0, 0, ACS_ULCORNER);    // Upper-left corner
    mvaddch(0, 79, ACS_URCORNER);   // Upper-right corner
    mvaddch(21, 0, ACS_LLCORNER);   // Lower-left corner
    mvaddch(21, 79, ACS_LRCORNER);  // Lower-right corner

    attroff(COLOR_PAIR(1));     // Turn off color pair
    return 0;
}

/*
This function uses the ball's location and direction to move the ball, then checks some cases;
    (If the ball hits a paddle, goal, or edge)
*/
int moveBall(Ball* ball, Player* p1, Player* p2, Paddle* left, Paddle* right){
    int randomAngle;
    double radians;
    mvaddch(ball->y,ball->x, ' ');
    ball->x += (cos(ball->dir));
    ball->y += (sin(ball->dir));

    if (ball->x < 2){
        if (ball->y - left->top < left->length && ball->y - left->top >=0){
            ball->x = 2;
            ball->dir = M_PI - ball->dir;
            ball->vel += 2;
        }
    }
    else if (ball->x > 77){
        if (ball->y - right->top < right->length && ball->y - right->top >=0){
            ball->x = 77;
            ball->dir = M_PI - ball->dir;
            ball->vel += 2;
        }
    }
    if (ball->x < 1){   
        randomAngle = rand() % 360;
        while ((randomAngle > 60 && randomAngle < 120) || (randomAngle > 240 && randomAngle < 300)){
            randomAngle = rand() % 360;
        }
        radians = randomAngle * M_PI / 180.0; // Convert degrees to radians
        mvaddch(ball->y, ball->x, ' ');
        p2->numPoints += 1;
        ball->x = 40;
        ball->y = rand() % 20 + 1;
        ball->dir = radians;
        ball->vel = 5;
    }

    if (ball->x > 78){
        randomAngle = rand() % 360;
        while ((randomAngle > 60 && randomAngle < 120) || (randomAngle > 240 && randomAngle < 300)){
            randomAngle = rand() % 360;
        }
        radians = randomAngle * M_PI / 180.0; // Convert degrees to radians
        mvaddch(ball->y, ball->x, ' ');
        p1->numPoints += 1;
        ball->x = 40;
        ball->y = rand() % 20 + 1;
        ball->dir = radians;
        ball->vel = 5;
    }

    if (ball->y < 1){
        ball->y = 1;
        ball->dir = -(ball->dir);
    }

    if (ball->y > 20){
        ball->y = 20;
        ball->dir = -(ball->dir);
    }

    return 0;
}

/*
This function draws the box, paddles, ball, score, and 'Bounce'
*/
int printBoard(Ball* ball, Paddle* l, Paddle* r, Player* a, Player* b){
    drawBox();
    r->draw();
    l->draw();
    mvaddch(ball->y, ball->x, 'o');
    mvprintw(23, 33, "score: %d-%d", a->numPoints, b->numPoints);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK); // Red on Black)
    attron(COLOR_PAIR(1));
    mvprintw(22, 35, "Bounce");
    attroff(COLOR_PAIR(1));
    move(23,80);
    refresh();
    return 0;
}

/*
This function initializes things for the game, gives the user an interface to begin the game
*/
int begin(){
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    srand(time(NULL));
    drawBox();
    mvprintw(1,5, "Welcome to Bounce!");
    mvprintw(2,5, "This totally original game was created as extra c++ practice");
    mvprintw(3,5, "by me, Carter Parks!");
    mvprintw(5,5, "Player 1 is on the left; use 'e' and 'd' to move your paddle");
    mvprintw(6,5, "Player 2 is on the right; use the up and down arrows to move your paddle");
    mvprintw(8,5, "Choose Difficulty: ");
    mvprintw(9, 10, "1: Easy    2: Moderate    3: Hard");
    refresh();
    return 0;
}

int main(int argc, char *argv[]){
    int wait, ui, pLength;
    begin();
    ui = 0;
    while (ui < 1 || ui > 3){
        ui = getch() - '0';
    }
    pLength = 3 + (3 - ui);
    clear();
    Ball ball = Ball((ui * 2) + 1);
    Paddle leftPaddle = Paddle(8, 1, pLength);
    Paddle rightPaddle = Paddle(8, 78, pLength);
    Player p1;
    Player p2;
    p1.numPoints = 0;
    p2.numPoints = 0;

    timeout(0);
    while(ui != 'q' && p1.numPoints < 3 && p2.numPoints < 3){
        wait = 500000 / (&ball)->vel;
        usleep(wait);
        moveBall(&ball, &p1, &p2, &leftPaddle, &rightPaddle);
        printBoard(&ball, &leftPaddle, &rightPaddle, &p1, &p2);
        ui = getch();
        switch (ui) {
            case 'e':
                leftPaddle.moveUp();
                break;
            case 'd':
                leftPaddle.moveDown();
                break;
            case KEY_UP:
                rightPaddle.moveUp();
                break;
            case KEY_DOWN:
                rightPaddle.moveDown();
                break;
            default:
                refresh();
        }
    }
    clear();

    if (p1.numPoints == 3){
        mvprintw(10,35, "Congrats player 1! You won!");
    }
    else if(p2.numPoints ==3){
        mvprintw(10,35, "Congrats player 2! You won!");
    }
    else{
        mvprintw(10,35, "Game Ended");
    }
    move(22,80);
    
    refresh();
    usleep(1000000);
    endwin();  // End curses mode

    return 0;
}