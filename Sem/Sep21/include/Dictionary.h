// #ifndef DictionaryHEADER
// #define DictionaryHEADER

#include <string>
#include <fstream>
#include <map>
#include <vector>

class Dictionary {
    public:
        Dictionary();
        const int getDictSize() const;
        const std::string getWord(unsigned int index) const;
    private:
        std::vector<std::string> m_data;
};

// #endif // DictionaryHEADER