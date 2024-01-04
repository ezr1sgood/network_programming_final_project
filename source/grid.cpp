#include "../include/grid.hpp"

// initialize the real estate grid
Grid::Grid(string name, int type, int price, int tex) {
    this->name = name;
    this->type = type;
    this->price = price;
    this->level = 0;
    this->tex = tex;
}

// initialize the other grids
Grid::Grid(string name, int type) {
    this->name = name;
    this->type = type;
    this->price = 0;
    this->level = 0;
    this->tex = 0;
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