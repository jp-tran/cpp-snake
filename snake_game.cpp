#include<iostream>
#include<stdlib.h>
#include<curses.h> 

using namespace std;


int mapx = 40; //map width
int mapy = 20; //map height

void makeMap();
void input();
void move();

int main(){	
	initscr(); 
	cbreak(); 	
	noecho(); 
	clear(); 
	
	int maxlines, maxcols;
	maxlines = LINES - 1; 
	maxcols = COLS -1; 
	mvaddch(maxlines/3, maxcols/3, 't'); 
	
	getch(); 
	endwin(); 

	return 0;
}

// Create a map
void makeMap(){
	wrefresh(stdscr);
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
