#include <curses.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//compiler vs linker
//linker links all the libraries and packages them
//different from java or .NET which the libraries are in the virtual machine

struct pos {
    int row;
    int col;
};

char direction[10] = "";
char trophychar;
int keepGoing = 1;
int snakeLength = 3;
int snakeSpeed;
int perimeter;
struct pos snakeCells[100];
struct pos trophyPos;

// Michael
// Draws the 
void drawSnake(){
    attron(COLOR_PAIR(1));
    //Draw New Head (which is at snakeCells[0])
    move(snakeCells[0].row,snakeCells[0].col);
    addch('@');
    move(snakeCells[1].row,snakeCells[1].col);
    addch('O');
    //Erase Tail (which is at snakeCells[snakeLength])
    move(snakeCells[snakeLength].row,snakeCells[snakeLength].col);
    addch(' ');
    attroff(COLOR_PAIR(1));
}

//Marek
//this function submits 
void insertHead(int row, int col) {
    struct pos newHead;
    newHead.row = row;
    newHead.col = col;
    //shift
    for(int i = snakeLength; i > 0; i--){
        snakeCells[i] = snakeCells[i-1];
    }
    snakeCells[0] = newHead;
    drawSnake();
}

//Michael
void createBorders(){
    // Create the horizontal borders
    for(int h = 0; h < COLS; h++) {
        move(0,h);
        addch('-');
        move(LINES-1,h);
        addch('-');
    }

    // Create the vertical borders
    for(int v = 0; v < LINES; v++) {
        move(v,0);
        addch('|');
        move(v,COLS-1);
        addch('|');
    }

    // Used to determine win condition
    perimeter = LINES*2 + COLS*2;
}

//Michael
void relative_move(int row, int col){
    insertHead(snakeCells[0].row+row,snakeCells[0].col+col);
}
void generateTrophy(){
    int trophyInSnake = 1;
    //make sure trophy
    while(trophyInSnake) {
        trophyPos.row = rand() % (LINES-2) + 1;
        trophyPos.col = rand() % (COLS-2) + 1;
        trophyInSnake = 0;
        //check if the trophy is in the snake
        for(int i = 0; i<snakeLength; i++) {
            if (trophyPos.row == snakeCells[i].row && trophyPos.col == snakeCells[i].col) {
                trophyInSnake = 1;
            }
        }
    }
    
    //Generate random 1-9 for trophy val
    switch (rand() % 9+1) {
        case 1: trophychar ='1';
        break;
        case 2: trophychar ='2';
        break;
        case 3: trophychar ='3';
        break;
        case 4: trophychar ='4';
        break;
        case 5: trophychar ='5';
        break;
        case 6: trophychar ='6';
        break;
        case 7: trophychar ='7';
        break;
        case 8: trophychar ='8';
        break;
        case 9: trophychar ='9';
        break;
    }
    move(trophyPos.row,trophyPos.col);
    addch(trophychar);
}

void increaseSnakeLength(){
    int oldsnakeLength = snakeLength;
    snakeLength += trophychar-'0';
    for(int i = oldsnakeLength; i < snakeLength; i++) {
        snakeCells[i].row = snakeCells[oldsnakeLength].row;
        snakeCells[i].col = snakeCells[oldsnakeLength].col;
    }
    move(0,0);
    printw("Score %3d ", snakeLength);
    generateTrophy();
}

// Marek
void checkCollision(){
    //Check if trophy is touched, increase snake length
    if(snakeCells[0].row == trophyPos.row && snakeCells[0].col == trophyPos.col){
        increaseSnakeLength();
    }
    //head is border?
    if(snakeCells[0].row == 0 || snakeCells[0].row == LINES-1){
        keepGoing = 0;
        move(LINES/2,COLS/2);
        attron(COLOR_PAIR(3));
        printw("YOU LOSE");
        refresh();
        attroff(COLOR_PAIR(3));
        sleep(3);
        endwin();
    }
    if(snakeCells[0].col == 0 || snakeCells[0].col == COLS-1){
        keepGoing = 0;
        move(LINES/2,COLS/2);
        attron(COLOR_PAIR(3));
        printw("YOU LOSE");
        refresh();
        attroff(COLOR_PAIR(3));
        sleep(3);
        endwin();
    }
    //head in self?
    for(int i = snakeLength-1; i > 0; i--){
        if(snakeCells[0].row == snakeCells[i].row && snakeCells[0].col == snakeCells[i].col){
            keepGoing= 0;
            move(LINES/2,COLS/2);
            attron(COLOR_PAIR(3));
            printw("YOU LOSE");
            refresh();
            attroff(COLOR_PAIR(3));
            sleep(3);
            endwin();
        }
    }
}

