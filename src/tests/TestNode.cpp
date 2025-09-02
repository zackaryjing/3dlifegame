#include <iostream>
#include <memory>
#include <utils/Node.hpp>

using namespace std;

int main() {
    auto t = Node<size_t>::with_capacity(10);
    for (int i = 0; i < 20; ++i) {
        cout << t->val << endl;
        t = t->next;
    }
}
