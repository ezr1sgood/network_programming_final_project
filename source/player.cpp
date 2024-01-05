#include "../include/player.hpp"
#include <string>
#include <iostream>
#define mapSize 18

// Constuctors
Player::Player() {}

Player::Player(std::string name, int id, int sockfd) {
    this->name = name;
    this->helth = 100;
    this->money = 1000;
    this->location = 0;
    this->party = 0;
    this->party_level = 0;
    this->jail_duration = 0;
    this->active = true;
    this->user_id = id;
    this->sockfd = sockfd;
    std::cerr << this->sockfd << std::endl;
}

// Copy constructor
Player::Player(const Player& player) {
    this->name = player.name;
    this->helth = player.helth;
    this->money = player.money;
    this->location = player.location;
    this->party = player.party;
    this->party_level = player.party_level;
    this->jail_duration = player.jail_duration;
    this->user_id = player.user_id;
    this->sockfd = player.sockfd;
    this->active = player.active;
    this->skills = player.skills;
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

std::string Player::getName() {
    return name;
}


void Player::showStatus() {
    std::cout << "Player\t\t" << this->getName() << std::endl;
    std::cout << "Helth\t\t" << this->getHelth() << std::endl;
    std::cout << "Money\t\t" << this->getMoney() << std::endl;
    std::cout << "Location\t" << this->getLocation() << std::endl;
    std::cout << "Party\t\t" << this->getParty() << std::endl;
    std::cout << "Party level\t" << this->getPartyLevel() << std::endl;
    std::cout << "Jail duration\t" << this->getJailDuration() << std::endl;
    std::cout << "--------------------------------\n";
}