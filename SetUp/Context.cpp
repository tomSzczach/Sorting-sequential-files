#include "Context.h"

Context::Context(std::string pathToConfigFile) {

    std::ifstream configFile(pathToConfigFile, std::ios::in);
    if (configFile.is_open())
    {
        configFile << "b = " << b << std::endl;
        configFile << "N = " << N << std::endl;
        configFile << "n = " << n << std::endl;
        configFile << "R = " << R << std::endl;
        configFile << "d = " << d << std::endl;
        configFile << "isManualMode = " << isManualMode << std::endl;
        configFile << "isStepMode = " << isStepMode << std::endl;
        configFile.close();
    }
    else std::cout << "[!] Unable to open 'config.txt' file" << std::endl;

}