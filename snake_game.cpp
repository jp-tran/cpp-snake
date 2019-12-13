#include<iostream>
#include<stdlib.h>
#include<curses.h>
#include<unistd.h> 
#include<time.h>
#include<string>
#include<deque>
#include <fstream> //for reading and writing to files
#include <vector>
#include <sstream>

using namespace std;

void mainMenu();
int startGame();
bool snakeCross(deque <int>, deque <int>);
void makeMap(int[], int length, int speed);
int* spawnFood(int[]);
void move();
int printEntity(deque <int>, deque <int>, int[]); 
int entityControl(deque <int>, deque <int>, int,  int[], int[]);

int main(){	
	
	initscr();
	// init colors, define them
	start_color(); 
	init_pair(1, COLOR_RED, COLOR_BLACK); 
	init_pair(2, COLOR_GREEN, COLOR_BLACK); 
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	cbreak(); 	
	noecho(); 
	clear();
	nodelay(stdscr, TRUE);  
	curs_set(0);
    mainMenu();
	endwin(); 

	
};

//function to read in files
vector< pair<string, string> > read_from_csv(string filename){
    vector< pair<string, string> > highscores;
    // Create an input filestream
    ifstream myFile(filename);
    if (!myFile.is_open()){
        cout << "Cannot Find File " << filename << endl;
        exit(1);
    }

    // Helper vars
    string line, val, tempName, tempScore;
    int row = 0, col;

    // Read the column names
    while(getline(myFile, line)){

        // Create a stringstream from line
        stringstream ss(line);
        col = 0;
        // Extract each column name
        while(ss >> val){
            if (col == 0){
                tempName = val;};
            if (col == 1){
                tempScore = val;};
            ++col;
        };
        highscores.push_back(make_pair(tempName, tempScore));
        ++row;
    };
    return highscores;
};

//function to write to files
void write_to_csv(string filename, vector< pair<string, string> > highscores){
    // Create an output filestream object
    std::ofstream myFile(filename);

    // Send data to the stream
    for(int i = 0; i < highscores.size(); ++i)
    {
        for(int j = 0; j < 2; ++j){
            if (j == 0) {myFile << highscores[i].first;};
            if (j == 1) {myFile << highscores[i].second;};
            if(j != 1) {myFile << " ";}; // No comma at end of line
        };
        if (i != highscores.size()-1) {myFile << "\n";}; //no new line at end of file
    };

    // Close the file
    myFile.close();
};

void update_high_score(string name, int score, string filename){
    vector< pair<string, string> > highscores = read_from_csv(filename);
    score = score - 3; //disregard 3 points since snake spawns with 3
    if (score >= stoi(highscores[0].second)){ //if score >= best score
        highscores[2] = highscores[1];
        highscores[1] = highscores[0];
        highscores[0].first = name;
        highscores[0].second = to_string(score);
    }
    else if (score >= stoi(highscores[1].second)){ //if score >= second best score
        highscores[2] = highscores[1];
        highscores[1].first = name;
        highscores[1].second = to_string(score);
    }
    else if (score >= stoi(highscores[2].second)){ //if score >= third best score
        highscores[2].first = name;
        highscores[2].second = to_string(score);
    }
    write_to_csv(filename, highscores);
};

void printMainMenu(int winrow, int wincol, string title){
    WINDOW * win = newwin(winrow, wincol, 0, 0);
    box(win,ACS_VLINE,ACS_HLINE);
    refresh();
    wrefresh(win);
    mvprintw(1, 1, title.c_str());
    for (int i = 1; i < wincol-1; ++i){ //Draw horz line
        mvaddch(2, i, ACS_HLINE);
    };
};

