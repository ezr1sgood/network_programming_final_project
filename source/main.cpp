#include "game.hpp"

int main() {
    cout << "Game server is started. Waiting players..." << endl;
    Game game;
    game.run();
    return 0;
}