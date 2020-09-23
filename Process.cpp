#include "Process.h"
// Constructors
Process::Process(std::vector<std::string> atomicStatement, bool builtin, bool background)
    : atomicStatement(atomicStatement), builtin(builtin), background(background)
    {}
Process::Process(std::vector<std::string> atomicStatement, bool builtin)
    : Process(atomicStatement, builtin, false)
    {}
Process::Process(std::vector<std::string> atomicStatement)
    : Process(atomicStatement, false, false)
    {}
Process::Process()
    :Process(std::vector<std::string> {} ,false, false)
    {}

// Functions
