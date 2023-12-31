#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <ncurses.h>
#include <cstring>

int gameWidth = 79;
int gameHeight = 21;

class Ball{
public:
    double x;
    double y;
    double dir;
    int vel;
    int diff;

    Ball(int velocity, int difficulty)
        :  vel(velocity), diff(difficulty){
        int randomAngle = rand() % 360;
        x = gameWidth / 2;
        y = rand() % (gameHeight - 1) + 1;
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
        if (top + length < gameHeight){
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
This function uses ncurses to draw a red box for the game field
*/
int drawBox(int height, int width){
    int i;
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);   //Color pair 1; Red on Black

    attron(COLOR_PAIR(1));      // Turn on color pair
    for (int i = 0; i <= width; ++i) {
        mvaddch(0, i, ACS_HLINE);   // Top horizontal line
        mvaddch(height, i, ACS_HLINE);  // Bottom horizontal line
    }

    for (int i = 0; i <= height; ++i) {
        mvaddch(i, 0, ACS_VLINE);   // Left vertical line
        mvaddch(i, width, ACS_VLINE);  // Right vertical line
    }

    mvaddch(0, 0, ACS_ULCORNER);    // Upper-left corner
    mvaddch(0, width, ACS_URCORNER);   // Upper-right corner
    mvaddch(height, 0, ACS_LLCORNER);   // Lower-left corner
    mvaddch(height, width, ACS_LRCORNER);  // Lower-right corner

    attroff(COLOR_PAIR(1));     // Turn off color pair
    return 0;
}

/*
This function uses the ball's location and direction to move the ball, then checks some cases;
    (If the ball hits a paddle, goal, or edge)
*/
int moveBall(Ball* ball, Paddle* left, Paddle* right, Player* p1, Player* p2){
    int randomAngle;
    double radians;
    mvaddch(ball->y,ball->x, ' ');
    ball->x += (cos(ball->dir));
    ball->y += (sin(ball->dir));

    //Hits the left paddle
    if (ball->x < 2 && 
    ball->y < left->top + left->length && ball->y >= left->top){
        ball->x = 2;
        ball->dir = M_PI - ball->dir;
        ball->vel += ball->diff;
        
    }   //Hits the right paddle
    else if (ball->x > (gameWidth - 2) &&
    ball->y  < right->top + right->length && ball->y >= right->top){
        ball->x = (gameWidth - 2);
        ball->dir = M_PI - ball->dir;
        ball->vel += ball->diff;
        
    }   //P2 Scores (reset ball, add a point to P2)
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
    }   //P1 Scores (reset ball, add a point to P2)
    if (ball->x > (gameWidth - 1)){
        randomAngle = rand() % 360;
        while ((randomAngle > 60 && randomAngle < 120) || (randomAngle > 240 && randomAngle < 300)){
            randomAngle = rand() % 360;
        }
        radians = randomAngle * M_PI / 180.0; // Convert degrees to radians
        mvaddch(ball->y, ball->x, ' ');
        p1->numPoints += 1;
        ball->x = gameWidth / 2;
        ball->y = rand() % (gameHeight - 1) + 1;
        ball->dir = radians;
        ball->vel = 5;
    }   //Hits top of screen
    if (ball->y < 1){
        ball->y = 1;
        ball->dir = -(ball->dir);
    }   //Hits bottom of screen
    if (ball->y > gameHeight - 1){
        ball->y = gameHeight - 1;
        ball->dir = -(ball->dir);
    }
    return 0;
}

/*
This function draws the box, paddles, ball, score, and 'Bounce'
*/
int printBoard(Ball* ball, Paddle* l, Paddle* r, Player* a, Player* b){
    drawBox(gameHeight,gameWidth);
    r->draw();
    l->draw();
    mvaddch(ball->y, ball->x, 'o');
    mvprintw(gameHeight + 2, (gameWidth / 2) - 7, "score: %d-%d", a->numPoints, b->numPoints);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK); // Red on Black)
    attron(COLOR_PAIR(1));
    mvprintw(gameHeight + 1, (gameWidth / 2) - 5, "Bounce");
    attroff(COLOR_PAIR(1));
    move(gameHeight + 2,80);
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
    drawBox(gameHeight,gameWidth);
    mvprintw(1,5, "Welcome to Bounce!");
    mvprintw(2,5, "This totally original game was created as extra c++ practice");
    mvprintw(3,5, "by me, Carter Parks!");
    mvprintw(5,5, "Player 1 is on the left; use 'e' and 'd' to move your paddle");
    mvprintw(6,5, "Player 2 is on the right; use the up and down arrows to move your paddle");
    mvprintw(8,5, "Choose Difficulty: ");
    mvprintw(9, 10, "1: Easy    2: Moderate    3: Hard");
    mvprintw(12, 5, "Press 1, 2, or 3 to choose difficulty and continue");
    refresh();
    return 0;
}

int main(int argc, char *argv[]){
    int ui, pLength, ballV, diff;
    begin();
    ui = 0;
    ui = getch();
    switch (ui){
        case '1':
            pLength = 5;
            ballV = 3;
            diff = 1;
            break;
        case '2':
            pLength = 4;
            ballV = 5;
            diff = 2;
            break;
        case '3':
            pLength = 3;
            ballV = 7;
            diff = 3;
            break;
        default:
            pLength = 4;
            ballV = 5;
            diff = 2;
    }

    clear();
    Ball ball = Ball(ballV, diff);
    Paddle leftPaddle = Paddle(8, 1, pLength);
    Paddle rightPaddle = Paddle(8, (gameWidth - 1), pLength);
    Player p1;
    Player p2;
    p1.numPoints = 0;
    p2.numPoints = 0;

    timeout(0);
    //This loop executes until the user presses 'q' or a player has 3 points
    while(ui != 'q' && p1.numPoints < 3 && p2.numPoints < 3){
        //This line effectively speeds up the game as the ball's velocity becomes faster
        usleep(400000 / (&ball)->vel);
        moveBall(&ball, &leftPaddle, &rightPaddle, &p1, &p2);
        printBoard(&ball, &leftPaddle, &rightPaddle, &p1, &p2);
        //Take user input (unbuffered)
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

    //If loop exits, clear screen, check if game was won, end program
    clear();

    if (p1.numPoints == 3){
        mvprintw(gameHeight / 2,(gameWidth / 2) - 5, "Congrats player 1! You won!");
    }
    else if(p2.numPoints ==3){
        mvprintw(gameHeight / 2,(gameWidth / 2) - 5, "Congrats player 2! You won!");
    }
    else{
        mvprintw(gameHeight / 2,(gameWidth / 2) - 5, "Game Ended");
    }
    move(gameHeight,gameWidth);
    refresh();
    usleep(1000000);
    endwin();

    return 0;
}