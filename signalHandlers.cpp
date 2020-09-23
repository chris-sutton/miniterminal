#include "signalHandlers.h"

void signalChildExitHandler(int signum) {
    int status = 0;
    int pid = waitpid(-1, &status, WNOHANG);
    std::cout << "[" << pid << "] child finished with status " << status << std::endl;
}

// I need to get this to only output for background processes. I also need to get the limit to work.