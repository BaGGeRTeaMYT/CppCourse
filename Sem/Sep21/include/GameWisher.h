// #ifndef GameWisherHEADER
// #define GameWisherHEADER

#include <Dictionary.h>
#include <random>

class GameWisher {
    public:
        GameWisher();
        void newGame();
        std::string checkWord(const std::string& word) const;
    private:
        const Dictionary m_dict;
        std::vector<int> m_letters;
        std::string m_word;
        void processWord();
};

// #endif // GameWisherHEADER