#include "../include/card.hpp"

Card::Card(std::string name, std::string statement) {
    this->name = name;
    this->statement = statement;
}

std::string Card::getName() {
    return name;
}

std::string Card::getStatement() {
    return statement;
}