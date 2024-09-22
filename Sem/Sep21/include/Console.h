// #ifndef ConsoleHEADER
// #define ConsoleHEADER

#include <iostream>
#include <optional>
#include <string>

class Console {
    public:
        Console();
        bool isWord;
        bool isCommand;
        bool incorrectInput;
        unsigned int signals;
        std::optional<std::string> process();
    private:
        bool execute(const std::string& command);
        bool wordChecker(std::string& word) const;
        void updateFlags();
};

// #endif // ConsoleHEADER