#include <iostream>
#include <vector>
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
    Tile(const int temperature, const int hunger, const string &occupant) {

        this->temperature = temperature;
        this->hunger = hunger;
        this->occupant = occupant;

    } // end of constructor Tile

    int getTemperature() const {return temperature;}

    int getHunger() const {return hunger;}

    string getOccupant() {return occupant;}

    void setTemperature(const int tileTemperature) {temperature = tileTemperature;}

    void setHunger(const int tileHunger) {hunger = tileHunger;}

    void setOccupant(const string &tileOccupant) {occupant = tileOccupant;}

}; // end of class Tile

// Represents a move that may or may not happen depending on board state
class QueuedMove {

    int temp_hunger;
    string temp_occupant;
    // The y and x coordinates of the Tile to be moved to
    int y;
    int x;
    // The y and x coordinates of the Tile to be moved from
    int old_y;
    int old_x;


    public:

    QueuedMove() {
        temp_hunger = 0;
        temp_occupant = "empty";
        y = 0;
        x = 0;
        old_y = 0;
        old_x = 0;
    } // end of constructor QueuedMove

    QueuedMove(int const temp_hunger, string const &temp_occupant, int const y, int const x, int const old_y,
        int const old_x) {
        this->temp_hunger = temp_hunger;
        this->temp_occupant = temp_occupant;
        this->y = y;
        this->x = x;
        this->old_y = old_y;
        this->old_x = old_x;
    } // end of constructor QueuedMove

    void setHunger(const int hunger) {this->temp_hunger = hunger;}

    void setOccupant(string const &occupant) {this->temp_occupant = occupant;}

    void setX(const int newX) {x = newX;}

    void setY(const int newY) {y = newY;}

    void setOldX(const int oldX) {old_x = oldX;}

    void setOldY(const int oldY) {old_y = oldY;}

    int getHunger() const {return temp_hunger;}

    string getOccupant() {return temp_occupant;}

    int getX() const {return x;}

    int getY() const {return y;}

    int getOldX() const {return old_x;}

    int getOldY() const {return old_y;}

}; // end of class QueuedMove

// An arrangement of Tiles, as well as a Queue to allow those Tiles to move and interact
// NOTE: The origin (0, 0) of the board is at the top left
class Board {
    int height;
    int width;
    vector<vector<Tile>> board;
    vector<QueuedMove> queue;
    int defaultHunger;

public:

    Board(int height, int width, int defaultHunger) {

        this->height = height;
        this->width = width;
        this->board = vector<vector<Tile>>(height, vector<Tile>(width));
        this->defaultHunger = defaultHunger;

        // Checks if height is a multiple of 3. If not, creates an extra row of warm water
        if (height % 3 != 0) {
            for (int i = 0; i < (height / 3) + 1; i++) {
                for (int j = 0; j < width; j++) {
                    board[i][j].setTemperature(2);
                } // end of for loop
            } // end of for loop
            for (int i = (height / 3) + 1; i < ((height / 3) * 2) + 1 ; i++) {
                for (int j = 0; j < width; j++) {
                    board[i][j].setTemperature(1);
                } // end of for loop
            } // end of for loop
        } else {
            for (int i = 0; i < (height / 3); i++) {
                for (int j = 0; j < width; j++) {
                    board[i][j].setTemperature(2);
                } // end of for loop
            } // end of for loop
            for (int i = (height / 3); i < (height / 3) * 2 ; i++) {
                for (int j = 0; j < width; j++) {
                    board[i][j].setTemperature(1);
                } // end of for loop
            } // end of for loop
        } // end of if else statement
    } // end of constructor Board

    // Returns the address of a Tile at a specific x and y coordinate of the board
    Tile& getTile(const int y, const int x) {return board[y][x];}

    int getDefaultHunger() const {return defaultHunger;}

