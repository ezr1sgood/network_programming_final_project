#include "../include/grid.hpp"

// initialize the real estate grid
Grid::Grid(std::string name, int id, int type, int price, int tex) {
    this->id = id;
    this->name = name;
    this->type = type;
    this->price = price;
    this->level = 0;
    this->tex = tex;
    this->owner = 0;
}

// initialize the other grids
Grid::Grid(std::string name, int id, int type) {
    this->id = id;
    this->name = name;
    this->type = type;
    this->price = 0;
    this->level = 0;
    this->tex = 0;
    this->owner = 0;
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

std::string Grid::getName() {
    return name;
}

int Grid::getOwner() {
    return owner;
}

int Grid::getId() {
    return id;
}

void Grid::setOwner(int _owner) {
    this->owner = _owner;
}

void Grid::setLevel(int _level) {
    this->level = _level;
}

void Grid::reset() {
    this->setLevel(0);
    this->setOwner(0);
}