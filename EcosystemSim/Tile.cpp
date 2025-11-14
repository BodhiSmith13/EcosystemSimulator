#include "Tile.h"
// Represents a Tile on the Board, which has a temperature and can contain creatures and their hunger values



    // Creates an empty Tile if no parameters are passed
    Tile::Tile() {

        temperature = 0;
        hunger = 0;
        occupant = "empty";

    } // end of constructor Tile

    // Creates a custom Tile
    Tile::Tile(const int temperature, const int hunger, const std::string &occupant) {

        this->temperature = temperature;
        this->hunger = hunger;
        this->occupant = occupant;

    } // end of constructor Tile

    int Tile::getTemperature() const {return temperature;}

    int Tile::getHunger() const {return hunger;}

    std::string Tile::getOccupant() {return occupant;}

    void Tile::setTemperature(const int tileTemperature) {temperature = tileTemperature;}

    void Tile::setHunger(const int tileHunger) {hunger = tileHunger;}

    void Tile::setOccupant(const std::string &tileOccupant) {occupant = tileOccupant;}
