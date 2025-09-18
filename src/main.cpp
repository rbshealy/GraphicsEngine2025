//
// Created by raysh on 9/16/2025.
//

#include <first_app.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
    GEngine::FirstApp app{};
    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;
}