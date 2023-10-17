#ifndef CARD_HPP_
#define CARD_HPP_

#include <string>

class Card {
public:
    Card();
    Card(const std::string& value);
    
    const std::string& getValue() const;
    bool isFaceUp() const;
    void flip();

    friend bool operator==(const Card& lhs, const Card& rhs);
    
private:
    std::string m_value;
    bool m_faceUp;
};

#endif