#include "../include/game.hpp"
#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <random>
#include <chrono> 
#include <string.h>
#include "../include/game.hpp"
#define MAP_SIZE 18
#define BUFFER_SIZE 1024

Game::Game() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    this->status = true;
    // initialize the chance cards
    chance = std::vector<Card>({
        Card("c-1", "外役監啦，哈哈!", 
             "你必須已經加入一個政黨才能使用此卡片\n效果：如果當前所在格子為監獄，則可立即離開監獄，並且獲得獄中人脈，政黨階級+1。"),
        Card("c-2", "政治鬥爭",
             "效果：指定一個已經有政黨的玩家，使其政黨階級 -1。"),
        Card("c-3", "神風特攻隊",
             "效果：攻擊其中一個格子，建築物夷平玩家生命值減少 50。"),
        Card("c-4", "機車三寶",
             "效果：指定一位玩家，令該玩家生命值減少 20"),
    });

    std::shuffle(chance.begin(), chance.end(), std::default_random_engine(seed));

    // initialize the density cards
    density = std::vector<Card>({
        Card("d-1", "快步過馬路",
             "因為你在台灣快步走過馬路，導致視野死角，被機車撞到。\n效果：生命值減少 20。"),
        Card("d-2", "慢步過馬路",
             "因為你在台灣慢步走過馬路，引起大眾觀感不佳。\n效果：政黨階級 -1。"),
        Card("d-3", "背鍋俠",
             "你是教育部的臉書粉專小編，教育部亂拍影片遭到公眾撻伐，你被迫『具名』背鍋，身心受創。\n效果：生命值減少 50。\nhttps://fongnews.net/political/83116/"),
        Card("d-4", "大撒幣",
             "總統大選即將到來，政府決定補助私立大學學費 3.5 萬元，但花的不是他的錢，是你這個納稅人的錢。\n效果：你的金錢減少 500。"),
        Card("d-5", "中樂透",
             "在小七無聊買個可爾必思沒事就中了 1000 萬元發票。\n效果：你的金錢增加 5000。")
    });

    std::shuffle(density.begin(), density.end(), std::default_random_engine(seed));

    // initialize the map
    map = std::vector<Grid>({
        Grid("起點", 1, START_GRID),
        Grid("陽明大學光復校區", 2, REAL_ESTATE, 200, 50),
        Grid("藍營總部", 3, BLUE_PARTY),
        Grid("清華大工地", 4, REAL_ESTATE, 5, 1),
        Grid("機會", 5, CHANCE_GRID),
        Grid("U2電影館", 6, REAL_ESTATE, 100, 100),
        Grid("苗栗國", 7, REAL_ESTATE, 1000, 200),
        Grid("新竹12億棒球場", 8, REAL_ESTATE, 10000, 5000),
        Grid("綠島監獄", 9, JAIL_GRID),
        Grid("台北101", 10, REAL_ESTATE, 5000, 1000),
        Grid("高雄愛情摩天輪", 11, REAL_ESTATE, 2000, 4000),
        Grid("綠營總部", 12, GREEN_PARTY),
        Grid("用愛發電發電廠", 13, REAL_ESTATE, 100, 5),
        Grid("命運", 14, CHANCE_GRID),
        Grid("中巨蛋", 15, REAL_ESTATE, 1000, 1200),
        Grid("三立假新聞總部", 16, REAL_ESTATE, 1000, 1),
        Grid("立法院（猴子院）", 17, REAL_ESTATE, 3000, 5000),
        Grid("交大二餐小七", 18, REAL_ESTATE, 100, 2000),
    });
}

// TODO: implement this function
void Game::SendMessageToClient(std::string message, int sockfd = 0) {
     message += "\n";
     if (sockfd == 0) {
          std::cerr << "no sockfd: " + message << std::endl;
          return;
     }
     char sendline[BUFFER_SIZE];
     bzero(&sendline, sizeof(sendline));
     sprintf(sendline, "%s", message.c_str());
     write(sockfd, sendline, strlen(sendline));
}

// TODO: remove brackets of for loop
void Game::SendMessageToAllClients(std::string message) {
     for (auto &player : this->players) {
          std::cerr << "send message to " + player.getName() + ":" + message << std::endl;
          SendMessageToClient(message, player.getSockfd());
     }
}

// TODO: implement this function
std::string Game::ReadMessageFromClient(int sockfd = 0) {
     char recvline[BUFFER_SIZE];
     bzero(&recvline, sizeof(recvline));
     read(sockfd, recvline, sizeof(recvline));
     std::string message(recvline);
     std::cerr << "read message: " + message << std::endl;
     return message;
}

void Game::victory(Player& player) {
     SendMessageToAllClients(player.getName() + "贏得了遊戲！");
     exit(0);
}

