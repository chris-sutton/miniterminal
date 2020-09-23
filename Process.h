#ifndef _PROCESS_H
#define _PROCESS_H

#include <string>
#include <vector>

class Process {
    public:
        // Constructors
        Process(std::vector<std::string> atomicStatement, bool builtin, bool background);
        Process(std::vector<std::string> atomicStatement, bool builtin);
        Process(std::vector<std::string> atomicStatement);
        Process();
        // Public variables
        std::vector<std::string> atomicStatement;
        bool background;
        bool builtin;
        // Public functions
        
    private:
};


#endif