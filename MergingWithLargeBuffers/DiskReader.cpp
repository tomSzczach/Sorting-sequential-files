#include "DiskReader.h"


DiskReader::DiskReader(std::string inputFile, std::size_t bufferSizeInBytes)
	: _inputFile(inputFile), _bufferSize(bufferSizeInBytes), _readFromOffset(0)
{
	std::ifstream file(_inputFile, std::ios::binary);
	if (!file.is_open()) 
	{
		_fileSize = 0;
	}
	else
	{
		file.seekg(0, file.end);
		_fileSize = file.tellg();
		file.close();
	}
}


bool DiskReader::ReadBlock(void* toBuffer)
{
	if (toBuffer == nullptr) 
	{
		std::cout << "[!] DiskReader::ReadBlock - toBuffer == nullptr" << std::endl;
		return false;
	}

	if (_readFromOffset >= _fileSize)
	{
		std::cout << "[!] DiskReader::ReadBlock - _readFromOffset >= _fileSize" << std::endl;
		return false;
	}

	std::ifstream inputFile(_inputFile, std::ios::binary);
	if (!inputFile.is_open())
	{
		std::cout << "[!] DiskReader::ReadBlock - Unable to open inputFile file" << std::endl;
		return false;
	}

	inputFile.seekg(_readFromOffset);
	inputFile.read(reinterpret_cast<char*>(toBuffer), _bufferSize);
	_readFromOffset += _bufferSize;

	inputFile.close();

	return true;
}


bool DiskReader::IsSthToRead() const
{
	return (_readFromOffset < _fileSize) ? true : false;
}
