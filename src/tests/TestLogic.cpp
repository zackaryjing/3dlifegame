#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include "../logic/GameOfLife.hpp"
int main() {
    Layer::setSize(40, 15);
    Layer layer1(0);
    layer1.randomInit();
    for (int i = 0; i < 500; ++i) {
        layer1.debug_output();
        layer1.data = layer1.next();
        usleep(500000);
        std::cout << "\033[2J\033[H" << std::flush;
        // std::system("cls");
    }
}
