#include "Terminal.h"




Terminal::Terminal() {
    venv = {
        {"USER", "cs6t"},
        {"AOSPATH", "/bin:/usr/bin"},
        {"AOSCWD", ""}, // update with cd command
        {"AOSHOME", ""} // originally copy from home n the shell that starts it up
    };
    commands =  {
            {"exit", 0},
            {"envset", 1},
            {"set", 1},
            {"envunset", 2},
            {"unset", 2},
            {"envprt", 3},
            {"prt", 4},
            {"witch", 5},
            {"pwd", 6},
            {"cd", 7}
        };
    userInput = "";
};


void Terminal::runTerminal() {
    std::string userInput = "";
    bool quit = false;
    
    venv["AOSCWD"] = this->getCurrentWorkingDirectory();
    venv["AOSHOME"] = getenv("HOME");

    while (!quit) {

        if (isatty(fileno(stdin))==1){
            std::cout << venv["USER"] << "_> ";
        }
        
        if(std::getline(std::cin, userInput)) {
            if (!userInput.empty()) {
                this->recieveInput(userInput);
            }
            
        } else {
            quit = true;
        }
        userInput = "";
    }
}

std::string Terminal::getCurrentWorkingDirectory() {
    char tempBuffer[PATH_MAX];

    getcwd(tempBuffer, PATH_MAX);
    std::string path(tempBuffer);
    return path;
}

void Terminal::changeCurrentWorkingDirectory(std::string pathName) {
    int success = chdir(pathName.c_str());
    if (success == 0) {
        venv["AOSCWD"] = this->getCurrentWorkingDirectory();
    }
    //std::cout << success << " is the value of success\n";
    //std::cout << errno << " is the value of errno\n";
}

void Terminal::printStatement(std::vector<std::string>::const_iterator it, std::vector<std::string>::const_iterator end) {
    bool endStatement = false;

    while (it != end) {
        endStatement = (((it)+1) == end);
        std::cout << *it << (endStatement ? "" : " ");
        ++(it);
    }
    std::cout << std::endl;
};

void Terminal::recieveInput(std::string const textInput) {

    delete inputArgs;

    inputArgs = this->processStatement(this->removeComments(textInput));
    this->runStatement(inputArgs);
};

void Terminal::setEnvironmentVariable(std::vector<std::string>::const_iterator it, std::vector<std::string>::const_iterator end) {
    std::string key = *it;
    std::string val = "";
    
    ++(it);

    if (it != end)
        val = *it;
    if (val.find("$", 0, 1) == std::string::npos)
        venv[key] = val;
}

void Terminal::unsetEnvironmentVariable(std::vector<std::string>::const_iterator it) {
    venv.erase(*it);
}

void Terminal::printEnvironementVariables() {
    auto begin = venv.begin();
    auto end = venv.end();

    while (begin != end) {
        std::cout << begin->first;
        std::cout << "=";
        std::cout << begin->second;
        std::cout << std::endl;
        ++begin;
    }
}

void Terminal::runStatement(std::vector<std::string> * const statementVector) {
    std::vector<std::string>::const_iterator it = statementVector->begin();
    std::vector<std::string>::const_iterator end = statementVector->end();
    int cmd = -1;
    auto envCmd = commands.find(*it);

    if (envCmd != commands.end()) {
        cmd = envCmd->second;
    }
    
    switch(cmd) {
        case 0:
            exit(0);
            break;
        case 1:
            ++it;
            this->setEnvironmentVariable(it, end);
            break;
        case 2:
            ++it;
            this->unsetEnvironmentVariable(it);
            break;
        case 3:
            this->printEnvironementVariables();
            break;
        case 4:
            ++it;
            this->printStatement(it, end);
            break;
        case 5:
            break;
        case 6:
            std::cout << venv["AOSCWD"] << std::endl;
            break;
        case 7:
            ++it;
            changeCurrentWorkingDirectory(*it);
            break;
        default:
            std::cout << "Invalid command - ";
            this->printStatement(it, end);
            break;
    }
};

std::string Terminal::removeComments(std::string const rawInputText) {
    std::string returnText = "";
    uint commentStartPosition = rawInputText.find_first_of("#");
    if (commentStartPosition != std::string::npos) {
        returnText = rawInputText.substr(0, commentStartPosition);
    }
    return returnText;
}

std::vector<std::string>* Terminal::processStatement(std::string const statement) {
    std::istringstream ss(statement);
    std::string argStr;
    std::vector<std::string>* statementVector = new std::vector<std::string>();
    while (ss >> std::quoted(argStr)) {
        statementVector->push_back(argStr);
    }
    
    this->replaceEnvironmentVariables(statementVector);
    return statementVector;
};

void Terminal::replaceEnvironmentVariables(std::vector<std::string>* statementVector) {
    std::size_t found;
    
    for (auto& it: *statementVector) {

        found = it.find("$",0,1);

        if (found != std::string::npos) {
            it = venv[it.substr(found+1)];
        }
    }
}
