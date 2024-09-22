#include <GameWisher.h>
#include <iostream>

GameWisher::GameWisher(): m_dict() {
    std::random_device rnd;
    m_word = m_dict.getWord( static_cast<unsigned int>(rnd()%m_dict.getDictSize()) );
    processWord();
}

void GameWisher::newGame() {
    std::random_device rnd;
    m_word = m_dict.getWord( static_cast<unsigned int>(rnd()%m_dict.getDictSize()) );
    processWord();
}

void GameWisher::processWord() {
    m_letters = std::vector<int>(26, 0);
    for (char letter : m_word) {
        m_letters[letter - 'a']++;
    }
}

std::string GameWisher::checkWord(const std::string& word) const {
    std::vector<int> letters(26, 0);
    std::string answer = "";
    for (unsigned int i = 0; i < m_word.size(); i++) {
        if (i >= word.size()) {
            answer += "-";
        } else {
            char letter = tolower(word[i]);
            unsigned int index = letter - 'a';
            letters[index]++;
            if (letter == m_word[i]) {
                answer += "X";
            } else if (letters[index] <= m_letters[index]) {
                answer += "+";
            } else {
                answer += "-";
            }
        }
    }
    return answer;
}
