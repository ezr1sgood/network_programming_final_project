#include "../include/game.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono> 

Game::Game() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // initialize the chance cards
    chance = vector<Card>({
        Card("外役監啦，哈哈!", 
             "你必須已經加入一個政黨才能使用此卡片\n效果：如果當前所在格子為監獄，則可立即離開監獄，並且獲得獄中人脈，政黨階級+1。"),
        Card("政治鬥爭",
             "效果：指定一個已經有政黨的玩家，使其政黨階級 -1。"),
        Card("神風特攻隊",
             "效果：攻擊其中一個格子，建築物夷平玩家生命值減少 50。"),
        Card("機車三寶",
             "效果：指定一位玩家，令該玩家生命值減少 20"),
    });

    shuffle(chance.begin(), chance.end(), std::default_random_engine(seed));

    // initialize the density cards
    density = vector<Card>({
        Card("快步過馬路",
             "效果：因為你在台灣快步走過馬路，導致視野死角，被機車撞到，生命值減少 20。"),
        Card("慢步過馬路",
             "效果：因為你在台灣慢步走過馬路，引起大眾觀感不佳，政黨階級 -1。"),
        Card("背鍋俠",
             "如果你沒有加入政黨，此命運對你無效。\n效果：你是教育部的臉書粉專小編，教育部亂拍影片遭到公眾撻伐，你被迫『具名』背鍋，身心受創，生命值減少 20。\nhttps://fongnews.net/political/83116/"),
    });

    shuffle(density.begin(), density.end(), std::default_random_engine(seed));

    // initialize the map
    map = vector<Grid>({
        Grid("起點", START_GRID),
        Grid("AA", REAL_ESTATE, 100, 33),
        Grid("藍營總部", BLUE_PARTY),
        Grid("BB", REAL_ESTATE, 100, 33),
        Grid("機會", CHANCE_GRID),
        Grid("DD", REAL_ESTATE, 100, 33),
        Grid("EE", REAL_ESTATE, 100, 33),
        Grid("FF", REAL_ESTATE, 100, 33),
        Grid("監獄", JAIL_GRID),
        Grid("GG", REAL_ESTATE, 100, 33),
        Grid("HH", REAL_ESTATE, 100, 33),
        Grid("綠營總部", GREEN_PARTY),
        Grid("JJ", REAL_ESTATE, 100, 33),
        Grid("命運", CHANCE_GRID),
        Grid("LL", REAL_ESTATE, 100, 33),
        Grid("MM", REAL_ESTATE, 100, 33),
        Grid("NN", REAL_ESTATE, 100, 33),
        Grid("OO", REAL_ESTATE, 100, 33),
    });
}

void Game::playerGetChance(Player& player) {
    if (chance.empty()) {
        cout << "No chance card." << endl;
        return;
    }
    Card chanceCard = chance.back();
    chance.pop_back();
    player.addSkill(chanceCard);

    cout << "You get a chance card: " << chanceCard.getName() << endl;
    cout << 
    chanceCard.getStatement() << endl;
}

void Game::playerGetDensity(Player& player) {
     ;
}

void Game::handlePartyEvent(Player& player, int partyId){
     int playerParty = player.getParty();
     if (partyId == playerParty) {
          cout << "支付 500 政治獻金，可以提升政黨階級。" << endl;
          cout << "輸入 Y 支付，輸入 N 放棄。" << endl;
          char choice; cin >> choice;
          if (choice == 'Y' || choice == 'y') {
               player.addMoney(-500);
               player.setPartyLevel(player.getPartyLevel() + 1);
               cout << "你提升了政黨階級，目前的階級為：" << player.getPartyLevel() << endl;
          } else {
               cout << "你放棄了提升政黨階級。" << endl;
          }
     } else {
          string partyName = (partyId == BLUE_PARTY ? "國名黨" : "民進黨");
          if (playerParty == 0) {
               cout << "支付 500 政治獻金，可以加入" << partyName << endl;
          } else {
               cout << "你現在已經有政黨了，但你仍然可以支付政治獻金轉換政黨。" << endl;
               cout << "支付 500 政治獻金，可以加入" << partyName << endl;
          }
          cout << "輸入 Y 支付，輸入 N 放棄。" << endl;
          char choice; cin >> choice;
          if (choice == 'Y' || choice == 'y') {
               player.addMoney(-500);
               player.setParty(partyId);
               player.setPartyLevel(1);
               cout << "你加入了政黨，目前的階級為：" << player.getPartyLevel() << endl;
          } else {
               cout << "你放棄了加入政黨。" << endl;
          }
     }
}

void Game::playerTurn(Player& player) {
     string playerName = player.getName();
     cout << playerName << "'s turn." << endl;
     
     // check jail duration
     int playerJailDuration = player.getJailDuration();
     if (playerJailDuration > 0) {
         cout << playerName << " is in jail, you can't move." << endl;
         player.setJailDuration(playerJailDuration - 1);
         return;
     }

     cout << "Enter a number to throw a dice:" << endl;
     int dice_seed; cin >> dice_seed;
     int step = rand() % 6; // throw a dice
     player.move(step);

     int playerPosition = player.getLocation();
     int gridType = map[playerPosition].getType();

     switch (gridType) {
           case START_GRID:
                cout << "You are at the start grid." << endl;
                player.addMoney(2000);
                break;
           case BLUE_PARTY:
                cout << "You are at the blue party." << endl;
                handlePartyEvent(player, BLUE_PARTY);
                break;
           case GREEN_PARTY:
                cout << "You are at the green party." << endl;
                handlePartyEvent(player, GREEN_PARTY);
                break;
           case CHANCE_GRID:
                cout << "You are at the chance grid." << endl;
                playerGetChance(player);
                break;
           case DENSITY_GRID:
                cout << "You are at the density grid." << endl;
                playerGetDensity(player);
                break;
           case JAIL_GRID:
                cout << "You are at the jail grid. Stop move for 2 turns." << endl;
                player.setJailDuration(2);
                break;
           case REAL_ESTATE:
                cout << "You are at the real estate grid." << endl;
                break;
           default:
                cout << "You are at the unknown grid." << endl;
                break;
      }

      cout << playerName << "'s turn end." << endl;
}

void Game::run() {
     cout << "Starting the game..." << endl;
     if (players.empty()) {
          cout << "No player." << endl;
          return;
     }
     for (int Round=1; ; Round++) {
          printf("Round %d\n", Round);
          for (auto &player : this->players) {
               playerTurn(player);
               break;
          }
     }
}