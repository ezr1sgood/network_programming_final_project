#include "../include/player.hpp"
#define mapSize 18

// Constuctors
Player::Player() {
    name = "";
    helth = 0;
    money = 0;
    party = 0;
    party_level = 0;
    jail_duration = 0;
    active = true;
}

Player::Player(string name) {
    name = name;
    helth = 100;
    money = 1000;
    location = 0;
    party = 0;
    party_level = 0;
    jail_duration = 0;
    active = true;
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

void Player::move(const int steps) {
    this->location += steps;
    this->location %= mapSize;
}

void Player::setJailDuration(const int jail_duration) {
    this->jail_duration = jail_duration;
}

void Player::setParty(const int party) {
    this->party = party;
}

void Player::setPartyLevel(const int party_level) {
    this->party_level = party_level;
}


// Get functions
int Player::getHelth() {
    return helth;
}

int Player::getMoney() {
    return money;
}

int Player::getLocation() {
    return location;
}

int Player::getJailDuration() {
    return jail_duration;
}

int Player::getParty() {
    return party;
}

int Player::getPartyLevel() {
    return party_level;
}

bool Player::getActive() {
    return active;
}

string Player::getName() {
    return name;
}