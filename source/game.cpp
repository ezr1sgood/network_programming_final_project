#include "../include/game.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono> 
#include "game.hpp"
#define MAP_SIZE 18

Game::Game() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    this->status = true;
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

    std::shuffle(chance.begin(), chance.end(), std::default_random_engine(seed));

    // initialize the density cards
    density = vector<Card>({
        Card("快步過馬路",
             "因為你在台灣快步走過馬路，導致視野死角，被機車撞到。\n效果：生命值減少 20。"),
        Card("慢步過馬路",
             "因為你在台灣慢步走過馬路，引起大眾觀感不佳。\n效果：政黨階級 -1。"),
        Card("背鍋俠",
             "你是教育部的臉書粉專小編，教育部亂拍影片遭到公眾撻伐，你被迫『具名』背鍋，身心受創。\n效果：生命值減少 20。\nhttps://fongnews.net/political/83116/"),
        Card("大撒幣",
             "總統大選即將到來，政府決定補助私立大學學費 3.5 萬元，但花的不是他的錢，是你這個納稅人的錢。\n效果：你的金錢減少 500。"),
        Card("中樂透",
             "在小七無聊買個可爾必思沒事就中了 1000 萬元發票。\n效果：你的金錢增加 5000。")
    });

    std::shuffle(density.begin(), density.end(), std::default_random_engine(seed));

    // initialize the map
    map = vector<Grid>({
        Grid("起點", START_GRID),
        Grid("陽明大學光復校區", REAL_ESTATE, 200, 50),
        Grid("藍營總部", BLUE_PARTY),
        Grid("清華大工地", REAL_ESTATE, 5, 1),
        Grid("機會", CHANCE_GRID),
        Grid("U2電影館", REAL_ESTATE, 100, 100),
        Grid("苗栗國", REAL_ESTATE, 1000, 200),
        Grid("新竹12億棒球場", REAL_ESTATE, 10000, 5000),
        Grid("綠島監獄", JAIL_GRID),
        Grid("台北101", REAL_ESTATE, 5000, 1000),
        Grid("高雄愛情摩天輪", REAL_ESTATE, 2000, 4000),
        Grid("綠營總部", GREEN_PARTY),
        Grid("用愛發電發電廠", REAL_ESTATE, 100, 5),
        Grid("命運", CHANCE_GRID),
        Grid("中巨蛋", REAL_ESTATE, 1000, 1200),
        Grid("三立假新聞總部", REAL_ESTATE, 1000, 1),
        Grid("立法院（猴子院）", REAL_ESTATE, 3000, 5000),
        Grid("交大二餐小七", REAL_ESTATE, 100, 2000),
    });
}

// TODO: implement this function
void Game::SendMessageToClient(string message, int sockfd = 0) {
     std::cout << message << endl;
}

// TODO: remove brackets of for loop
void Game::SendMessageToAllClients(string message) {
     // for (auto &player : this->players) {
          SendMessageToClient(message);
     // }
}

// TODO: implement this function
string Game::ReadMessageFromClient(int sockfd = 0) {
     string message;
     std::cin >> message;
     return message;
}

void Game::victory(Player& player) {
     for (auto &player : this->players) {
          SendMessageToClient(player.getName() + "贏得了遊戲！");
     }
     exit(0);
}

void Game::addPlayer(string name, int id) {
     players.push_back(Player(name, id));
}

void Game::playerGetChance(Player& player) {
    if (chance.empty()) {
        cerr << "No chance card." << endl;
        return;
    }
    Card chanceCard = chance.back();
    chance.pop_back();
    player.addSkill(chanceCard);

    SendMessageToClient("你得到了機會: " + chanceCard.getName());
    SendMessageToClient(chanceCard.getStatement());
}

void Game::playerGetDensity(Player& player) {
     ;
}

void Game::handlePartyEvent(Player& player, int partyId){
     int playerParty = player.getParty();
     string partyName = (partyId == BLUE_PARTY ? "國民黨" : "民進黨");
     SendMessageToClient("你所在的格子為政黨格： " + partyName);
     if (player.getMoney() < 500) {
          SendMessageToClient("你的錢不夠支付政治獻金，無法提升政黨階級。");
          return;
     } 

     if (partyId == playerParty) {
          SendMessageToClient( "支付 500 政治獻金，可以提升政黨階級。");
          SendMessageToClient( "輸入 Y 支付，輸入 N 放棄。");
          std::string choice = ReadMessageFromClient();
          // TODO: Maybe we can use while loop to check the input is "Y" neither "N".
          
          // convert choice to upper case.
          std::transform(choice.begin(), choice.end(), choice.begin(), ::toupper);
          if (choice == "Y") {
               player.addMoney(-500);
               player.setPartyLevel(player.getPartyLevel() + 1);
               SendMessageToClient( "你提升了政黨階級，目前的階級為：" + to_string(player.getPartyLevel()));
          } else {
               SendMessageToClient( "你放棄了提升政黨階級。");
          }
     } else {
          if (playerParty == 0) {
               SendMessageToClient( "支付 500 政治獻金，可以加入" + partyName);
          } else {
               SendMessageToClient( "你現在已經有政黨了，但你仍然可以支付政治獻金轉換政黨。");
               SendMessageToClient( "支付 500 政治獻金，可以加入" + partyName);
          }
          SendMessageToClient( "輸入 Y 支付，輸入 N 放棄。");
          char choice; std::cin >> choice;
          if (choice == 'Y' || choice == 'y') {
               player.addMoney(-500);
               player.setParty(partyId);
               player.setPartyLevel(1);
               SendMessageToClient( "你加入了政黨，目前的階級為：" + to_string(player.getPartyLevel()));
          } else {
               SendMessageToClient( "你放棄了加入政黨。");
          }
     }
}

