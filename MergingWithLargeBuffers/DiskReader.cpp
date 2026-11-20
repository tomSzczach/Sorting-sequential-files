#include "DiskReader.h"


DiskReader::DiskReader(std::string inputFile, std::size_t bufferSizeInBytes, std::size_t readFromOffset, std::size_t howManyBytes) :
	_inputFile(inputFile),
	_bufferSize(bufferSizeInBytes), 
	_fromOffset(readFromOffset),
	_currentOffset(readFromOffset),
	_toOffset(readFromOffset + howManyBytes - 1)
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


std::size_t DiskReader::ReadBlock(void* toBuffer)
{
	if (toBuffer == nullptr)
	{
		//std::cout << "[!] DiskReader::ReadBlock - toBuffer == nullptr" << std::endl;
		return 0;
	}

	if (_currentOffset >= _fileSize)
	{
		//std::cout << "[!] DiskReader::ReadBlock - _currentOffset >= _fileSize" << std::endl;
		return 0;
	}

	if (_currentOffset > _toOffset)
	{
		//std::cout << "[!] DiskReader::ReadBlock - _currentOffset > _readToOffset" << std::endl;
		return 0;
	}

	std::size_t bytesToRead = (_currentOffset + _bufferSize > _toOffset)
		? _toOffset - _currentOffset + 1
		: _bufferSize
		;

	std::ifstream inputFile(_inputFile, std::ios::binary);
	if (!inputFile.is_open())
	{
		std::cout << "[!] DiskReader::ReadBlock - Unable to open inputFile file" << std::endl;
		return 0;
	}

	inputFile.seekg(_currentOffset);
	inputFile.read(reinterpret_cast<char*>(toBuffer), bytesToRead);
	inputFile.close();

	_currentOffset += bytesToRead;
	
	return bytesToRead;
}


bool DiskReader::IsSthToRead() const
{
	return (_currentOffset <= _toOffset && _currentOffset < _fileSize) ? true : false;
}
