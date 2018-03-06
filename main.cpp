#include <iostream>

void invoke(void(*f)()){
    f();
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto f = []()->void{
        std::cout << "Hello, lambda!" << std::endl;
    };
    invoke(f);
    return 0;
}