void printSubMenu(int yloc, int xloc, string text[], int size_text, string option){
    while(1){
        // determine size of window
        int textwidth = option.length();
        for (int i = 0; i < size_text; ++i){
            if (textwidth < text[i].length()){
                textwidth = text[i].length(); //set textwidth = to length of longest str
            };
        };
        int ncols = textwidth + 6;
        int nrows = size_text + 5;
        WINDOW * subwin = newwin(nrows, ncols, yloc, xloc);
        box(subwin,ACS_VLINE,ACS_HLINE); //draw lines around window border

        //print text to sub-menu window
        for (int i = 0; i < size_text; ++i){
            int cornerx = (ncols/2) - text[i].length()/2;
            mvwprintw(subwin, i+1, cornerx, text[i].c_str());
        };

        //Draw box around button
        int cornerx = (ncols/2) - option.length()/2;
        int startdrw = cornerx -1;
        int enddrw = cornerx + option.length();
        for (int i = startdrw; i < enddrw +1; ++i){
            mvwaddch(subwin, nrows-4, i, ACS_HLINE);
            mvwaddch(subwin, nrows-2, i, ACS_HLINE);
        }
        mvwaddch(subwin, nrows-4, startdrw, ACS_ULCORNER);
        mvwaddch(subwin, nrows-4, enddrw, ACS_URCORNER);
        mvwaddch(subwin, nrows-3, startdrw, ACS_VLINE);
        mvwaddch(subwin, nrows-3, enddrw, ACS_VLINE);
        mvwaddch(subwin, nrows-2, startdrw, ACS_LLCORNER);
        mvwaddch(subwin, nrows-2, enddrw, ACS_LRCORNER);

        //print action button (e.g. OK button)
        wattron(subwin,A_REVERSE);
        mvwprintw(subwin, nrows - 3, cornerx, option.c_str());
        refresh();
        wrefresh(subwin);
        int choice = getch();
        if (choice == 10){
            clear();
            break;
        }

    }
}

void mainMenu(){
    string filename = "highscores.txt"; //file with highscores
    // Create a main menu window
    int winrow = 10, wincol = 15;
    string title = "MAIN MENU";
    int num_choices = 4;
    string choices[4] = {"START", "INSTRUCTIONS", "HIGH SCORES", "QUIT"};
    int choice;
    int startind = 2;
    int highlight = startind + 1;

    printMainMenu(winrow, wincol, title);
    keypad(stdscr, true); // get arrow keys

    while(1){
        for(int i = startind+1; i < num_choices+startind+1; i++){
            if(i == highlight){
                attron(A_REVERSE); //reverses fore/background colors
            }
            mvprintw(i, 1, choices[i-startind-1].c_str()); //print choice
            attroff(A_REVERSE);
        }
        choice = getch();

        switch(choice){
            case KEY_UP:
                highlight--;
                if (highlight < startind+1){
                    highlight = startind+num_choices;
                }
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight > startind+num_choices){
                    highlight = startind+1;
                }
                break;
            case 10: // If Enter key is pressed
                //If user selects START
                if (highlight == 3){
                    // Read in high scores and start game
                    curs_set(1); //makes cursor visible
                    echo(); //allows auto echoing of characters
                    string mesg = "Enter name: ";
                    char input[10];
                    int row,col;
                    refresh();
                    WINDOW * win = newwin(winrow, 30, 0, 0);
                    refresh();
                    mvwprintw(win,0,0,mesg.c_str());
                    wgetnstr(win, input, 10);
                    wrefresh(win);
                    string name(input); //convert char to string
                    curs_set(0);
                    noecho();
                    int score = startGame();
                    update_high_score(name, score, filename);
                    clear();
                    printMainMenu(winrow, wincol, title);
                };
                //If user selects INSTRUCTIONS
                if (highlight == 4){
                    string InstText[2];
                    InstText[0] = "Use arrow keys to move";
                    InstText[1] = "Press s/f to go slower/faster";
                    string InstOK = "OK";
                    int nrows = 5, ncols = 20;
                    printSubMenu(highlight-1, wincol, InstText, 2, InstOK);
                    printMainMenu(winrow, wincol, title);
                };
                //If user selects HIGH SCORES
                if (highlight == 5){
                    string scoreButton = "OK";
                    vector< pair<string, string> > highscores = read_from_csv(filename);
                    string scoreArr[4];
                    scoreArr[0] = "TOP 3 SCORES";
                    //Determine longest name and score
                    int namelen = highscores[0].first.length();
                    int scorelen = highscores[0].second.length();
                    for (int i = 1; i < 4; ++i){
                        if (namelen < highscores[i-1].first.length()){
                            namelen = highscores[i-1].first.length();}
                        if (scorelen < highscores[i-1].second.length()){
                            scorelen = highscores[i-1].second.length();}
                    }
                    for (int i = 1; i < 4; ++i){ //pad strings with spaces
                        if (scorelen > highscores[i-1].second.length()){
                            string tempscore = highscores[i-1].second;
                            for (int j = 0; j < scorelen -highscores[i-1].second.length(); ++j){
                                tempscore = " " + tempscore;
                            }
                            highscores[i-1].second = tempscore;
                        }
                        if (namelen > highscores[i-1].first.length()){
                            string tempstr = highscores[i-1].first;
                            for (int j = 0; j < namelen -highscores[i-1].first.length(); ++j){
                                tempstr = tempstr + " ";
                            }
                            highscores[i-1].first = tempstr;
                        }
                        scoreArr[i] = highscores[i-1].first + " " + highscores[i-1].second;
                    }
                    printSubMenu(0, wincol, scoreArr, 4, scoreButton);
                    highscores.clear();
                    printMainMenu(winrow, wincol, title);
                };
                break;
            default:
                break;
        }
        //quit the game
        if (choice == 10 && highlight == startind+num_choices){
            break;
        }
    }
}

