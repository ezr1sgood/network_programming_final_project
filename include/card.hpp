#ifndef CARD_HPP_INCLUDED
#define CARD_HPP_INCLUDED
#include <string>

class Card {
private:
    std::string name;
    std::string statement;
public:
    Card(std::string name, std::string statement);
    std::string getName();
    std::string getStatement();
};
#endif // CARD_HPP_INCLUDED