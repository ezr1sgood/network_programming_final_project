#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED
#include <vector>
#include <random>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <algorithm>
#include <iostream>     // std::cout
#include "grid.hpp"
#include "player.hpp"
#include "card.hpp"

using namespace std;



class Game {
private:
    vector<int> players;
    vector< Grid > map;
    vector< Card > chance;
    vector< Card > density;
public:
    Game();
    void run();
    void addPlayer(string name);
    void playerGetChance(Player& player);
    void playerGetDensity(Player& player);
    void playerMove(Player& player, int steps);
};
#endif // GAME_HPP_INCLUDED