# Snake Game 
### Stephen Koch (skoch) and Joseph Tran (ptran3)
### CSE 20232

The main program is in `snake_game.cpp` and requires the following dependencies: <br>
- stdlib.h
- curses.h
- unistd.h
- time.h
- string
- deque
- fstream
- vector
- sstream
- iostream (for debugging purposes)

To compile: <br>
`g++ -std=c++11 snake_game.cpp -o snake_game -lncurses`

Note that `highscores.txt` also needs to be in the working directory and should be filled with three high scores.
Otherwise the program will throw a segmentation fault.