int startGame(){
    clear();
    int direction, screen[2];
	deque <int> xpos;
	deque <int> ypos;
	// array containing [0] = # of lines [1] = # of cols
	screen[0] = LINES /2;
	screen[1] = COLS /2;
	// 2 deques and 1 int defined,  containing xpos, y pos, and direction where
	// direction = 1 - facing right
	// direction = 2 - facing left
	// direction = 3 - facing up
	// direction = 4 - facing down
	xpos.push_back(screen[1]/2);
	ypos.push_back(screen[0]/2);
	// add two segments
	xpos.push_back(screen[1]/2 - 1);
	ypos.push_back(screen[0]/2);
	xpos.push_back(screen[1]/2 -2);
	ypos.push_back(screen[0]/2);
	direction = 1;
	// print initial entity, map, and food
	makeMap(screen, 3, 1);
	printEntity(xpos,ypos,screen);
	int* foodPos = spawnFood(screen); //get location of food
	//initialize food, set color 
	attron(COLOR_PAIR(3));
	mvaddch(foodPos[0], foodPos[1], 'O');
	attroff(COLOR_PAIR(3)); 
	refresh;
	int score = entityControl(xpos, ypos, direction, screen, foodPos);
	// Print Game Over Pop Up
	string goText[2];
	goText[0] = "GAME OVER";
	goText[1] = "Your score: " + to_string(score-3);
	string goOption = "GO TO MAIN MENU";
	int nrows = 5, ncols = 20;
    printSubMenu((screen[0]/2) - (nrows/2), screen[1]/2, goText, 2, goOption);
    return score;
};

