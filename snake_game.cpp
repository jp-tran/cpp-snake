#include<iostream>
#include<stdlib.h>
#include<curses.h>
#include<unistd.h> 

using namespace std;


int mapx = 40; //map width
int mapy = 20; //map height 

void makeMap();
void input();
void move();
void printEntity(int, int[], int[]); 
void entityControl(int[], int[]); 
int main(){	
	initscr(); 
	cbreak(); 	
	noecho(); 
	clear(); 
	curs_set(0); 
	int pos[2], screen[2];
	// array containing [0] = # of lines [1] = # of cols 
	screen[0] = LINES - 1; 
	screen[1] = COLS -1; 
	// array containing [y pos, xpos] 
	pos[0] = screen[0] / 2; 
	pos[1] = screen[1] /2; 
	// print initial entity
	printEntity(1, pos, screen); 
	entityControl(pos, screen);  
	endwin(); 

	return 0;
}

void entityControl(int pos[2], int screen[2]) { 
	int ch = getch(); 
	while(ch != 101) {
		ch = getch();  
		if (ch == 119) { // w (up)
		pos[0] = pos[0] - 1;  
		printEntity(1, pos, screen); 
		} 
		else if (ch == 97) { // a (left)
		pos[1] = pos[1] - 1;  
		printEntity(1, pos, screen); 
		}
		else if (ch == 115) { // s (down) 
		pos[0] = pos[0] + 1;  
		printEntity(1, pos, screen);
		} 
		else if (ch == 100) { // d (right) 
		pos[1] = pos[1] + 1; 
		printEntity(1, pos, screen); 
		}
	}
}
void printEntity(int length, int pos[2], int screen[2]){ 
	if ((pos[0] > 0)&&(pos[0] < screen[0])&&(pos[1] > 0)&&(pos[1] < screen[1])) {
		clear();
		mvaddch(pos[0], pos[1], 'x');
		refresh;   
	} 
	 			
}
// Create a map
void makeMap(){
	system("clear");
	//create walls
	for (int i = 0; i <= mapy+1; ++i){
		for (int j = 0; j <= mapx +1; ++j){
			if (i == 0 || i == mapy+1){
				cout << "-";
			}
			else if (i > 0 && i < mapy+1){
				if (j == 0 || j == mapx+1){
					cout << "|";
				}
				else {
					cout << " ";
				}
			}
		}
		cout << endl;
	}

}

// Take input from keyboard
void input(){

}

// Logic for moving
void move(){

} 
