#include <Player.h>

Player::Player(std::vector<wordWithLetters> possibleWords):
    m_mask(""),
    m_possibleWords(possibleWords) {

}

void Player::guess() {
    m_lastGuess = m_possibleWords[0].word;
}

void Player::updateWords() {
    std::vector<wordWithLetters> newPossibilities;
    if (firstTurn) {
        for (unsigned int i = 0; i < m_possibleWords.size(); i++) {
            if (m_possibleWords[i].word.size() == m_info.length) {
                newPossibilities.push_back(m_possibleWords[i]);
            }
        }
        m_possibleWords = newPossibilities;
    }
    for (unsigned int i = 0; i < m_possibleWords.size(); i++) {
        for (unsigned int j = 0; j < m_mask.size(); j++) {
            if (m_mask[j] == '+') {
                
            }
        }
    }
}