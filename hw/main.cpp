#include <tree.h>
#include <string>

int main() {
    auto test = SearchingTree<std::string, int>("test2", 2);
    test.insert("test1", 1);
    // std::cout << "Begin: " << (*test.begin()).first << std::endl;
    int i = 0;
    for (const auto &[k, v]: test) {
        std::cout << i++ << ": ";
        std::cout << k << ' ' << v << std::endl;
    }
    return 0;
}