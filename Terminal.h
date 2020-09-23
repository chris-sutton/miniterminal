#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <iomanip>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <dirent.h>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include "signalHandlers.h"

class Terminal {
    public:
        // Constructors
        Terminal();
        // Public variables
        // Public functions
        void runTerminal(std::string argument="");
    private:
        std::string userInput;
        std::map<std::string, std::string> venv;
        std::vector<std::string> *inputArgs;
        std::vector<std::vector<std::string>> inputStatements;
        std::map<std::string, int> commands;

        int cpu, mem;
        struct rlimit mainLimitCPU;
        struct rlimit mainLimitMEM;
        
        void recieveInput(std::string textInput, int isPipe);
        std::string removeComments(std::string rawInputText);
        void runStatement(std::vector<std::string> * statementVector, int isPipe);
        std::vector<std::string>* processStatement(std::string statement);
        std::string getCurrentWorkingDirectory();
        void changeCurrentWorkingDirectory(std::string pathName);
        void printStatement(std::vector<std::string>::const_iterator it, std::vector<std::string>::const_iterator end);
        void setEnvironmentVariable(std::vector<std::string>::const_iterator it, std::vector<std::string>::const_iterator end);
        void unsetEnvironmentVariable(std::vector<std::string>::const_iterator it);
        void printEnvironementVariables();
        void replaceEnvironmentVariables(std::vector<std::string>* statementVector);
        std::vector<std::string> splitString(std::string text, char delimiter);
        void witchCommand(std::string command);
        std::string findCommand(std::string command);
        bool checkExec(std::string path);
        bool handlePipe(std::string statements);
        void executeProgram(std::vector<std::string> * arguments, bool isBackground);
        bool isBackground(std::vector<std::string> * statements);
        void handleProgram(std::vector<std::string> * statements, bool background);
        void removeAmpersand(std::vector<std::string> * statementVector);
        void lim(int cpu, int mem);
        void limPrint();
        void setLim();

        
};


#endif