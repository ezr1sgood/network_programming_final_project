#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED
#include <string>
#include <vector>
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
    Player(string);
    Player(const Player&);

    // Operations
    void addSkill(Card);
    void addHelth(const int);
    void addMoney(const int);
    void move(const int);
    void setJailDuration(const int);
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

    // show status
    void showStatus();
};
#endif // PLAYER_HPP_INCLUDED