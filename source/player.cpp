#include "../include/Player.hpp"


// Constuctors
Player::Player() {
    name = "";
    helth = 0;
    money = 0;
}

Player::Player(string name) {
    name = name;
    helth = 100;
    money = 1000;
    location = 0;
}

// Copy constructor
Player::Player(Player& player) {
    name = player.name;
    helth = player.helth;
    money = player.money;
    skills = player.skills;
}

// Operations
void Player::addSkill(Card skill) {
    skills.push_back(skill);
}

void Player::addHelth(const int helth) {
    this->helth += helth;
}

void Player::addMoney(const int money) {
    this->money += money;
}



// Get functions
int Player::getHelth() {
    return helth;
}

int Player::getMoney() {
    return money;
}

string Player::getName() {
    return name;
}   