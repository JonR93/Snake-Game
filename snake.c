
// compile with gcc snake.c -lncurses -o snake 

#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define DRAW_SPEED 100000
#define MAX_SNAKE_LENGTH 20
#define WIN_MSG "YOU WIN!"
#define LOSE_MSG "GAME OVER!"

void drawSnake(void);
void eraseTail(void);
void moveSnake(char direction);
void spawnFood(void);
void killSnake(void);

struct bodyPart // linkable body parts of the snake
{       
	int x; //location
	int y;
	struct bodyPart *next; //link to next body part
};

struct pickUp // pickUP object aka food
{
	int x; // location
	int y;
};

struct bodyPart *tail; // tail of the snake
struct bodyPart *body; // body of the snake 
struct bodyPart *head; // head of the snake 
struct pickUp food; // food pick up item

char dir; // direction snake is moving
long speed=DRAW_SPEED; // game rendering speed
int bodyToBeDrawn=0; // number of snake body parts to still be printed
int snakeLength=3; // begining length 3. (head, body, and tail)
int gameOver=0, gameWon=0; // (bools) game has not been won or lost yet
char snakeHead=219,snakeBody=219,deadHead='X'; //chars to be drawn on screen
int growthValue=0;

int main(int argc, char *argv[])
{
	int x,y;
	srandom((unsigned)time(NULL)); //random number generator seeded with the current time 
	initscr(); // init screen for the use of curses
        curs_set(0); // Don't show cursor
	nonl(); // ignore return
	cbreak(); // don't let getch() wait for a newline 
	noecho(); // don't echo
	keypad(stdscr,1); // enable _KEY defs
	nodelay(stdscr,1); // getch() returns ERR if no key is pressed
	LINES--; // number of rows
	COLS--;  // number of columns
	dir='R'; // direction key: up='U', right='R', down='D', left='L'
 
	for(x=1;x<COLS;x++) // draw background 
	{
		for(y=1;y<LINES;y++)
		{
			move(y,x);
			addch(' '); //background
		}
	}
	// draw border 
	for(x=0;x<LINES;x++) // draw left border 
	{
		move(x,0);
		addch('*'); //print border
	}
	for(x=0;x<LINES;x++) // draw right border
	{
		move(x,COLS);
		addch('*'); //print border
	}
        for(x=0;x<COLS;x++) // draw top border 
	{
		move(0,x);
		addch('*'); // print border
	}
	for(x=0;x<COLS;x++) // draw bottom border
	{
		move(LINES,x);
		addch('*'); // print border
	}
	
	move(LINES,COLS);
	
	// create snake 
	// create end of the tail
	tail=(struct bodyPart *)malloc(sizeof(struct bodyPart));
	tail->x=1;
	tail->y=1;
	// create body of the snake
	tail->next=(struct bodyPart *)malloc(sizeof(struct bodyPart)); // find memory for first bodyPart of the tail
	body=tail->next; // store memory address of first bodyPart of the tail in body
	body->x=2;
	body->y=1;
        // create snake head 
	body->next=(struct bodyPart *)malloc(sizeof(struct bodyPart)); // find memory for head of the snake
	body=body->next;
	body->x=3; // set x cord for head
	body->y=1; // set y cord for head 
	body->next=NULL;
	// display the snake
	drawSnake();
	// spawn first food
	spawnFood();
	// game loop
	while(1)
	{
		x=getch();
		//  sleep 
		usleep(speed);
		// user input to control snake
		switch(x)
		{
			case KEY_UP:
				// move up
				moveSnake('U');
				break;
			case KEY_RIGHT:
				// move right 
				moveSnake('R');
				break;
			case KEY_DOWN:
				// move down 
				moveSnake('D');
				break;
			case KEY_LEFT:
				// move left 
				moveSnake('L');
				break;	
			case ERR:
				// no key was pressed
				moveSnake(dir); //move in current direction
				break;
			default:
				// unknown key was pressed 
				moveSnake(dir); //move in current direction
				break;
		}
		// reset curser position
		move(0,0);
		// refresh screen 
		refresh();
                //check if the snake has gotten too long (winning condition)
        	if(snakeLength>MAX_SNAKE_LENGTH)
        	{
                	gameWon=1; // gameWon = TRUE	
                	killSnake(); // end the game.
        	}		
	}
}

