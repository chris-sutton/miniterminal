// Author: Christopher Sutton
// Date: 8/30/2020
// Project 1: See p1.txt for description.
//

#include "p1.h"

int main() {
    std::string userInput = "";
    Terminal *term = new Terminal();

    while(true) {
        std::getline(std::cin, userInput);
        term->recieveInput(userInput);
        userInput = "";
    }
    return 0;
}