void Game::addPlayer(std::string name, int id, int sockfd) {
     this->players.push_back(Player(name, id, sockfd));
}

void Game::playerGetChance(Player& player) {
    if (chance.empty()) {
        std::cerr << "No chance card." << std::endl;
        return;
    }
    Card chanceCard = chance.back();
    chance.pop_back();
    player.addSkill(chanceCard);

    SendMessageToAllClients("\\\\\\\\\\" + player.getName() + "得到了機會: " + chanceCard.getName() + "/////");
    SendMessageToAllClients(chanceCard.getStatement());
}

void Game::playerGetDensity(Player& player) {
     auto playerName = player.getName();
     auto densityCard = density.back();
     density.pop_back();
     auto densityId = densityCard.getId();
     auto densityName = densityCard.getName();
     auto densityStatement = densityCard.getStatement();
     SendMessageToAllClients("!!!!!!" + playerName + " 得到了命運: " + densityName + "!!!!!!");
     SendMessageToAllClients(densityStatement);

     if (densityId == "d-1") {
          player.addHelth(-20);
     } else if (densityId == "d-2") {
          player.setPartyLevel(player.getPartyLevel() - 1);
     } else if (densityId == "d-3") {
          player.addHelth(-50);
     } else if (densityId == "d-4") {
          player.addMoney(-500);
     } else if (densityId == "d-5") {
          player.addMoney(5000);
     }
}

void Game::handlePartyEvent(Player& player, int partyId){
     int playerParty = player.getParty();
     std::string partyName = (partyId == BLUE_PARTY ? "國民黨" : "民進黨");
     SendMessageToClient("你所在的格子為政黨格： " + partyName, player.getSockfd());
     if (player.getMoney() < 500) {
          SendMessageToClient("你的錢不夠支付政治獻金，無法提升政黨階級。", player.getSockfd());
          return;
     } 

     if (partyId == playerParty) {
          SendMessageToClient( "支付 500 政治獻金，可以提升政黨階級。", player.getSockfd());
          SendMessageToClient( "輸入 Y 支付，輸入 N 放棄。", player.getSockfd());
          std::string choice = ReadMessageFromClient(player.getSockfd());
          // TODO: Maybe we can use while loop to check the input is "Y" neither "N".
          
          // convert choice to upper case.
          std::transform(choice.begin(), choice.end(), choice.begin(), ::toupper);
          if (choice == "Y") {
               player.addMoney(-500);
               player.setPartyLevel(player.getPartyLevel() + 1);
               SendMessageToClient( "你提升了政黨階級，目前的階級為：" + std::to_string(player.getPartyLevel()), player.getSockfd());
          } else {
               SendMessageToClient( "你放棄了提升政黨階級。", player.getSockfd());
          }
     } else {
          if (playerParty == 0) {
               SendMessageToClient( "支付 500 政治獻金，可以加入" + partyName, player.getSockfd());
          } else {
               SendMessageToClient( "你現在已經有政黨了，但你仍然可以支付政治獻金轉換政黨。", player.getSockfd());
               SendMessageToClient( "支付 500 政治獻金，可以加入" + partyName, player.getSockfd());
          }
          SendMessageToClient( "輸入 Y 支付，輸入 N 放棄。");
          std::string choice = ReadMessageFromClient(player.getSockfd());
          if (choice[0] == 'Y' || choice[0] == 'y') {
               player.addMoney(-500);
               player.setParty(partyId);
               player.setPartyLevel(1);
               SendMessageToClient( "你加入了政黨，目前的階級為：" + std::to_string(player.getPartyLevel()), player.getSockfd());
          } else {
               SendMessageToClient( "你放棄了加入政黨。", player.getSockfd());
          }
     }
}

