#include <Wordly.h>

int main(int argc, char* argv[], char* envp[]) {
    Wordly game;
    while (game.shouldRun()) {
        game.gameLoop();
    }
    return 0;
}