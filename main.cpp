#include <iostream>
#include<cstdlib>
#include<ctime>

const int DIMX = 6;

const int DIMY = 6;

const int MINES = 6;

const int MINE = -1;

const char COVERED = 'X';

const char UNCOVERED = ' ';

const char FLAG = 'F';

int state[DIMX][DIMY];

char display[DIMX][DIMY];

void init(); // initialize game states

int countSorroundingMines(int x, int y); // helper function used by init

void reveal(int x, int y);

void player(); // handle player input

void setFlag();

void uncover();

void print(); // print the minefield

void cheat(); // print out the mines

bool isWin();

bool isLose();

bool playNewGame();

int main() {
    std::cout << "Welcome to MineSweeper!" << std::endl;
    init();
    bool playing = true;
    while(playing) {
        print();
        player();
        if(isWin()) {
            std::cout << "You Win!" << std::endl;
            playing = false;
        } else if(isLose()) {
            std::cout << "You Lose!" << std::endl;
            playing = false;
        }

        if(!playing) {
            playing = playNewGame();
        }
    }
    std::cout << "Exiting Minesweeper!" << std::endl;
    return 0;
}

void init() {
    std::cout << "New Game!" << std::endl;
    // set display to "uncovered"
    for(int y = 0; y < DIMY; y++) {
        for(int x = 0; x < DIMX; x++) {
            display[x][y] = COVERED;
        }
    }
    // initialize mines
    srand(time(0));
    for(int i = 0; i < MINES; i++) {
        bool placed = false;
        while(!placed) {
            int x = rand() % DIMX;
            int y = rand() % DIMY;
            if(state[x][y] != MINE) { // check mine not set
                placed = true;
                state[x][y] = MINE;
            }
        }
    }
    // place hint numbers
    for(int y = 0; y < DIMY; y++) {
        for(int x = 0; x < DIMX; x++) {
            state[x][y] = countSorroundingMines(x, y);
        }
    }
}

bool playNewGame() {
    bool selected = false;
    while(!selected) {
        char choice;
        std::cout << "Play another game? Y or N" << std::endl;
        std::cin >> choice;
        if(choice == 'N' || choice == 'n') {
            return false;
            selected = true;
        } else if(choice == 'Y' || choice == 'y') {
            return true;
            selected = true;
        } else {
            std::cout << "(X,Y) Values out of range!" << std::endl;
        }
    }
}

void reveal(int x, int y) {
    if(x >= 0 && x < DIMX && y >= 0 && y < DIMY) { // check that x and y are valid
        if(display[x][y] == UNCOVERED) {
            return;
        }
        display[x][y] = UNCOVERED;

        if(state[x][y] == MINE || state[x][y] != 0) {
            return;
        }
        reveal(x - 1, y - 1);
        reveal(x, y - 1);
        reveal(x + 1, y - 1);
        reveal(x - 1, y);
        reveal(x + 1, y);
        reveal(x - 1, y + 1);
        reveal(x, y + 1);
        reveal(x + 1, y + 1);
    } else {
        return;
    }
}

/*
[x-1, y-1][x, y-1][x+1, y-1]
[x-1, y  ][x, y  ][x+1, y  ]
[x-1, y+1][x, y+1][x+1, y+1]
*/
int countSorroundingMines(int x, int y) {
    if(state[x][y] == MINE) {
        return MINE;
    }
    int num = 0;
    if(x > 0 && y > 0) { // top left
        if(state[x - 1][y - 1] == MINE) {
            num++;
        }
    }
    if(y > 0) { // top
        if(state[x][y - 1] == MINE) {
            num++;
        }
    }
    if(x < DIMX - 1  && y > 0) { // top  right
        if(state[x + 1][y - 1] == MINE) {
            num++;
        }
    }
    if(x > 0) { // left
        if(state[x - 1][y] == MINE) {
            num++;
        }
    }
    if(x < DIMX - 1) { // right
        if(state[x + 1][y] == MINE) {
            num++;
        }
    }
    if(x > 0 && y < DIMY - 1) { // bottom left
        if(state[x - 1][y + 1] == MINE) {
            num++;
        }
    }
    if(y < DIMY - 1) { // bottom
        if(state[x][y + 1] == MINE) {
            num++;
        }
    }
    if(x < DIMX - 1 && y < DIMY - 1) { // bottom right
        if(state[x + 1][y + 1] == MINE) {
            num++;
        }
    }
    return num;
}

