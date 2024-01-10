#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED
#include <string>
#include <vector>
#include "card.hpp"

class Player {
private:
    std::string name;
    int helth, money, location;
    int party, party_level;
    int jail_duration;
    int user_id;
    int sockfd;
    bool active; // active or not                                                                            
    std::vector<Card> skills{};
    
public:
    // Constructors
    Player();
    Player(std::string name, int id, int sockfd);
    Player(const Player&);

    // Operations
    void addSkill(Card);
    void addHelth(const int);
    void addMoney(const int);
    void move(const int);
    void setJailDuration(const int);
    void setParty(const int);
    void setPartyLevel(const int);
    void setActive(const bool);
    void removeSkill(int);
    
    // Get functions
    int getHelth() const;
    int getMoney() const;
    int getLocation() const;
    int getJailDuration() const;
    int getParty() const;
    int getPartyLevel() const;
    int getId() const;
    int getSockfd() const;
    bool getActive() const;
    std::vector<Card> getSkills() const;
    std::string getName() const;

    // show status
    void showStatus();
};
#endif // PLAYER_HPP_INCLUDED