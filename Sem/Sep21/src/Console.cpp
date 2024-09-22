#include <Console.h>

Console::Console():
    isWord(false),
    isCommand(false),
    incorrectInput(false) {}

std::optional<std::string> Console::process() {
    updateFlags();
    std::string answer = {};
    std::string userInput;
    std::cin >> userInput;
    bool incorrectFlag = true;
    char symbol = userInput[0];
    if (symbol == '-') {
        isCommand = true;
        incorrectInput = execute(userInput);
    } else if (isalpha(symbol)) {
        isWord = true;
        incorrectInput = wordChecker(userInput);
    }

    isWord &= !incorrectInput;
    if (isWord) {
        answer = userInput;
    }
    isCommand &= !incorrectInput;
    return answer;
}

bool Console::execute(const std::string& command) {
    if (command == static_cast<std::string>("-h")) {
        std::cout << "Possible commands are:" << std::endl
                  << "-h to get this message" << std::endl
                  << "-NG to start new game" << std::endl
                  << "-q to quit playing" << std::endl;
    } else if (command == static_cast<std::string>("-NG")) {
        signals |= 1;
    } else if (command == static_cast<std::string>("-q")) {
        signals |= 2;
    } else {
        return true;
    }
    return false;
}

bool Console::wordChecker(std::string& word) const {
    for (unsigned int i = 0; i < word.size(); i++) {
        if (!isalpha(word[i])) {
            return true;
        }
    }
    return false;
}

void Console::updateFlags() {
    isWord = false;
    isCommand = false;
    incorrectInput = false;
    signals = 0;
}