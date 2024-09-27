#include <Dictionary.h>

Dictionary::Dictionary() {
    std::fstream file;
    file.open("../dict.txt");
    while (!file.eof()) {
        wordWithLetters tmp;
        file >> tmp.word;
        for (char letter : tmp.word) {
            tmp.letters[tolower(letter) - 'a']++;
        }
        m_data.push_back(tmp);
    }
    file.close();
}

const int Dictionary::getDictSize() const {
    return m_data.size();
}

const std::string& Dictionary::getWord(unsigned int index) const {
    return m_data[index].word;
}

std::vector<wordWithLetters> Dictionary::getWords() {
    return m_data;
}