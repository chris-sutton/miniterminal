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

        void runStatement(std::vector<std::string> * const statements);
        std::vector<std::string>* processStatement(std::string statement);
        void printStatement(std::vector<std::string>::const_iterator * it, std::vector<std::string>::const_iterator * end);
        void replaceEnvironmentVariables(std::vector<std::string>* statementVector);
};


#endif