    // Prints the board, where each space is displayed as the one occupying it
    void displayOccupants() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                cout << board[y][x].getOccupant() << " ";
            } // end of for loop
            cout << endl;
        } // end of for loop
    } // end of method displayOccupants

    // Prints the board, where each space is displayed as the hunger value associated with it
    void displayHunger() const {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                cout << board[y][x].getHunger() << " ";
            } // end of for loop
            cout << endl;
        } // end of for loop
    } // end of method displayHunger

    void displayTemperature() const {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                cout << board[y][x].getTemperature() << " ";
            } // end of for loop
            cout << endl;
        } // end of for loop
    } // end of method displayTemperature

    QueuedMove calculateMove(const int y, const int x, const string &occupant, const int minTemp,
        const int maxTemp) const {

        QueuedMove move;
        move.setHunger(board[y][x].getHunger() - 1);
        move.setOccupant(occupant);

        int newX;
        int newY;
        // Checks if the Tile to be moved to is warm or cool. If not, runs the calculation again
        do {

            /* If a mobile Tile is in a corner and its safe temperature only occupies one row, it is
                possible that the program will loop endlessly as it tries and fails to find a safe tile
                for the Tile to move to. This if statement adds unique behavior for each corner of the
                board, preventing this endless loop*/
            if (x == 0 && y == 0) {

                const int chance = rand() % 100;
                if (chance < 50) {
                    newX = x + 1;
                    newY = y;
                } else {
                    newX = x;
                    newY = y + 1;
                } // end of if else statement

            } else if (x == width - 1 && y == 0) {

                const int chance = rand() % 100;
                if (chance < 50) {
                    newX = x - 1;
                    newY = y;
                } else {
                    newX = x;
                    newY = y + 1;
                } // end of if else statement

            } else if (x == 0 && y == height - 1) {

                const int chance = rand() % 100;
                if (chance < 50) {
                    newX = x + 1;
                    newY = y;
                } else {
                    newX = x;
                    newY = y - 1;
                } // end of if else statement

            } else if (x == width - 1 && y == height - 1) {

                const int chance = rand() % 100;
                if (chance < 50) {
                    newX = x - 1;
                    newY = y;
                } else {
                    newX = x;
                    newY = y - 1;
                } // end of if else statement

            } else {
                if (x == 0) {
                    int chance = rand() % 100;
                    if (chance < 50) {
                        newX = x + 1;
                    } else {
                        newX = x;
                    } // end of if else statement
                } else if (x == width - 1) {
                    int chance = rand() % 100;
                    if (chance < 50) {
                        newX = x - 1;
                    } else {
                        newX = x;
                    } // end of if else statement
                } else {
                    newX = x + randomMove();
                } // end of if else statement
                if (y == 0) {
                    int chance = rand() % 100;
                    if (chance < 50) {
                        newX = x;
                        newY = y + 1;
                    } else {
                        chance = rand() % 100;
                        if (chance < 50) {
                            newX = x + 1;
                            newY = y;
                        } else {
                            newX = x - 1;
                            newY = y;
                        } // end of if else statement
                    } // end of if else statement
                } else if (y == height - 1) {
                    int chance = rand() % 100;
                    if (chance < 50) {
                        newX = x;
                        newY = y - 1;
                    } else {
                        chance = rand() % 100;
                        if (chance < 50) {
                            newX = x + 1;
                            newY = y;
                        } else {
                            newX = x - 1;
                            newY = y;
                        } // end of if else statement
                    } // end of if else statement
                } else {
                    newY = y + randomMove();
                } // end of if else statement
            } // end of if else statement
        } while (board[newY][newX].getTemperature() < minTemp || board[newY][newX].getTemperature() > maxTemp);
        move.setX(newX);
        move.setY(newY);
        move.setOldX(x);
        move.setOldY(y);
        return move;
    } // end of method calculateMove

    void breed(int y, int x, int minTemp, int maxTemp, const string &baby) {
            bool bred = false;
            for (int top = y - 1; top <= y + 1 && !bred; top++) {
                for (int left = x - 1; left <= x + 1 && !bred; left++) {
                    // If the Tile is out of bounds, it will not be checked
                    if (top < 0 || left < 0 || top >= height || left >= width || (top == y && left == x)) {
                        continue;
                    } // end of if statement

                    // Checks that the Tile to be cloned to is empty and within the given temperature range.
                    if (board[top][left].getOccupant() == "empty" && board[top][left].getTemperature() > minTemp
                        && board[top][left].getTemperature() <= maxTemp){
                        if ((rand() % 100) < 70) {
                            board[top][left].setOccupant(baby);
                            board[top][left].setHunger(defaultHunger);
                            board[y][x].setHunger(board[y][x].getHunger() - 5);
                            bred = true;
                        } // end of if else statement
                    } // end of if else statement
                } // end of for loop
            } // end of for loop
        } // end of function Breed

    void eat(int y, int x, const string &prey, int chance) {
        bool ate = false;
        for (int top = y - 1; top <= y + 1 && !ate; top++) {
            for (int left = x - 1; left <= x + 1 && !ate; left++) {
                // If the Tile is out of bounds, it will not be checked
                if (top < 0 || left < 0 || top >= height || left >= width || (top == y && left == x)) {
                    continue;
                } // end of if statement

                if (board[top][left].getOccupant() == prey) {
                    if ((rand() % 100) < chance) {
                        if (prey != "kelp") {
                            board[top][left].setOccupant("bones");
                            board[y][x].setHunger(-1);
                        } else {
                            board[y][x].setHunger(0);
                            board[y][x].setHunger(board[y][x].getHunger() + 10);

                        }  // end of if else statement
                        ate = true;
                    } // end of if statement
                } // end of if statement
            } // end of for loop
        } // end of for loop
    } // end of method eat

    void eat(int y, int x, const string &prey1, int chance1, const string &prey2, int chance2) {
        bool ate = false;
        for (int top = y - 1; top <= y + 1 && !ate; top++) {
            for (int left = x - 1; left <= x + 1 && !ate; left++) {
                // If the Tile is out of bounds, it will not be checked
                if (top < 0 || left < 0 || top >= height || left >= width || (top == y && left == x)) {
                    continue;
                } // end of if statement

                if (board[top][left].getOccupant() == prey1) {
                    if ((rand() % 100) < chance1) {
                        if (prey1 != "kelp") {
                            board[top][left].setOccupant("bones");
                            board[y][x].setHunger(-1);
                        } else {
                            board[y][x].setHunger(0);
                            board[y][x].setHunger(board[y][x].getHunger() + 10);

                        }  // end of if else statement
                        ate = true;
                    } // end of if statement
                } else if (board[top][left].getOccupant() == prey2) {
                    if ((rand() % 100) < chance2) {
                        if (prey2 != "kelp") {
                            board[top][left].setOccupant("bones");
                            board[y][x].setHunger(-1);
                        } else {
                            board[y][x].setHunger(0);
                            board[y][x].setHunger(board[y][x].getHunger() + 10);
                        }  // end of if else statement
                        ate = true;
                    } // end of if statement
                }// end of if else statement
            } // end of for loop
        } // end of for loop
    } // end of method eat

    void dangerEat(int y, int x, const string &dangerousPrey, int chance, int deathChance) {
        bool ate = false;
        for (int top = y - 1; top <= y + 1 && !ate; top++) {
            for (int left = x - 1; left <= x + 1 && !ate; left++) {
                // If the Tile is out of bounds, it will not be checked
                if (top < 0 || left < 0 || top >= height || left >= width || (top == y && left == x)) {
                    continue;
                } // end of if statement
                const int outcome = rand() % 100;
                if (board[top][left].getOccupant() == dangerousPrey) {
                    if (outcome < chance) {
                        if (outcome < deathChance) {
                            board[y][x].setOccupant("bones");
                            board[y][x].setHunger(-1);
                        } else {
                            if (dangerousPrey != "kelp") {
                                board[top][left].setOccupant("bones");
                                board[y][x].setHunger(-1);
                            } else {
                                board[y][x].setHunger(0);
                                board[y][x].setHunger(board[y][x].getHunger() + 10);
                                } // end of if else statement
                            }  // end of if else statement
                        ate = true;
                    } // end of if statement
                } // end of if statement
            } // end of for loop
        } // end of for loop
    } // end of method eat



    // Sweeps the board from top left to bottom right, and resolves Tile's actions when it encounters a Tile capable
    // of actions
    void tick() {

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Finds starving mobile Tiles that aren't seaweed or bones and turns those Tiles into bones. These
                // Tiles then have their hunger values set to -1, so that bones hang in the same spot before sinking
                if ((board[y][x].getOccupant() != "empty" && board[y][x].getOccupant() != "seaweed"
                    && board[y][x].getOccupant() != "bones") && board[y][x].getHunger() == 0) {

                    board[y][x].setOccupant("bones");
                    board[y][x].setHunger(-1);

                    } else if (board[y][x].getOccupant() == "bones" && board[y][x].getHunger() == 0) {
                        if (y + 1 < height && board[y + 1][x].getOccupant() == "empty") {
                            queue.push_back(
                                QueuedMove(0, "bones", y + 1, x, y, x));
                        }
                    } else if (board[y][x].getOccupant() == "shark") {

                        if (board[y][x].getHunger() > 10) {
                            breed(y, x, 1, 2, "shark");
                        } // end of if statement

                        // A shark checks all Tiles around it for prey. When it encounters a Tile occupied by its prey
                        // (goldfish or pufferfish), it attempts to convert that Tile into empty with a 70% success rate
                        eat(y, x, "goldfish", "bones", "pufferfish");

                        // Randomly moves the shark one space in any direction, as long as that movement will not take
                        // it out of bounds
                        // Also checks if the shark died from eating a pufferfish.
                        if (board[y][x].getOccupant() != "empty") {
                            queue.push_back(calculateMove(y, x, "shark", 2, 2));
                        } // end of if statement


                    } else if (board[y][x].getOccupant() == "goldfish") {

                        // Breeding
                        if (board[y][x].getHunger() > 10) {
                            breed(y, x, 0, 2, "goldfish");
                        } // end of if statement

                        // A goldfish checks all Tiles around it for prey. When it encounters a Tile occupied by its
                        // prey (seaweed), it attempts to convert that Tile into empty with a 70% success rate
                        eat(y, x, "seaweed", "empty");

                        // Randomly moves the goldfish one space in any direction, as long as that movement will not
                        // take it out of bounds
                        queue.push_back(calculateMove(y, x, "goldfish", 1, 2));

                    } else if (board[y][x].getOccupant() == "seaweed") {

                        // Seaweed has a 50% chance each tick to convert a random empty Tile adjacent to it into seaweed
                        if (rand() % 100 < 50) {
                            breed(y, x, 2, 2, "seaweed");
                        }

                    } else if (board[y][x].getOccupant() == "pufferfish") {

                        // Breeding
                        if (board[y][x].getHunger() > 10) {
                            breed(y, x, 1, 2, "pufferfish");
                        } // end of if statement

                        // A pufferfish checks all Tiles around it for prey. When it encounters a Tile occupied by its
                        // prey (seaweed or goldfish), it attempts to convert that Tile into empty with a 70% success
                        // rate or a 35% success rate, respectively

                        queue.push_back(calculateMove(y, x, "pufferfish", 1, 2));
                    } else if (board[y][x].getOccupant() == "crab") {

                        // Breeding
                        breed(y, x, 0, 0, "crab");

                        // A crab checks all Tiles around it for prey. When it encounters a Tile occupied by its
                        // prey (bones), it attempts to convert that Tile into empty with a 70% success rate
                        bool ate = false;
                        for (int top = y - 1; top <= y + 1 && !ate; top++) {
                            for (int left = x - 1; left <= x + 1 && !ate; left++) {
                                // If the Tile is out of bounds, it will not be checked
                                if (top < 0 || left < 0 || top >= height || left >= width || (top == y && left == x)) {
                                    continue;
                                } // end of if statement

                                if (board[top][left].getOccupant() == "bones") {
                                    if ((rand() % 100) < 70) {
                                        board[top][left].setOccupant("empty");
                                        board[top][left].setHunger(0);
                                        board[y][x].setHunger(board[y][x].getHunger() + 10);
                                        ate = true;
                                    } // end of if else statement
                                } // end of if statement
                            } // end of for loop
                        } // end of for loop
                        queue.push_back(calculateMove(y, x, "crab", 0, 0));
                    } // end of if else statement
            } // end of for loop
        } // end of for loop

        // Iterates through each QueuedMove in the queue, allowing that QueuedMove to resolve if the targeted Tile is
        // empty and the original Tile has not been turned empty (eaten)
        for (int i = 0; i < queue.size(); i++) {
            QueuedMove current_move = queue[i];
            Tile* newTile = &board[current_move.getY()][current_move.getX()];
            Tile* oldTile = &board[current_move.getOldY()][current_move.getOldX()];
            if (newTile->getOccupant() == "empty" && oldTile->getOccupant() != "empty") {

                newTile->setOccupant(current_move.getOccupant());
                newTile->setHunger(current_move.getHunger());
                oldTile->setOccupant("empty");
                oldTile->setHunger(0);

            } // end of if statement
        } // end of for loop
        // Changes all bones' hunger values from -1 to 0
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (board[y][x].getOccupant() == "bones" && board[y][x].getHunger() == -1) {
                    board[y][x].setHunger(0);
                }
            }
        }
        // Clears the queue to prepare for the next tick
        queue.clear();
    } // end of method Tick
}; // end of Class Board

int main() {
    // Seeds the random number generator
    srand(time(nullptr));

    Board board(3, 3, 5);
    board.getTile(0, 0).setOccupant("shark");
    board.getTile(0, 0).setHunger(board.getDefaultHunger());
    board.getTile(0, 1).setOccupant("goldfish");
    board.getTile(0, 1).setHunger(board.getDefaultHunger());
    board.getTile(2, 2).setOccupant("crab");
    board.getTile(2, 2).setHunger(board.getDefaultHunger());
    board.displayTemperature();
    cout << endl;
    board.displayOccupants();
    cout << endl;
    board.displayHunger();
    cout << endl;
    for (int i = 0; i < 10; i++) {
        board.tick();
        board.displayOccupants();
        cout << endl;
        board.displayHunger();
        cout << endl;
    }
    return 0;
} // end of main