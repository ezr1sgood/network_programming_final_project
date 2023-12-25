#include "../include/grid.hpp"

// initialize the real estate grid
Grid::Grid(string _name, int _type, int _price, int _tex) {
    name = _name;
    type = _type;
    price = _price;
    level = 0;
    tex = 0;
}

// initialize the other grids
Grid::Grid(string _name, int _type) {
    name = _name;
    type = _type;
    price = 0;
    level = 0;
    tex = 0;
}

int Grid::getPayTex() {
    return tex * level;
}

int Grid::getTex() {
    return tex;
}

int Grid::getLevel() {
    return level;
}

int Grid::getPrice() {
    return price;
}

int Grid::getType() {
    return type;
}
