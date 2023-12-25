#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED
#include <string>
#include "card.hpp"
using namespace std;

class Player {
private:
    string name;
    int helth, money, location;
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
    
    // Get functions
    int getHelth();
    int getMoney();
    string getName();
};
#endif // PLAYER_HPP_INCLUDED