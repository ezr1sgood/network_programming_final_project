#include "../include/card.hpp"

Card::Card(string name, string statement) {
    name = name;
    statement = statement;
}

string Card::getName() {
    return name;
}

string Card::getStatement() {
    return statement;
}