int entityControl(deque <int> xpos, deque <int> ypos, int direction, int screen[2], int foodPos[2]) {
	keypad(stdscr, true); // get arrow keys
	int ch = getch(); 
	int speed = 1; 
	double initial_time = 1000000; 
	while(ch != 101) {
		usleep(initial_time/speed); // sleep for .5 seconds 
		ch = getch();
		if(xpos.size() == ypos.size() && xpos.size() > 1) {
			for(int i = xpos.size()-1; i > 0; i--) { 
				xpos[i] = xpos[i-1]; 
				ypos[i] = ypos[i-1]; 
			}
		}
		switch(ch)  
		{
			case KEY_UP:  // w (up)
				if(direction != 4) {
					direction = 3; 
				}
				break;
		
			case KEY_LEFT:  // a (left)
				if(direction != 1){ 
					direction = 2; 
				}
				break;  
		
			case KEY_DOWN:  // s (down) 
				if(direction !=3) {
					direction = 4;
				}
				break;    
		
			case KEY_RIGHT:  // d (right) 
				if(direction != 2) {
					direction = 1;
				}
				break;
			case 102: // t (increase speed up to max of 10) 
				if (speed <10){
				speed = speed + 1;
				} 
				break;  
			case 115: // g (decrease speed up to min of 1) 
				if (speed > 1) { 
				speed = speed - 1; 
				} 
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
		makeMap(screen, xpos.size(), speed);
		if(printEntity(xpos, ypos, screen) != 1){} 
		else if(printEntity(xpos, ypos, screen) == 1) {
			return xpos.size(); //when snake collides with a wall or itself
		}
		if(xpos[0] == foodPos[1] && ypos[0] == foodPos[0]) {
				xpos.push_back(xpos[0]); 
				ypos.push_back(ypos[0]); 
				foodPos = spawnFood(screen); 
		}
		else {
			attron(COLOR_PAIR(3)); 
			mvaddch(foodPos[0], foodPos[1], 'O');
			attroff(COLOR_PAIR(3));
		}
		refresh; 
	}
	return xpos.size(); //in case user quits prematurely
}
int printEntity(deque <int> xpos, deque <int> ypos, int screen[2]){ 
	if ((ypos[0] > 0)&&(ypos[0] < screen[0] + 1)&&(xpos[0] > 0)&&(xpos[0] < screen[1]+1) && !snakeCross(xpos, ypos)) {
		attron(COLOR_PAIR(1)); 
		mvaddch(ypos[0], xpos[0], '#');
		attroff(COLOR_PAIR(1));
		if(xpos.size() == ypos.size() && xpos.size() > 1) {
			for(int i = 1; i < xpos.size(); i++) { 
				attron(COLOR_PAIR(2));
				mvaddch(ypos[i], xpos[i], 'x');
				attroff(COLOR_PAIR(2)); 
			}
		}
		return 0;   
	}
	else {
		return 1; 
	} 			
}
// snakecross - checks if head is crossing over any part of the snake 
// returns true if snake is crossing which ends game. otherwise returns false
bool snakeCross(deque <int> xpos, deque <int> ypos) { 
	for(int i=1; i<xpos.size(); i++) {
		if(xpos[0] == xpos[i] && ypos[0] == ypos[i]) { 
			return true; 
		} 
	} 
	return false;
} 
// Create a map
void makeMap(int screen[2], int length, int speed){
	//create walls
	for (int i = 0; i <= screen[0]+1; ++i){
		for (int j = 0; j <= screen[1]+1; ++j){
			if (i == 0 && j == 0) {  // upper left corner
				mvaddch(i,j,ACS_ULCORNER); 
			} 
			else if (i == 0 && j == screen[1]+1) { // upper right corner
				mvaddch(i,j,ACS_URCORNER); 
			}  
			else if (i == 0 || i == screen[0]){
				mvaddch(i,j,ACS_HLINE);
				
			}
			else if (i > 0 && i < screen[0] + 1){
				if (j == 0 || j == screen[1]+1){
					mvaddch(i,j,ACS_VLINE);
				}
			}
		}
	}
	
	for (int j = 1; j <=screen[1]; ++j) {
		mvaddch(screen[0]+2, j, ACS_HLINE); 
	} 
	
	mvaddch(screen[0], 0, ACS_LTEE); 
	mvaddch(screen[0], screen[1]+1, ACS_RTEE); 
	mvaddch(screen[0]+2,0,ACS_LLCORNER); 
	mvaddch(screen[0]+2,screen[1]+1, ACS_LRCORNER);
	// add gui on bottom
	mvaddch(screen[0]+1, 0, ACS_VLINE); 
	mvaddch(screen[0]+1, screen[1]+1, ACS_VLINE); 
	mvaddstr(screen[0]+1, screen[1]/4, "Score: ");
	mvaddstr(screen[0]+1, screen[1]/4 + 8, to_string(length-3).c_str());
	mvaddstr(screen[0]+1, screen[1]*3/4, "Speed: "); 
	mvaddstr(screen[0]+1, screen[1]*3/4 + 8, to_string(speed).c_str()); 
}

// Creates a new food and returns its location
int* spawnFood(int screen[2]){
	static int foodPos[2];
	srand(time(NULL));
	foodPos[0] = (rand() %(screen[0] -1)) + 1;
	foodPos[1] = (rand() %(screen[1] -1)) + 1;
	return foodPos;
};