//Marek
void checkWin(){
    if(snakeLength >= perimeter/2){
        move(LINES/2,COLS/2);
        attron(COLOR_PAIR(4));
        printw("YOU WIN!");
        refresh();
        sleep(1);
        attroff(COLOR_PAIR(4));
        move(LINES/2,COLS/2);
        attron(COLOR_PAIR(2));
        printw("YOU WIN!");
        refresh();
        sleep(1);
        attroff(COLOR_PAIR(2));
        move(LINES/2,COLS/2);
        attron(COLOR_PAIR(4));
        printw("YOU WIN!");
        refresh();
        sleep(5);
        keepGoing = 0;
        endwin();
    }
}

//Marek
void initializeSnake(){
    move(LINES/2,COLS/2);
    int snakeRow = LINES/2;
    int snakeCol = COLS/2;
        switch (rand() % 4+1){
        case 1:
        //direction right
            snakeCells[0].row = snakeRow;
            snakeCells[0].col = snakeCol;
            //addch('@');
            snakeCells[1].row = snakeRow;
            snakeCells[1].col = snakeCol-1;
            //addch('@');
            snakeCells[2].row = snakeRow;
            snakeCells[2].col = snakeCol-2;
            //addch('@');
            strcpy(direction,"RIGHT");
        break;
        case 2:
        //direction left
            snakeCells[0].row = snakeRow;
            snakeCells[0].col = snakeCol;
            //addch('@');
            snakeCells[1].row = snakeRow;
            snakeCells[1].col = snakeCol+1;
            //addch('@');
            snakeCells[2].row = snakeRow;
            snakeCells[2].col = snakeCol+2;
            //addch('@');
            strcpy(direction,"LEFT");
        break;
        case 3:
        //direction up
            snakeCells[0].row = snakeRow;
            snakeCells[0].col = snakeCol;
            //addch('@');
            snakeCells[1].row = snakeRow+1;
            snakeCells[1].col = snakeCol;
            //addch('@');
            snakeCells[2].row = snakeRow+2;
            snakeCells[2].col = snakeCol;
            //addch('@');
            strcpy(direction,"UP");
        break;
        case 4:
        //direction down
            snakeCells[0].row = snakeRow;
            snakeCells[0].col = snakeCol;
            //addch('@');
            snakeCells[1].row = snakeRow-1;
            snakeCells[1].col = snakeCol;
            //addch('@');
            snakeCells[2].row = snakeRow-2;
            snakeCells[2].col = snakeCol;
            //addch('@');
            strcpy(direction,"DOWN");
        break;
    }

    //initialize keyboard
    curs_set(0); //setting to 0 hides the blinking cursor
    noecho(); //no inputs will be echoed to the terminal
    keypad(stdscr, TRUE); //sets up the keyboard so all the inputs will be captured
    createBorders();
    generateTrophy();

    //Initialize score top right
    move(0,0);
    printw("Score %3d ",snakeLength);
}

// Marek & Michael
int main(int ac, char *av[]) {
    int snakeSpeed = 1;
    int timePassed = 0;
    int trophyTime = 9; //initialize beginning trophy time to be 9 seconds
    
    initscr(); //initializes the ncurses library
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    clear(); //clears the screen

    // Initialize random number generator (pass in the time for the seed)
    time_t t;
    srand((unsigned) time(&t));
    
    nodelay(stdscr, TRUE);
    initializeSnake();
    //char direction[10] = "RIGHT"; // Default direction is RIGHT

    // Main game loop
    while(keepGoing)
    {
        int ch = getch();
        switch(ch){
            case KEY_UP:
                //addstr("You pressed the UP arrow               ");
                strcpy(direction,"UP");
                break;
            case KEY_DOWN:
                //addstr("You pressed the DOWN arrow               ");
                strcpy(direction,"DOWN");
                break;
            case KEY_LEFT:
                //addstr("You pressed the LEFT arrow               ");
                strcpy(direction,"LEFT");
                break;
            case KEY_RIGHT:
                //addstr("You pressed the RIGHT arrow               ");
                strcpy(direction,"RIGHT");
                break;
        }
        if(strcmp(direction,"UP")==0){
            relative_move(-1,0);
        }
        else if(strcmp(direction,"DOWN")==0){
            relative_move(1,0);
        }
        else if(strcmp(direction,"LEFT")==0){
            relative_move(0,-1);
        }
        else if(strcmp(direction,"RIGHT")==0){
            relative_move(0,1);
        }

        checkCollision();
        checkWin();
        //usleep works in microseconds (1 sec = 1,000,000 microseconds)
        int sleepTime = 160000 - (snakeLength*1000);
        if(sleepTime < 20000) { //this if statement prevents sleepTime from going less than 20000;
            sleepTime = 20000; 
        }
        usleep(sleepTime);

        //timePassed incremented by the value passed to usleep
        timePassed += sleepTime;
        
        if(timePassed > (trophyTime * 1000000)) { //this if statement is true after (trophyTime * 1) second(s) has passed
            //remove the old trophy
            move(trophyPos.row,trophyPos.col);
            addch(' ');
            //create new trophy
            generateTrophy();
            timePassed = 0;
            trophyTime = rand()%9+3; //range 3 to 12
        }
        //refresh
        refresh();
    }
    endwin(); //close gracefully

    return 0;
}