//animate snake
void moveSnake(char direction) 
{
	//dont allow snake to turn backwards and die
	if((direction=='U' && dir=='D') || (direction=='R' && dir=='L') || (direction=='D' && dir=='U') || (direction=='L' && dir=='R'))
	{
		moveSnake(dir); // move in current direction
		return;
	}
	// remove the end of the tail of the snake
	if(bodyToBeDrawn==0)
		eraseTail();
	else
		bodyToBeDrawn--;
	// update snake 
	body=tail;
	while(body->next != NULL)
		body=body->next;
	body->next=(struct bodyPart *)malloc(sizeof(struct bodyPart));
	head=body->next;
	head->next=NULL;
	switch(direction) // up='U', right='R', down='D', left='L' 
	{
		case 'U': // up
			head->x=body->x;
			head->y=body->y-1;
			break;
		case 'R': // right 
			head->x=body->x+1;
			head->y=body->y;
			break;
		case 'D': // down 
			head->x=body->x;
			head->y=body->y+1;
			break;
		case 'L': //left
			head->x=body->x-1;
			head->y=body->y;
			break;
	}
	dir=direction;
	// draw new head 
	move(body->y,body->x);
	addch(snakeBody); // print snakes tail/body
	move(head->y,head->x);
	addch(snakeHead); // print snakes head
	// check for food collision
	if(head->x==food.x && head->y==food.y)
	{
		bodyToBeDrawn+=growthValue; //grow the snake to value of the food
		spawnFood();  // spawn a new food
                snakeLength+=growthValue; // snake grows longer
	}
	// check for border collision
	if((head->x == 0) || (head->x == COLS) || (head->y == 0) || (head->y == LINES))
        {
        	gameOver=1; //gameOVER = TRUE
		killSnake();
        }
	// check if the snake collided with itself
	body=tail;
	while(body->next != NULL)
	{
		if(head->x==body->x && head->y==body->y)
                {
                        gameOver=1; //gameOVER = TRUE
			killSnake();
                }
		body=body->next;
	}
}

// end the game
void killSnake(void) 
{
	move(head->y,head->x); //move curser to head of snake
	
        if(gameOver) 
        {
                addch(deadHead); //show snake has died
        	move(LINES/2-2,(COLS-sizeof(LOSE_MSG))/2); 
		printw(LOSE_MSG); // print msg in center of screen
        }
        if(gameWon)
        {
        	move(LINES/2-2,(COLS-sizeof(WIN_MSG))/2);
		printw(WIN_MSG); // print msg in center of screen
        }
	move(0,0); //reset curser
	refresh();
	// wait for input 
	nodelay(stdscr,0);
        while(1){}// game is paused, ctrl+c closes the game
}

// render snake on screen 
void drawSnake(void)
{
	int x=1;
	body=tail;
	while(x)
	{
		move(body->y,body->x);
		if(body->next==NULL)
		{
			addch(snakeHead); // print snakes head
			x=0;
		}
		else
		{
			addch(snakeBody); //print snakes tail
			body=body->next;
		}
	}
}

// remove the end of the snakes tail for animation purposes
void eraseTail(void)
{
	body=tail; // store memmory location of the current tail in body 
	tail=tail->next; // make tail point to the next bodyPart of the tail
	move(body->y,body->x); // move to body 
	addch(' '); // print background on body
	free(body); // free whats in body
}

void spawnFood(void)
{
	int foodNum=(random()%9)+1; // value of food (1-9)
        char foodChar =(char)(((int)'0')+foodNum); // will grow snake by food value
        int run=1;
        growthValue = foodNum;
	while(run) // get x,y cords of a space that is not taken by snake body
	{
		run=0;
		food.x=random()%(COLS-2)+1;
		food.y=random()%(LINES-2)+1;
		body=tail;
		while(body->next!=NULL)
		{
			if((body->x==food.x) && (body->y==food.y))
				run=1;
			body=body->next;
		}
	}
	move(food.y,food.x);
	addch(foodChar); // print food to screen
}


