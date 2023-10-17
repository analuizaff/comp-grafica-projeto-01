#include "card.hpp"

Card::Card() : m_value(""), m_faceUp(false) {}

Card::Card(const std::string& value) : m_value(value), m_faceUp(false) {}

const std::string& Card::getValue() const {
    return m_value;
}

bool Card::isFaceUp() const {
    return m_faceUp;
}

void Card::flip() {
    m_faceUp = !m_faceUp;
}

bool operator==(const Card& lhs, const Card& rhs) {
    return lhs.m_value == rhs.m_value;
}