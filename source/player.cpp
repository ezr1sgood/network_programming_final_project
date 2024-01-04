#include "../include/player.hpp"
#include <string>
#include <iostream>
#define mapSize 18

// Constuctors
Player::Player() {}

Player::Player(string name, int id) {
    this->name = name;
    this->helth = 100;
    this->money = 1000;
    this->location = 0;
    this->party = 0;
    this->party_level = 0;
    this->jail_duration = 0;
    this->active = true;
    this->user_id = id;
}

// Copy constructor
Player::Player(const Player& player) {
    this->name = player.name;
    this->helth = player.helth;
    this->money = player.money;
    this->skills = player.skills;
    this->user_id = player.user_id;
}

// Operations
void Player::addSkill(Card skill) {
    this->skills.push_back(skill);
}

void Player::addHelth(const int val) {
    this->helth += val;
}

void Player::addMoney(const int val) {
    this->money += val;
}

void Player::move(const int steps) {
    this->location += steps;
    this->location %= mapSize;
}

void Player::setJailDuration(const int upd_duration) {
    this->jail_duration = upd_duration;
}

void Player::setParty(const int _party) {
    this->party = _party;
}

void Player::setPartyLevel(const int _party_level) {
    this->party_level = _party_level;
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

int Player::getId() {
    return user_id;
}

int Player::getSockfd() {
    return sockfd;
}

bool Player::getActive() {
    return active;
}

string Player::getName() {
    return name;
}


void Player::showStatus() {
    cout << "Player\t\t" << this->getName() << endl;
    cout << "Helth\t\t" << this->getHelth() << endl;
    cout << "Money\t\t" << this->getMoney() << endl;
    cout << "Location\t" << this->getLocation() << endl;
    cout << "Party\t\t" << this->getParty() << endl;
    cout << "Party level\t" << this->getPartyLevel() << endl;
    cout << "Jail duration\t" << this->getJailDuration() << endl;
    cout << "--------------------------------\n";
}