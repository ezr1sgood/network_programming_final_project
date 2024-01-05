#include "../include/card.hpp"

Card::Card(std::string id, std::string name, std::string statement) {
    this->cardId = id;
    this->name = name;
    this->statement = statement;
}

std::string Card::getName() {
    return name;
}

std::string Card::getStatement() {
    return statement;
}

std::string Card::getId() {
    return cardId;
}