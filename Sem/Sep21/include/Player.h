#ifndef PlayerHEADER
#define PlayerHEADER

#include <vector>
#include <string>
#include <sstream>

struct GameParams {
    unsigned int length;
    std::vector<int> letters;

};

struct wordWithLetters {
    std::string word;
    std::vector<int> letters = std::vector<int>(26, 0);
};

class Player {
    public:
        Player() = delete;
        Player(std::vector<wordWithLetters> possibleWords);
    private:
        void guess();
        void updateWords();
        void createMap();
        void stupidCheck();
        bool readMask();
        std::string m_lastGuess;
        std::string m_mask;
        bool firstTurn;
        std::vector<wordWithLetters> m_possibleWords;
        GameParams m_info;
};

#endif // PlayerHEADER