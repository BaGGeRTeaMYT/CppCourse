#include <Treap.h>
#include <string>

int main() {
    Treap<std::string, int> test;
    test.insert("Tomato", 5);
    test.insert("Cucumber", 2);
    test.insert("Bread", 3);
    test.insert("Water", 4);
    test.erase("Tomato");
    int i = 0;
    for (const auto &[k, v]: test.range("Cucumber", "ZZZ")) {
        std::cout << i++ << ": {";
        std::cout << k << ", " << v << "}" << std::endl;
    }
    return 0;
}