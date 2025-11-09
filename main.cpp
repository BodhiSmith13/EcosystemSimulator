#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Returns either -1 or 1. Used to determine what tile is moved towards
int randomMove () {return (rand() % 2) * 2 - 1;}

// Represents a Tile on the Board, which has a temperature and can contain creatures and their hunger values
class Tile {

int temperature;
int hunger;
string occupant;

    public:

    // Creates an empty Tile if no parameters are passed
    Tile() {

        temperature = 0;
        hunger = 0;
        occupant = "empty";

    } // end of constructor Tile

    // Creates a custom Tile
    Tile(int temperature, int hunger, string occupant) {

        this->temperature = temperature;
        this->hunger = hunger;
        this->occupant = occupant;

    } // end of constructor Tile

    int getTemperature() {return temperature;}

    int getHunger() {return hunger;}

    string getOccupant() {return occupant;}

    void setTemperature(int temperature) {this->temperature = temperature;}

    void setHunger(int hunger) {this->hunger = hunger;}

    void setOccupant(string occupant) {this->occupant = occupant;}

}; // end of class Tile

// Represents a move that may or may not happen depending on board state
class QueuedMove {

    int temp_hunger;
    string temp_occupant;
    // The x and y coordinates of the Tile to be moved to
    int x;
    int y;
    // The x and y coordinates of the Tile to be moved from
    int old_x;
    int old_y;

    public:

    void setHunger(int hunger) {this->temp_hunger = hunger;}

    void setOccupant(string occupant) {this->temp_occupant = occupant;}

    void setX(int x) {this->x = x;}

    void setY(int y) {this->y = y;}

    void setOldX(int oldX) {this->old_x = oldX;}

    void setOldY(int oldY) {this->old_y = oldY;}

    int getHunger() {return temp_hunger;}

    string getOccupant() {return temp_occupant;}

    int getX() {return x;}

    int getY() {return y;}

    int getOldX() {return old_x;}

    int getOldY() {return old_y;}

}; // end of class QueuedMove

// Used to assemble QueuedMoves in a way that they can be parsed one after another, similar to a stack
class Queue {

    vector<QueuedMove> stack;

    public:

    // Adds QueuedMoves to the stack
    void push(QueuedMove move) {stack.push_back(move);}

    // Clears the stack
    void clear() {stack.clear();}

    // Returns the size of the stack
    int size() {return stack.size();}

    // Returns the QueuedMove at a specific index of the stack
    QueuedMove get(int index) {return stack[index];}

}; // end of class Queue

// An arrangement of Tiles, as well as a Queue to allow those Tiles to move and interact
// NOTE: The origin (0, 0) of the board is at the top left
class Board {

    int height;
    int width;
    vector<vector<Tile>> board;
    Queue queue;

public:

    // Initializes the board to be 9 Tiles high and wide, and creates a 2D vector to keep track of tiles
    Board() : height(9), width(9), board(9, vector<Tile>(9)) {}

    // Returns the address of a Tile at a specific x and y coordinate of the board
    Tile& getTile(int x, int y) {return board[y][x];}

