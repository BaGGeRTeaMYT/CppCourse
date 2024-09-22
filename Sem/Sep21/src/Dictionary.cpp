#include <Dictionary.h>

Dictionary::Dictionary() {
    std::fstream file;
    file.open("../dict.txt");
    std::string tmp;
    while (!file.eof()) {
        file >> tmp;
        m_data.push_back(tmp);
    }
    file.close();
}

const int Dictionary::getDictSize() const {
    return m_data.size();
}

const std::string Dictionary::getWord(unsigned int index) const {
    return m_data[index];
}