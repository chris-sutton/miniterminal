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
            {"cd", 7},
            {"lim", 8}
        };
    userInput = "";
    
};
// void Terminal::lim(int cpu=0, int mem=0) {
//     int i;
//     struct rlimit rl;
//     if (cpu==0 || mem==0)
//         return;
//     i = getrlimit(RLIMIT_CPU, &rl);
//     rl.rlim_cur = cpu;
//     rl.rlim_max = cpu;
//     i = setrlimit(RLIMIT_CPU, &rl);
//     i = getrlimit(RLIMIT_AS, &rl);
//     rl.rlim_cur = mem;
//     rl.rlim_max = mem;
//     i = setrlimit(RLIMIT_AS, &rl);
// }
void Terminal::runTerminal(std::string argument) {
    std::string userInput = "";
    bool quit = false;
    std::istream *shellIn;
    std::ifstream fileIn;
    if (!argument.empty()) {
        fileIn.open(argument);
        shellIn = &fileIn;
    } else {
        shellIn = &std::cin;
    }

    venv["AOSCWD"] = this->getCurrentWorkingDirectory();
    venv["AOSHOME"] = getenv("HOME");
    std::string prompt;

    while (!quit) {
        
        if (isatty(fileno(stdin))==1 && argument.empty()){
            std::cout << venv["USER"] << "_sh> ";
        
        }
        if(std::getline(*shellIn, userInput)) {
            if (!userInput.empty()) {
                if (!this->handlePipe(userInput)) {
                    this->recieveInput(userInput, 0);
                }
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

void Terminal::recieveInput(std::string const textInput, int isPipe) {

    delete inputArgs;

    inputArgs = this->processStatement(this->removeComments(textInput));
    this->runStatement(inputArgs, isPipe);
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

void Terminal::runStatement(std::vector<std::string> * statementVector, int isPipe) {
    std::vector<std::string>::const_iterator it = statementVector->begin();
    std::vector<std::string>::const_iterator end = statementVector->end();
    std::vector<std::string>::const_iterator found;

    bool bg = this->isBackground(statementVector);
    int cmd = -1;
    auto envCmd = commands.find(*it);
    
    std::string pathExec = "";
    this->removeAmpersand(statementVector);
    if (envCmd != commands.end()) {
        cmd = envCmd->second;
    } else if (!this->findCommand(*it).empty()) {
        //found in aospath
        pathExec = this->findCommand(*it);
        if (this->checkExec(pathExec)) {
            cmd = 99;
            cmd += isPipe;
        }
    } else if (this->checkExec(*it)) {
        pathExec = *it;
        cmd = 99; // don't forget this is how you were going to execute with fork exec
        cmd += isPipe;
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
            ++it;
            this->witchCommand(*it);
            break;
        case 6:
            std::cout << venv["AOSCWD"] << std::endl;
            break;
        case 7:
            ++it;
            changeCurrentWorkingDirectory(*it);
            break;
        case 8:
            break;
        case 99:
            this->handleProgram(statementVector, bg);
            break;
        case 100:
            this->executeProgram(statementVector, false);
            break;
        default:
            std::cout << "Invalid command - ";
            this->printStatement(it, end);
            break;
    }
    ++it;

};
void Terminal::removeAmpersand(std::vector<std::string> * statementVector) {
    std::vector<std::string>::iterator it = statementVector->begin();
    std::vector<std::string>::iterator end = statementVector->end();

    while (it != end) {
        size_t pos = it->find('&');

        if (pos != std::string::npos) {
            if (it->empty()) {
                statementVector->erase(it);
            }
        }
        it->erase(std::remove(it->begin(), it->end(), '&'), it->end());
        ++it;
    }
}
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

std::vector<std::string> Terminal::splitString(std::string text, char delimiter) {
    std::vector<std::string> paths;
    std::string temp;
    std::stringstream ss(text);

    while(std::getline(ss, temp, delimiter)) {
        paths.push_back(temp);
    }
    return paths;
}

void Terminal::witchCommand(std::string command) {
    std::cout << this->findCommand(command) << std::endl;
    if (this->checkExec(command))
        std::cout << command << " was found with access(2)\n";
}

bool Terminal::checkExec(std::string path) {
    if(access(path.c_str(), X_OK) == 0) {
        return true;
    }
    return false;
}

std::string Terminal::findCommand(std::string command) {
    std::string notFound = "";
    std::vector<std::string> paths = this->splitString(venv["AOSPATH"], ':');
    std::vector<std::string>::iterator it = paths.begin();
    std::vector<std::string>::iterator end = paths.end();
    DIR *dir;
    struct dirent *dirp;

    while (it != end) {
        const char * directory = &(*it->c_str());
        const char * filename;
        const char * cmd = command.c_str();

        if ((dir = opendir(directory)) != NULL) {
            
                while((dirp = readdir(dir)) != NULL) {
                    filename = dirp->d_name;
                    //std::cout << filename << " " << cmd << std::endl;
                    if (std::strcmp(filename, cmd) == 0) {
                        std::string temp;
                        temp = directory;
                        temp += "/";
                        temp += cmd;
                        if (this->checkExec(temp)) {
                            return temp;
                        }
                    }
                }
        }
        ++it;
    }
    return notFound;
}

bool Terminal::handlePipe(std::string statements) {
    
    std::vector<std::string> pipeStatements = this->splitString(this->removeComments(statements), '|');
    int pipefd[2];
    pipe(pipefd);
    int pids[2];
    int status[2];
    if (pipeStatements.size() == 2) {
        pids[0] = fork();
        if (pids[0] == 0) {
            close(pipefd[0]);
            dup2(pipefd[1], 1);
            this->recieveInput(pipeStatements[0], 1);
            exit(0);
        } else {
            pids[1] = fork();
            if (pids[1] == 0) {
                close(pipefd[1]);
                dup2(pipefd[0], 0);
                this->recieveInput(pipeStatements[1], 1);
                exit(0);
            }

        }
        close(pipefd[0]);
        close(pipefd[1]);
        
        status[0] = wait(&pids[0]);
        status[1] = wait(&pids[1]);

    } else {
        return false;
    }
    return true;
}
bool Terminal::isBackground(std::vector<std::string> * statements) {
    std::vector<std::string>::iterator it = statements->begin();
    std::vector<std::string>::iterator end = statements->end();
    std::string whitespaces (" \t\f\v\n\r");

    while (it != end) {
        size_t pos = it->find_last_not_of(whitespaces);
            if (pos != std::string::npos) {
                char c = it->at(pos);

                if (c == '&' && (it+1) == end) {
                    return true;
                }
            }
        ++it;
    }
    return false;
}
void Terminal::handleProgram(std::vector<std::string> * statements, bool background) {
    
    int pid = fork();
    if (pid == 0) {
        this->executeProgram(statements, background);
        exit(0);
    }
    if (!background) {
        int status = wait(&pid);
    }
}
void Terminal::executeProgram(std::vector<std::string> * arguments, bool background) {
    std::vector<std::string>::iterator it = arguments->begin();
    std::vector<std::string>::iterator end = arguments->end();
    
    std::vector<char *> commandArgs;

    while (it < end) {
        if (!it->empty()) {
            commandArgs.push_back(const_cast<char*>(it->c_str()));
        }
        ++it;
    }
    char **args = commandArgs.data();
    execvp(args[0], &args[0]);
}