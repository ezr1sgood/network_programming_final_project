#include "../include/game.hpp"

int main() {
    cout << "Game server is started. Waiting players..." << endl;
    Game game;
    game.addPlayer("Ben",1);
    game.addPlayer("Ryan",2);
    game.addPlayer("Jojo",3);
    game.run();
    game.showStatus();
    return 0;
}