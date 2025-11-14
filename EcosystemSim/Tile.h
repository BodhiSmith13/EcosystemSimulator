#ifndef TILE_H
#define TILE_H

#include <string>
// Represents a Tile on the Board, which has a temperature and can contain creatures and their hunger values
class Tile {

    int temperature;
    int hunger;
    std::string occupant;

public:

    // Creates an empty Tile if no parameters are passed
    Tile();

    // Creates a custom Tile
    Tile(const int temperature, const int hunger, const std::string &occupant);

    int getTemperature() const;

    int getHunger() const;

    std::string getOccupant();

    void setTemperature(const int tileTemperature);

    void setHunger(const int tileHunger);

    void setOccupant(const std::string &tileOccupant);

}; // end of class Tile
#endif // TILE_H
