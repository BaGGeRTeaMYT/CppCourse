// #ifndef GameWisherHEADER
// #define GameWisherHEADER

#include <Dictionary.h>
#include <random>

class GameWisher {
    public:
        GameWisher();
        GameWisher(GameWisher&) = delete;
        void newGame();
        std::string checkWord(const std::string& word) const;
    private:
        Dictionary m_dict;
        std::vector<int> m_letters;
        std::string m_word;
        void processWord();
};

// #endif // GameWisherHEADER