#include "../include/card.hpp"

Card::Card(string name, string statement) {
    this->name = name;
    this->statement = statement;
}

string Card::getName() {
    return name;
}

string Card::getStatement() {
    return statement;
}