    // Prints the board, where each space is displayed as the one occupying it
    void displayOccupants() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cout << board[i][j].getOccupant() << " ";
            } // end of for loop
            cout << endl;
        } // end of for loop
    } // end of method displayOccupants

    // Prints the board, where each space is displayed as the hunger value associated with it
    void displayHunger() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cout << board[i][j].getHunger() << " ";
            } // end of for loop
            cout << endl;
        } // end of for loop
    } // end of method displayHunger

    // Sweeps the board from top left to bottom right, and resolves Tile's actions when it encounters a Tile capable
    // of actions
    void tick() {

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (board[i][j].getOccupant() == "shark") {

                    // A shark checks all Tiles around it for prey. When it encounters a Tile occupied by its prey
                    // (goldfish or pufferfish), it attempts to convert that Tile into empty with a 70% success rate
                    bool ate = false;
                    for (int top = i - 1; top <= i + 1 && !ate; top++) {
                        for (int left = j - 1; left <= j + 1 && !ate; left++) {
                            // If the Tile is out of bounds, it will not be checked
                            if (top < 0 || left < 0 || top >= height || left >= width || (top == i && left == j)) {
                                continue;
                            } // end of if statement

                            if (board[top][left].getOccupant() == "goldfish") {
                                if ((rand() % 100) < 70) {
                                     cout << "Ate a goldfish" << endl;
                                    board[top][left].setOccupant("empty");
                                    board[top][left].setHunger(0);
                                    ate = true;
                                } else {
                                     cout << "Ignored a goldfish" << endl;
                                } // end of if else statement
                            } else if (board[top][left].getOccupant() == "pufferfish") {
                                int outcome = rand() % 100;
                                if (outcome < 35) {
                                     cout << "Tried to eat a pufferfish and died" << endl;
                                    board[i][j].setOccupant("empty");
                                    board[i][j].setHunger(0);
                                    ate = true;
                                } else if (outcome < 70) {
                                     cout << "Succesfully ate a pufferfish" << endl;
                                    board[top][left].setOccupant("empty");
                                    board[top][left].setHunger(0);
                                    ate = true;
                                } else {
                                     cout << "Ignored a pufferfish" << endl;
                                } // end of if else statement
                            } // end of if else statement
                        } // end of for loop
                    } // end of for loop

                    // Randomly moves the shark one space in any direction, as long as that movement will not take it
                    // out of bounds
                    // Also checks if the shark died from eating a pufferfish.
                    if (board[i][j].getOccupant() != "empty") {

                        QueuedMove move;
                        move.setHunger(board[i][j].getHunger());
                        move.setOccupant("shark");

                        int newX;
                        int newY;
                        if (j == 0) {
                            newX = j + 1;
                        } else if (j == width - 1) {
                            newX = j - 1;
                        } else {
                            newX = j + randomMove();
                        } // end of if else statement
                        if (i == 0) {
                            newY = i + 1;
                        } else if (i == height - 1) {
                            newY = i - 1;
                        } else {
                            newY = i + randomMove();
                        } // end of if else statement
                        move.setX(newX);
                        move.setY(newY);
                        move.setOldX(j);
                        move.setOldY(i);
                        // cout << move.getX() << " " << move.getY() << endl;
                        queue.push(move);

                    }


                } else if (board[i][j].getOccupant() == "goldfish") {

                    // A goldfish checks all Tiles around it for prey. When it encounters a Tile occupied by its prey
                    // (seaweed), it attempts to convert that Tile into empty with a 70% success rate
                    bool ate = false;
                    for (int top = i - 1; top <= i + 1 && !ate; top++) {
                        for (int left = j - 1; left <= j + 1 && !ate; left++) {
                            // If the Tile is out of bounds, it will not be checked
                            if (top < 0 || left < 0 || top >= height || left >= width || (top == i && left == j)) {
                                continue;
                            } // end of if statement

                            if (board[top][left].getOccupant() == "seaweed") {
                                if ((rand() % 100) < 70) {
                                     cout << "Ate seaweed" << endl;
                                    board[top][left].setOccupant("empty");
                                    board[top][left].setHunger(0);
                                    ate = true;
                                }  // end of if statement
                            } // end of if statement
                        } // end of for loop
                    } // end of for loop

                    // Randomly moves the goldfish one space in any direction, as long as that movement will not take it
                    // out of bounds
                    QueuedMove move;
                    move.setHunger(board[i][j].getHunger());
                    move.setOccupant("goldfish");
                    int newX;
                    int newY;
                    if (j == 0) {
                        newX = j + 1;
                    } else if (j == width - 1) {
                        newX = j - 1;
                    } else {
                        newX = j + randomMove();
                    } // end of if else statement
                    if (i == 0) {
                        newY = i + 1;
                    } else if (i == height - 1) {
                        newY = i - 1;
                    } else {
                        newY = i + randomMove();
                    } // end of if else statement
                    move.setX(newX);
                    move.setY(newY);
                    move.setOldX(j);
                    move.setOldY(i);
                    // cout << move.getX() << " " << move.getY() << endl;
                    queue.push(move);

                } else if (board[i][j].getOccupant() == "seaweed") {

                    // Seaweed has a 30% chance each tick to convert a random empty Tile adjacent to it into seaweed
                    bool birthed = false;
                    for (int top = i - 1; top <= i + 1 && !birthed; top++) {
                        for (int left = j - 1; left <= j + 1 && !birthed; left++) {
                            // If the Tile is out of bounds, it will not be checked
                            if (top < 0 || left < 0 || top >= height || left >= width || (top == i && left == j)) {
                                continue;
                            } // end of if statement

                            if (board[top][left].getOccupant() == "empty") {
                                if ((rand() % 100) < 30) {
                                    cout << "Seaweed cloned" << endl;
                                    board[top][left].setOccupant("seaweed");
                                    birthed = true;
                                }  // end of if statement
                            } // end of if statement
                        } // end of for loop
                    } // end of for loop

                    // Randomly moves the seaweed one space in any direction, as long as that movement will not take it
                    // out of bounds
                    // Seaweed has a 50% chance to not move
                    if ((rand() % 100) < 50) {
                        QueuedMove move;
                        move.setHunger(0);
                        move.setOccupant("seaweed");
                        int newX;
                        int newY;
                        if (j == 0) {
                            newX = j + 1;
                        } else if (j == width - 1) {
                            newX = j - 1;
                        } else {
                            newX = j + randomMove();
                        } // end of if else statement
                        if (i == 0) {
                            newY = i + 1;
                        } else if (i == height - 1) {
                            newY = i - 1;
                        } else {
                            newY = i + randomMove();
                        } // end of if else statement
                        move.setX(newX);
                        move.setY(newY);
                        move.setOldX(j);
                        move.setOldY(i);
                        // cout << move.getX() << " " << move.getY() << endl;
                        queue.push(move);
                    }

                } else if (board[i][j].getOccupant() == "pufferfish") {

                    // A pufferfish checks all Tiles around it for prey. When it encounters a Tile occupied by its prey
                    // (seaweed or goldfish), it attempts to convert that Tile into empty with a 70% success rate
                    bool ate = false;
                    for (int top = i - 1; top <= i + 1 && !ate; top++) {
                        for (int left = j - 1; left <= j + 1 && !ate; left++) {
                            // If the Tile is out of bounds, it will not be checked
                            if (top < 0 || left < 0 || top >= height || left >= width || (top == i && left == j)) {
                                continue;
                            } // end of if statement

                            if (board[top][left].getOccupant() == "seaweed") {
                                if ((rand() % 100) < 70) {
                                    cout << "Ate seaweed" << endl;
                                    board[top][left].setOccupant("empty");
                                    board[top][left].setHunger(0);
                                    ate = true;
                                } else {
                                    cout << "Ignored seaweed" << endl;
                                } // end of if else statement
                            } // end of if statement
                        } // end of for loop
                    } // end of for loop
                } // end of if else statement
            } // end of for loop
        } // end of for loop

        // Iterates through each QueuedMove in the queue, allowing that QueuedMove to resolve if the targeted Tile is
        // empty and the original Tile has not been turned empty (eaten)
        for (int i = 0; i < queue.size(); i++) {
            QueuedMove current_move = queue.get(i);
            Tile* newTile = &board[current_move.getY()][current_move.getX()];
            Tile* oldTile = &board[current_move.getOldY()][current_move.getOldX()];
            if (newTile->getOccupant() == "empty" && oldTile->getOccupant() != "empty") {

                newTile->setOccupant(current_move.getOccupant());
                if (current_move.getHunger() > 0) {
                    newTile->setHunger(current_move.getHunger() - 1);
                } else {
                    newTile->setHunger(current_move.getHunger());
                }
                oldTile->setOccupant("empty");
                oldTile->setHunger(0);

            } // end of if statement
        } // end of for loop

        // Clears the queue to prepare for the next tick
        queue.clear();
    } // end of method Tick
}; // end of Class Board

int main() {
    // Seeds the random number generator
    srand(time(0));

    Board board;
    board.getTile(0, 0).setOccupant("goldfish");
    board.getTile(0, 0).setHunger(5);
    board.getTile(1, 1).setOccupant("pufferfish");
    board.getTile(1, 1).setHunger(5);
    board.getTile(2, 2).setOccupant("shark");
    board.getTile(2, 2).setHunger(5);
    board.getTile(1, 2).setOccupant("seaweed");
    board.getTile(1, 2).setHunger(0);
    board.getTile(8, 8).setOccupant("seaweed");
    board.getTile(8, 8).setHunger(0);
    board.displayOccupants();
    cout << endl;
   for (int i = 0; i < 10; i++) {
       board.tick();
       board.displayOccupants();
       cout << endl;
   }



    return 0;
} // end of main