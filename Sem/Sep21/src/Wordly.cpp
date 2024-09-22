#include <Wordly.h>

Wordly::Wordly(): m_wisher(), m_console(), m_winFlag(false), m_quitFlag(false), m_tries(0) {
    m_wisher.newGame();
    gameLoop();
}

void Wordly::gameLoop() {
    auto input = m_console.process();
    if (m_winFlag) {
        if (m_console.isWord) {
            std::cout << "You have already guessed the word correctly" << std::endl
                      << "If you wish to start a new game type -NG" << std::endl
                      << "Otherwise: -q" << std::endl;
        } else if (m_console.incorrectInput) {
            std::cout << "Incorrect input given" << std::endl
                      << "If you wish to start a new game type -NG" << std::endl
                      << "Otherwise: -q" << std::endl;
        } else {
            if (m_console.signals & 1) {
                m_winFlag = false;
                m_wisher.newGame();
            }
            if (m_console.signals & 2) {
                m_quitFlag = true;
                return;
            }
        }
    } else {
        if (m_console.isWord) {
            std::string outputString = m_wisher.checkWord(input.value());
            m_winFlag = true;
            m_tries++;
            for (char symbol : outputString) {
                if (symbol != 'X') {
                    m_winFlag = false;
                    break;
                }
            }
            if (m_winFlag) {
                winMessage();
                return;
            }
            std::cout << outputString << std::endl;
        } else if (m_console.incorrectInput) {
            std::cout << "Incorrect input given" << std::endl
                      << "Type one word without spaces" << std::endl;
        } else {
            if (m_console.signals & 1) {
                m_wisher.newGame();
            }
            if (m_console.signals & 2) {
                m_quitFlag = true;
                return;
            }
        }
    }
}

bool Wordly::shouldRun() const {
    return !m_quitFlag;
}

void Wordly::winMessage() {
    std::cout << "Your guess is correct! Total attempts: " << m_tries << std::endl
              << "If you wish to start a new game type -NG" << std::endl
              << "Otherwise: -q"
              << std::endl;
}