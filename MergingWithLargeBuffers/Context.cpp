#include "Context.h"


Context::Context(std::string pathToConfigFile)
    : _recordType(RECORD_TYPE_INT),
    _numberOfAllRecords(0),
    _numberOfRecordsInBuffer(0),
    _numberOfBuffers(0),
    _bufferSizeInBytes(0),
    _allBuffersSizeInBytes(0),
    _recordSizeInType(0),
    _recordSizeInBytes(0),
    _isConfigFileRead(false)
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
            _recordSizeInType = std::stoull(value);
        }
    }
    configFile.close();
    _isConfigFileRead = true;

    if (_recordType == RECORD_TYPE_INT)
    {
        _recordSizeInBytes = _recordSizeInType * sizeof(int);
    }
}


bool Context::IsConfigFileReadSuccessfully() const
{
    return _isConfigFileRead;
}

std::size_t Context::RecordSizeInBytes() const
{
    return _recordSizeInBytes;
}

std::size_t Context::RecordSizeInType() const
{
    return _recordSizeInType;
}


std::size_t Context::BufferSizeInBytes() const
{
    return _numberOfRecordsInBuffer * _recordSizeInBytes;
}


std::size_t Context::BufferSizeInType() const
{
    return _numberOfRecordsInBuffer * _recordSizeInType;
}


std::size_t Context::AllBuffersSizeInBytes() const
{
    return _numberOfBuffers * BufferSizeInBytes();
}


std::size_t Context::AllBuffersSizeInType() const
{
    return _numberOfBuffers * BufferSizeInType();
}

std::size_t Context::AllRecordsSizeInBytes() const
{
    return _numberOfAllRecords * _recordSizeInBytes;
}


unsigned int Context::NumberOfRecords() const
{
    return _numberOfAllRecords;
}


unsigned int Context::NumberOfBuffers() const
{
    return _numberOfBuffers;
}

unsigned int Context::NumberOfRecordsInBuffer() const
{
    return _numberOfRecordsInBuffer;
}

