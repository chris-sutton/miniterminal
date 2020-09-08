// Author: Christopher Sutton
// Date: 8/30/2020
// Project 1: See p1.txt for description.
//

#include "p1.h"


int main(int argc, char * argv[]) {

    Terminal *term = new Terminal();
    if (argc == 2)
        term->runTerminal(argv[1]);
    else
        term->runTerminal();
    return 0;
}