#ifndef CARD_HPP_INCLUDED
#define CARD_HPP_INCLUDED
#include <string>
using namespace std;

class Card {
private:
    string name;
    string statement;
public:
    Card(string name, string statement);
    string getName();
    string getStatement();
};
#endif // CARD_HPP_INCLUDED