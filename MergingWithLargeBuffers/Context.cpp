#include "Context.h"

Context::Context(std::string pathToConfigFile)
    : _recordType(RECORD_TYPE_INT),
    _numberOfAllRecords(0),
    _numberOfRecordsInBuffer(0),
    _numberOfBuffers(0),
    _recordSizeInType(0),
    _recordSizeInBytes(0)
{
    std::ifstream configFile(pathToConfigFile, std::ios::in);

    if (!configFile.is_open())
    {
        std::cout << "[!] Unable to open 'config.txt' file" << std::endl;
        return;
    }

    std::string line, key, value;
    std::size_t delimiterPos;
    while (std::getline(configFile, line))
    {
        delimiterPos = line.find(" = ");
        key = line.substr(0, delimiterPos);
        value = line.substr(delimiterPos + 3);

        if (key == "N")
        {
            _numberOfAllRecords = std::stoul(value);
        }
        else if (key == "b") 
        {
            _numberOfRecordsInBuffer = std::stoul(value);
        }
        else if (key == "n") 
        {
            _numberOfBuffers = std::stoul(value);
        }
        else if (key == "R") 
        {
            _recordSizeInType = std::stoul(value);
        }
    }
    configFile.close();

    if (_recordType == RECORD_TYPE_INT)
    {
        _recordSizeInBytes = _recordSizeInType * sizeof(int);
    }
}