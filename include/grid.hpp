#ifndef GRID_HPP_INCLUDED
#define GRID_HPP_INCLUDED
#include <vector>
#include <string>
using namespace std;

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
    string name;
    int type;
    int price, level, tex; // for real estate
public:
    Grid(){};
    Grid(string name, int type, int price, int tex); // initialize the real estate grid
    Grid(string name, int type); // initialize the other grids
    ~Grid(){};
    int getPayTex();
    int getTex();
    int getLevel();
    int getPrice();
    int getType();
};
#endif // GRID_HPP_INCLUDED