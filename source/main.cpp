#include "../include/game.hpp"

int main() {
    cout << "Game server is started. Waiting players..." << endl;
    Game game;
    game.addPlayer("Ben");
    game.addPlayer("Ryan");
    game.addPlayer("Jojo");
    game.run();
    game.showStatus();
    return 0;
}