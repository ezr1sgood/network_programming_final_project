#ifndef CARD_HPP_INCLUDED
#define CARD_HPP_INCLUDED
#include <string>

class Card {
private:
    std::string cardId;
    std::string name;
    std::string statement;
public:
    Card(std::string id, std::string name, std::string statement);
    std::string getName();
    std::string getStatement();
    std::string getId();
};
#endif // CARD_HPP_INCLUDED