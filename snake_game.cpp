#include<iostream>
#include<ncurses.h>
using namespace std;


int mapx = 40; //map width
int mapy = 20; //map height

void makeMap();
void input();
void move();

int main(){
	makeMap();
	return 0;
}

// Create a map
void makeMap(){
	wrefresh(stdscr);
	//system("clear"); //clear terminal (on Linux)
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