void Game::handleRealEstateEvent(Player& player, Grid& grid) {
     auto playerSockfd = player.getSockfd();
     int ownerId = grid.getOwner();
     int tex = grid.getPayTex();
     int price = grid.getPrice();
     int level = grid.getLevel();
     int playerId = player.getId();
     std::string playerName = player.getName();

     if (ownerId == 0) { // no owner
          SendMessageToClient("你可以購買這塊土地，花費" + std::to_string(price) + "元。", playerSockfd);
          SendMessageToClient("輸入 Y 購買，輸入 N 放棄。", playerSockfd);
          std::string choice = ReadMessageFromClient(playerSockfd);
          if (choice[0] == 'Y' || choice[0] == 'y') {
               if (player.getMoney() < price) {
                    SendMessageToClient("你的錢不夠支付購買費用。", playerSockfd);
               } else {
                    player.addMoney(-price);
                    grid.setOwner(playerId);
                    grid.setLevel(1);
                    SendMessageToClient("你購買了這塊土地。", playerSockfd);
               }
          }
     } else {
          Player &owner = players[ownerId-1];
          int ownerSockfd = owner.getSockfd();
          if (ownerId == playerId) { // own by player
               SendMessageToClient("你已經擁有這塊土地了，花費" + std::to_string(tex) + "元升級。", playerSockfd);
               if (player.getMoney() < tex) {
                    SendMessageToClient("你的錢不夠支付升級費用。", playerSockfd);
               } else {
                    SendMessageToClient("輸入 Y 升級，輸入 N 放棄。", playerSockfd);
                    std::string choice = ReadMessageFromClient(playerSockfd);
                    if (choice == "Y" || choice == "y") {
                         player.addMoney(-tex);
                         int currentLevel = grid.getLevel();
                         grid.setLevel(currentLevel + 1);
                         SendMessageToClient("你升級了這塊土地，目前的等級為：" + std::to_string(level), playerSockfd);
                    }
               }
          } else { // own by other player
               SendMessageToClient("你必須支付" + std::to_string(tex) + "元給" + owner.getName() + "。", playerSockfd);
               SendMessageToClient(playerName + "踏入了你的房地產，你獲得了" + std::to_string(tex) + "元。", ownerSockfd);
               player.addMoney(-tex);
               owner.addMoney(tex);
          }
     }
}

void Game::sendStatus(){

     SendMessageToAllClients("status of player");
     for(auto &player: this -> players){

          SendMessageToAllClients("| Player\t\t" + player.getName() + "\t|");
          SendMessageToAllClients("| Health\t\t" + std::to_string(player.getHelth()) + "\t|");
          SendMessageToAllClients("| Money\t\t\t" + std::to_string(player.getMoney()) + "\t|");
          SendMessageToAllClients("| Location\t\t" + std::to_string(player.getLocation()) + "\t|");
          SendMessageToAllClients("| Party\t\t\t" + std::to_string(player.getParty()) + "\t|");
          SendMessageToAllClients("| Party Level\t\t" + std::to_string(player.getPartyLevel()) + "\t|");
          SendMessageToAllClients("|-------------------------------|");
     }
     SendMessageToAllClients("end of status");
}
void Game::sendMap(Player &player){

     int playerNowPosition = player.getLocation();
     std::string gridName = map[playerNowPosition].getName();
     std::string prevGridName = map[(playerNowPosition - 1 + MAP_SIZE) % MAP_SIZE].getName();
     std::string nextGridName = map[(playerNowPosition + 1) % MAP_SIZE].getName();

     SendMessageToClient("map of player", player.getSockfd());

     SendMessageToClient(prevGridName, player.getSockfd());
     SendMessageToClient(gridName, player.getSockfd());
     SendMessageToClient(nextGridName, player.getSockfd());

     SendMessageToClient("end of map", player.getSockfd());
}

void Game::playerTurn(Player& player) {
     int playerSockfd = player.getSockfd();
     const std::string playerName = player.getName();
     SendMessageToAllClients(playerName + "'s turn.");
     
     // check jail duration
     int playerJailDuration = player.getJailDuration();
     if (playerJailDuration > 0) {
         SendMessageToAllClients(playerName + " 正在服刑，跳過一回合。");
         player.setJailDuration(playerJailDuration - 1);
         return;
     }

     SendMessageToClient("輸入一個字串作為骰子的隨機種子:", playerSockfd);
     std::string dice_seed = ReadMessageFromClient(playerSockfd);
     int sum = accumulate(dice_seed.begin(), dice_seed.end(), 0);
     srand(sum);

     int step = rand() % 6 + 1; // throw a dice
     int playerOldPosition = player.getLocation();

     SendMessageToAllClients("骰到了 " + std::to_string(step) + " ！");
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
                std::cerr << "You are at the unknown grid." << std::endl;
                break;
     }
}

void Game::endTurn(Player& player) {
     const std::string playerName = player.getName();
     SendMessageToAllClients(playerName + "'s turn end.");
     SendMessageToAllClients("--------------------------");
}

void Game::run() {
     if (players.empty()) {
          std::cerr << "No player." << std::endl;
          return;
     }
     SendMessageToAllClients("------遊戲開始------");
     for (int Round=1; ; Round++) {
          SendMessageToAllClients("Round " + std::to_string(Round));
          sendStatus();
          for(auto &player: this->players){

               sendMap(player);
          }
          for (auto &player : this->players) {
               playerTurn(player);
               endTurn(player);
          }
          if (Round == 2) {
               sendStatus();
               for(auto &player: this->players){

                    sendMap(player);
               }
               SendMessageToAllClients("Game end.");
               break;
          }
     }
}