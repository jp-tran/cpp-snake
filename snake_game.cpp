#include<iostream>
#include<stdlib.h>
#include<curses.h>
#include<unistd.h> 
#include<time.h>
#include<deque>

using namespace std;

void makeMap(int[]);
int* spawnFood(int[]);
void move();
int printEntity(deque <int>, deque <int>, int[]); 
void entityControl(deque <int>, deque <int>, int,  int[], int[]); 
int main(){	
	initscr();
	cbreak(); 	
	noecho(); 
	clear();
	nodelay(stdscr, TRUE);  
	curs_set(0); 
	int direction, screen[2];
	deque <int> xpos; 
	deque <int> ypos; 
	// array containing [0] = # of lines [1] = # of cols 
	screen[0] = LINES - 40; 
	screen[1] = COLS - 70; 
	// 2 deques and 1 int defined,  containing xpos, y pos, and direction where 
	// direction = 1 - facing right 
	// direction = 2 - facing left 
	// direction = 3 - facing up 
	// direction = 4 - facing down
	xpos.push_back(screen[1]/2); 
	ypos.push_back(screen[0]/2); 
	direction = 1;
	// print initial entity, map, and food
	makeMap(screen);
	printEntity(xpos,ypos,screen);
	int* foodPos = spawnFood(screen); //get location of food
	mvaddch(foodPos[0], foodPos[1], 'O');
	refresh;
	entityControl(xpos, ypos, direction, screen, foodPos);  
	endwin(); 

	
}

void entityControl(deque <int> xpos, deque <int> ypos, int direction, int screen[2], int foodPos[2]) { 
	int ch = getch(); 
	while(ch != 101) {
		usleep(500000); // sleep for .5 seconds 
		ch = getch();
		switch(ch)  
		{
			case 119:  // w (up)
				direction = 3; 
				break;
		
			case 97:  // a (left)
				direction = 2; 
				break;  
		
			case 115:  // s (down) 
				direction = 4;
				break;    
		
			case 100:  // d (right) 
				direction = 1;
				break; 
			default: 
				break;  
		}
		switch (direction) 	
		{ 
			case 1: 
				xpos[0] = xpos[0] + 1;
				break; 
			case 2: 
				xpos[0] = xpos[0] - 1;
				break; 
			case 3: 
				ypos[0] = ypos[0] - 1;
				break; 
			case 4: 
				ypos[0] = ypos[0] + 1;
				break; 
			default:
				break; 
		} 
		clear();	
		makeMap(screen);
		if(printEntity(xpos, ypos, screen) != 1){} 
		else if(printEntity(xpos, ypos, screen) == 1) {
			return; 
		}
		mvaddch(foodPos[0], foodPos[1], 'O');
		refresh; 
	}
}
int printEntity(deque <int> xpos, deque <int> ypos, int screen[2]){ 
	if ((ypos[0] > 0)&&(ypos[0] < screen[0])&&(xpos[0] > 0)&&(xpos[0] < screen[1])) {
		//clear();
		mvaddch(ypos[0], xpos[0], 'x');
		//refresh; 
		return 0;   
	}
	else {
		return 1; 
	} 			
}
// Create a map
void makeMap(int screen[2]){
	//create walls
	for (int i = 0; i <= screen[0]+1; ++i){
		for (int j = 0; j <= screen[1]+1; ++j){
			if (i == 0 || i == screen[0]){
				mvaddch(i,j,'-');
			}
			else if (i > 0 && i < screen[0]+1){
				if (j == 0 || j == screen[1]+1){
					mvaddch(i,j,'|');
				}
				else {
					mvaddch(i,j,' ');
				}
			}
		}
	}
}

// Creates a new food and returns its location
int* spawnFood(int screen[2]){
	static int foodPos[2];
	srand(time(NULL));
	foodPos[0] = rand() %screen[0] +1;
	foodPos[1] = rand() %screen[1] +1;
	return foodPos;
};
