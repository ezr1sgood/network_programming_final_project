#ifndef GRID_HPP_INCLUDED
#define GRID_HPP_INCLUDED
#include <vector>
#include <string>

enum GRID_TYPE{
    START_GRID  = 0,
    BLUE_PARTY  = 1,
    GREEN_PARTY = 2,
    CHANCE_GRID = 3,
    DENSITY_GRID= 4,
    JAIL_GRID   = 5,
    REAL_ESTATE = 6,
};

class Grid {
private:
    std::string name;
    int type;
    int price, level, tex; // for real estate
    int id;
    int owner; // owner's id
public:
    Grid(){};
    Grid(std::string name, int id, int type, int price, int tex); // initialize the real estate grid
    Grid(std::string name, int id, int type); // initialize the other grids
    ~Grid(){};
    int getPayTex();
    int getTex();
    int getLevel();
    int getPrice();
    int getType();
    int getOwner();
    int getId();

    void setOwner(int);
    void setLevel(int);
    void reset();
    std::string getName();
};
#endif // GRID_HPP_INCLUDED