#ifndef DictionaryHEADER
#define DictionaryHEADER

#include <string>
#include <fstream>
#include <map>
#include <vector>

struct wordWithLetters {
    std::string word;
    std::vector<int> letters = std::vector<int>(26, 0);
};

class Dictionary {
    public:
        Dictionary();
        const int getDictSize() const;
        const std::string& getWord(unsigned int index) const;
        std::vector<wordWithLetters> getWords();
    private:
        std::vector<wordWithLetters> m_data;
};

#endif // DictionaryHEADER