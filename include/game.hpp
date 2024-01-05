#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED
#include "grid.hpp"
#include "player.hpp"
#include "card.hpp"

class Game {
private:
    std::vector< Player > players;
    std::vector< Card > chance;
    std::vector< Card > density;
    bool status; // 1: game is running, 0: game is over
public:
    std::vector< Grid > map;
    Game();
    ~Game(){};
    void SendMessageToClient(std::string message, int sockfd);
    void SendMessageToAllClients(std::string message);
    std::string ReadMessageFromClient(int sockfd);

    void run();
    void victory(Player& player);
    void addPlayer(std::string name, int id);
    void playerGetChance(Player& player);
    void playerGetDensity(Player& player);
    void playerTurn(Player& player);
    void endTurn(Player& player);

    void handlePartyEvent(Player&, const int partyType);
    void handleRealEstateEvent(Player&, Grid&);
    
    void showStatus() {
        for (auto &player : players) {
            player.showStatus();
        }
    }
};
#endif // GAME_HPP_INCLUDED