bool isWin() {
    bool win = true;
    for(int y = 0; y < DIMY; y++) {
        for(int x = 0; x < DIMX; x++) {
            win &= ((display[x][y] == UNCOVERED && state[x][y] != MINE) ||
                    ((display[x][y] == COVERED || display[x][y] == FLAG) && state[x][y] == MINE));
        }
    }
    return win;
}

bool isLose() {
    for(int y = 0; y < DIMY; y++) {
        for(int x = 0; x < DIMX; x++) {
            if (display[x][y] == UNCOVERED && state[x][y] == MINE) {
                return true;
            }
        }
    }
    return false;
}

void player() {
    std::cout << "Commands: R => reveal square, F => set flag, C => cheat, N => New Game" << std::endl;
    bool selected = false;
    while(!selected) {
        char choice;
        std::cin >> choice;
        if(choice == 'R' || choice == 'r') {
            uncover();
            selected = true;
        } else if(choice == 'F' || choice == 'f') {
            setFlag();
            selected = true;
        } else if(choice == 'C' || choice == 'c') {
            cheat();
        } else if(choice == 'N' || choice == 'n') {
            init();
            selected = true;
        } else {
            std::cout << "Invalid Choice!" << std::endl;
        }
    }

}

void uncover() {
    bool selected = false;
    while(!selected) {
        int x;
        int y;
        std::cout << "Input X (1 - " << (DIMX) << ")" << std::endl;
        std::cin >> x;
        std::cout << "Input Y (1 - " << (DIMY) << ")" << std::endl;
        std::cin >> y;
        x--;
        y--;
        if(x >= 0 && x < DIMX && y >= 0 && y < DIMY) {
            reveal(x, y); // call recursive reveal algorithm
            selected = true;
        } else {
            std::cout << "(X,Y) Values out of range!" << std::endl;
        }
    }
}

void setFlag() {
    bool selected = false;
    while(!selected) {
        int x;
        int y;
        std::cout << "Input X (1 - " << (DIMX) << ")" << std::endl;
        std::cin >> x;
        std::cout << "Input Y (1 - " << (DIMY) << ")" << std::endl;
        std::cin >> y;
        x--;
        y--;
        if(x >= 0 && x < DIMX && y >= 0 && y < DIMY) {
            display[x][y] = FLAG;
            selected = true;
        } else {
            std::cout << "(X,Y) Values out of range!" << std::endl;
        }
    }
}

void cheat() {
    std::cout << "Cheating is bad!" << std::endl;
    std::cout << "   ";
    for(int x = 0; x < DIMX; x++) {
        std::cout << (x + 1) << " ";
    }
    std::cout << std::endl << "";
    for(int x = 0; x < DIMX; x++) {
        std::cout << "";
    }
    std::cout << std::endl;
    for(int y = 0; y < DIMY; y++) {
        std::cout << (y + 1) << "  ";
        for(int x = 0; x < DIMX; x++) {
            if(state[x][y] == MINE) {
                std::cout << "@ ";
            } else {
                if(state[x][y] == 0) {
                    std:: cout << "  ";
                } else {
                    std:: cout << state[x][y] << " ";
                }
            }
        }
        std::cout << std::endl;
    }
}

void print() {
    std::cout << "   ";
    for(int x = 0; x < DIMX; x++) {
        std::cout << (x + 1) << " ";
    }
    std::cout << std::endl << "";
    for(int x = 0; x < DIMX; x++) {
        std::cout << "";
    }
    std::cout << std::endl;
    for(int y = 0; y < DIMY; y++) {
        std::cout << (y + 1) << "  ";
        for(int x = 0; x < DIMX; x++) {
            if(display[x][y] == COVERED || display[x][y] == 'F') { // is it tile still uncovered
                std::cout << display[x][y] << " ";
            } else { // display the state
                if(state[x][y] == MINE) {
                    std::cout << "@ ";
                } else {
                    if(state[x][y] == 0) {
                        std:: cout << "  ";
                    } else {
                        std:: cout << state[x][y] << " ";
                    }
                }
            }
        }

        std::cout << std::endl;
    }
}