void Game::handleRealEstateEvent(Player& player, Grid& grid) {
     int ownerId = grid.getOwner();
     int tex = grid.getPayTex();
     int price = grid.getPrice();
     int level = grid.getLevel();
     int playerId = player.getId();
     string playerName = player.getName();
     int playerSockfd = player.getSockfd();

     if (ownerId == 0) { // no owner
          SendMessageToClient("你可以購買這塊土地，花費" + to_string(price) + "元。");
          SendMessageToClient("輸入 Y 購買，輸入 N 放棄。");
          std::string choice = ReadMessageFromClient();
          if (choice == "Y" || choice == "y") {
               if (player.getMoney() < price) {
                    SendMessageToClient("你的錢不夠支付購買費用。");
               } else {
                    player.addMoney(-price);
                    grid.setOwner(playerId);
                    grid.setLevel(1);
                    SendMessageToClient("你購買了這塊土地。");
               }
          }
     } else {
          Player &owner = players[ownerId-1];
          int ownerSockfd = owner.getSockfd();
          if (ownerId == playerId) { // own by player
               SendMessageToClient("你已經擁有這塊土地了，花費" + to_string(tex) + "元升級。", playerSockfd);
               if (player.getMoney() < tex) {
                    SendMessageToClient("你的錢不夠支付升級費用。");
               } else {
                    SendMessageToClient("輸入 Y 升級，輸入 N 放棄。", playerSockfd);
                    std::string choice = ReadMessageFromClient(playerSockfd);
                    if (choice == "Y" || choice == "y") {
                         player.addMoney(-tex);
                         int currentLevel = grid.getLevel();
                         grid.setLevel(currentLevel + 1);
                         SendMessageToClient("你升級了這塊土地，目前的等級為：" + to_string(level));
                    }
               }
          } else { // own by other player
          std::cout << "ownerId: " << ownerId << endl;
               SendMessageToClient("你必須支付" + to_string(tex) + "元給" + owner.getName() + "。", playerSockfd);
               SendMessageToClient(playerName + "踏入了你的房地產，你獲得了" + to_string(tex) + "元。", ownerSockfd);
               player.addMoney(-tex);
               owner.addMoney(tex);
          }
     }
}

void Game::playerTurn(Player& player) {
     const string playerName = player.getName();
     SendMessageToAllClients(playerName + "'s turn.");
     
     // check jail duration
     int playerJailDuration = player.getJailDuration();
     if (playerJailDuration > 0) {
         SendMessageToAllClients(playerName + " 正在服刑，跳過一回合。");
         player.setJailDuration(playerJailDuration - 1);
         return;
     }

     SendMessageToClient("輸入一個字串作為骰子的隨機種子:");
     string dice_seed = ReadMessageFromClient();
     int sum = accumulate(dice_seed.begin(), dice_seed.end(), 0);
     srand(sum);

     int step = rand() % 6 + 1; // throw a dice
     int playerOldPosition = player.getLocation();

     SendMessageToAllClients("骰到了 " + to_string(step) + " ！");
     player.move(step);

     int playerNowPosition = player.getLocation();
     int gridType = map[playerNowPosition].getType();

     if (playerOldPosition > playerNowPosition) {
          SendMessageToAllClients(playerName + "經過了起點，獲得 2000 元！！");
          player.addMoney(2000);
     }

     std::string gridName = map[playerNowPosition].getName();
     SendMessageToAllClients(playerName + "抵達" + gridName + "。");
     switch (gridType) {
           case START_GRID:
                player.addMoney(2000);
                break;
           case BLUE_PARTY:
                handlePartyEvent(player, BLUE_PARTY);
                break;
           case GREEN_PARTY:
                handlePartyEvent(player, GREEN_PARTY);
                break;
           case CHANCE_GRID:
                playerGetChance(player);
                break;
           case DENSITY_GRID:
                playerGetDensity(player);
                break;
           case JAIL_GRID:
                player.setJailDuration(2);
                break;
           case REAL_ESTATE:
                handleRealEstateEvent(player, this->map[playerNowPosition]);
                break;
           default:
                std::cerr << "You are at the unknown grid." << endl;
                break;
     }
}

void Game::endTurn(Player& player) {
     const string playerName = player.getName();
     SendMessageToAllClients(playerName + "'s turn end.");
     SendMessageToAllClients("--------------------------");
}

void Game::run() {
     if (players.empty()) {
          std::cerr << "No player." << endl;
          return;
     }
     SendMessageToAllClients("Starting the game...");
     for (int Round=1; ; Round++) {
          SendMessageToAllClients("Round " + to_string(Round));
          for (auto &player : this->players) {
               playerTurn(player);
               endTurn(player);
          }
          if (Round == 20) {
               SendMessageToAllClients("Game end.");
               break;
          }
     }
}