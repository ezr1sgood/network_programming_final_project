#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED
#include <string>
#include "card.hpp"
using namespace std;

class Player {
private:
    string name;
    int helth, money, location;
    int party, party_level;
    int jail_duration;
    bool active; // active or not
    vector<Card> skills{};
    
public:
    // Constructors
    Player();
    Player(string name);
    Player(Player& player);

    // Operations
    void addSkill(Card skill);
    void addHelth(const int helth);
    void addMoney(const int money);
    void move(const int steps);
    void setJailDuration(const int jail_duration);
    void setParty(const int);
    void setPartyLevel(const int);
    
    // Get functions
    int getHelth();
    int getMoney();
    int getLocation();
    int getJailDuration();
    int getParty();
    int getPartyLevel();
    bool getActive();
    string getName();
};
#endif // PLAYER_HPP_INCLUDED