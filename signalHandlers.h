#ifndef _SIGNALHANDLERS_H
#define _SIGNALHANDLERS_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>

void signalChildExitHandler(int signum);
char* convertStringVector(const std::string &str);
#endif