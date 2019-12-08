#include<iostream>
#include<stdlib.h>
#include<curses.h>
#include<unistd.h> 
#include<time.h>

using namespace std;

void makeMap(int[]);
int* spawnFood(int[]);
void move();
void printEntity(int, int[], int[]); 
void entityControl(int[], int[], int[]); 
int main(){	
	initscr();
	cbreak(); 	
	noecho(); 
	clear(); 
	curs_set(0); 
	int pos[3], screen[2];
	// array containing [0] = # of lines [1] = # of cols 
	screen[0] = LINES - 40; 
	screen[1] = COLS - 70; 
	// array containing [y pos, xpos, directoion], where 
	// direction = 1 - facing right 
	// direction = 2 - facing left 
	// direction = 3 - facing up 
	// direction = 4 - facing down
	pos[0] = screen[0] / 2; 
	pos[1] = screen[1] /2; 
	pos[2] = 1; 
	// print initial entity, map, and food
	makeMap(screen);
	printEntity(1, pos, screen);
	int* foodPos = spawnFood(screen); //get location of food
	mvaddch(foodPos[0], foodPos[1], 'O');
	refresh;
	entityControl(pos, screen, foodPos);  
	endwin(); 

	return 0;
}

void entityControl(int pos[3], int screen[2], int foodPos[2]) { 
	int ch = getch(); 
	while(ch != 101) {
		usleep(500000); // sleep for .5 seconds 
		ch = getch();  
		if (ch == 119) { // w (up)
		pos[2] = 3; 
		} 
		else if (ch == 97) { // a (left)
		pos[2] = 2;   
		}
		else if (ch == 115) { // s (down) 
		pos[2] = 4;   
		} 
		else if (ch == 100) { // d (right) 
		pos[2] = 1; 
		}
		switch (pos[2]) 	
		{ 
			case 1: 
				pos[1] = pos[1] + 1;
				break; 
			case 2: 
				pos[1] = pos[1] - 1;
				break; 
			case 3: 
				pos[0] = pos[0] - 1;
				break; 
			case 4: 
				pos[0] = pos[0] + 1;
				break; 
			default:
				break; 
		} 
		clear();	
		makeMap(screen);
		printEntity(1, pos, screen);
		mvaddch(foodPos[0], foodPos[1], 'O');
		refresh; 
	}
}
void printEntity(int length, int pos[3], int screen[2]){ 
	if ((pos[0] > 0)&&(pos[0] < screen[0])&&(pos[1] > 0)&&(pos[1] < screen[1])) {
		//clear();
		mvaddch(pos[0], pos[1], 'x');
		//refresh;   
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
