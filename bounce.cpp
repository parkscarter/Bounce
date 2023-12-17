#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>
#include <ncurses.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include <cmath>

class Ball{
public:
    double x;
    double y;
    double dir;
    int vel;
};

class Paddle{
public:
    int top;
    int x;
};

class Player{
public:
    int numPoints;
};

int drawBox(){
    int i;
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK); // Define a color pair (pair number, foreground color, background color)

    // Draw the box
    attron(COLOR_PAIR(1)); // Turn on color pair
    for (int i = 0; i <= 79; ++i) {
        mvaddch(0, i, ACS_HLINE); // Draw top horizontal line
        mvaddch(21, i, ACS_HLINE);   // Draw bottom horizontal line
    }

    for (int i = 0; i <= 21; ++i) {
        mvaddch(i, 0, ACS_VLINE); // Draw left vertical line
        mvaddch(i, 79, ACS_VLINE);   // Draw right vertical line
    }

    mvaddch(0, 0, ACS_ULCORNER); // Draw upper-left corner
    mvaddch(0, 79, ACS_URCORNER);   // Draw upper-right corner
    mvaddch(21, 0, ACS_LLCORNER);   // Draw lower-left corner
    mvaddch(21, 79, ACS_LRCORNER);     // Draw lower-right corner

    attroff(COLOR_PAIR(1)); // Turn off color pair
    return 0;
    
}

int addBall(Ball* ball){
    int ballY = rand() % 20 + 1;
    int ballX = 40;
    int randomAngle = rand() % 360;

    while ((randomAngle > 60 && randomAngle < 120) || (randomAngle > 240 && randomAngle < 300)){
        randomAngle = rand() % 360;
    }
    double radians = randomAngle * M_PI / 180.0; // Convert degrees to radians

    ball->x = ballX;
    ball->y = ballY;
    ball->dir = radians;
    ball->vel = 5;
    

    mvaddch(ballY, ballX, 'o');
    return 0;
}

int moveBall(Ball* ball, Player* p1, Player* p2, Paddle* left, Paddle* right){
    int randomAngle;
    double radians;
    mvaddch(ball->y, ball->x, ' ');
    ball->x += (cos(ball->dir));
    ball->y += (sin(ball->dir));

    if (ball->x < 2){
        if (ball->y - left->top < 4 && ball->y - left->top >=0){
            ball->x = 2;
            ball->dir = M_PI - ball->dir;
            ball->vel += 1;
        }
    }
    else if (ball->x > 77){
        if (ball->y - right->top < 4 && ball->y - right->top >=0){
            ball->x = 77;
            ball->dir = M_PI - ball->dir;
            ball->vel += 1;
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

int drawPaddle(Paddle* paddle){
    mvaddch(paddle->top, paddle->x, '|');
    mvaddch(paddle->top + 1, paddle->x, '|');
    mvaddch(paddle->top + 2, paddle->x, '|');
    mvaddch(paddle->top + 3, paddle->x, '|');

    return 0;
}

int moveUp(Paddle* paddle){
    if (paddle->top > 1){
        paddle->top -= 1;
        mvaddch(paddle->top + 4, paddle->x, ' ');
    }
    return 0;
}

int moveDown(Paddle* paddle){
    if (paddle->top < 17){
        paddle->top += 1;
        mvaddch(paddle->top - 1, paddle->x, ' ');
    }
    return 0;
}

int main(int argc, char *argv[]){
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
    mvprintw(8,5, "Press any button to play; Enjoy!");

    refresh();
    getch();
    clear();
    Ball ball;
    Paddle leftPaddle;
    Paddle rightPaddle;
    Player p1;
    Player p2;

    p1.numPoints = 0;
    p2.numPoints = 0;

    rightPaddle.x = 78;
    rightPaddle.top = 8;

    leftPaddle.x = 1;
    leftPaddle.top = 8;

    addBall(&ball);
    refresh(); // Refresh the screen to display the changes
    timeout(0);
    int wait;
    int ui;

    while(ui != 'q' && p1.numPoints < 3 && p2.numPoints < 3){
        wait = 500000 / (&ball)->vel;
        usleep(wait);
        moveBall(&ball, &p1, &p2, &leftPaddle, &rightPaddle);
        drawBox();
        drawPaddle(&rightPaddle);
        drawPaddle(&leftPaddle);
        mvaddch(ball.y, ball.x, 'o');
        mvprintw(22, 0, "p1: %d", p1.numPoints);
        mvprintw(22, 25, "First to 3 wins, press 'q' to quit");
        mvprintw(22, 74, "p2: %d", p2.numPoints);
        refresh(); // Refresh the screen to display the changes
        ui = getch();
        if (ui == 'e'){
            moveUp(&leftPaddle);
        }
        else if(ui == 'd'){
            moveDown(&leftPaddle);
        }
        else if(ui == KEY_UP){
            moveUp(&rightPaddle);
        }
        else if(ui == KEY_DOWN){
            moveDown(&rightPaddle);
        }
    }
    clear();

    if (p1.numPoints == 3){
        mvprintw(1,0, "Congrats player 1! You won!");
    }
    else if(p2.numPoints ==3){
        mvprintw(1,0, "Congrats player 2! You won!");
    }
    
    refresh();
    usleep(1000000);
    endwin();  // End curses mode

    return 0;
}