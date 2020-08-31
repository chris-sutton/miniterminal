#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <iomanip>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <map>



class Terminal {
    public:
        // Constructors
        Terminal();
        // Public variables
        std::string userInput;
        std::map<std::string, std::string> venv;
        std::vector<std::string> *inputArgs;
        // Public functions
        void recieveInput(std::string textInput);

    private:
        std::vector<std::vector<std::string>> inputStatements;
        std::map<std::string, int> commands;

        void runStatements(std::vector<std::vector<std::string>> statements);
        std::vector<std::string>* processStatement(std::string statement);
        void printStatement(const std::vector<std::string>& text);
        void replaceEnvironmentVariables(std::vector<std::string>* statementVector);
};


#endif