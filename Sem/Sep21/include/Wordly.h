#ifndef WordlyHEADER
#define WordlyHEADER

#include <GameWisher.h>
#include <Console.h>
#include <Player.h>

class Wordly {
    public:
        Wordly();
        void gameLoop();
        bool shouldRun() const;
    private:
        void winMessage();
        GameWisher m_wisher;
        Console m_console;
        Player m_player;
        bool m_winFlag;
        bool m_quitFlag;
        unsigned int m_tries;
};

#endif // WordlyHEADER