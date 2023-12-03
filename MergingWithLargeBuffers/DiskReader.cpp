#include "DiskReader.h"


DiskReader::DiskReader(std::string inputFile, std::size_t bufferSizeInBytes, std::size_t readFromByte, std::size_t readToByte) :
	_inputFile(inputFile),
	_bufferSize(bufferSizeInBytes), 
	_firstOffset(readFromByte), 
	_currentOffset(readFromByte),
	_lastOffset(readToByte)
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
		std::cout << "[!] DiskReader::ReadBlock - toBuffer == nullptr" << std::endl;
		return 0;
	}

	if (_currentOffset >= _fileSize)
	{
		std::cout << "[!] DiskReader::ReadBlock - _currentOffset >= _fileSize" << std::endl;
		return 0;
	}

	if (_currentOffset >= _lastOffset)
	{
		std::cout << "[!] DiskReader::ReadBlock - _currentOffset >= _readToOffset" << std::endl;
		return 0;
	}

	std::size_t bytesToRead = (_currentOffset + _bufferSize > _lastOffset)
		? _lastOffset - _currentOffset + 1
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
	return (_currentOffset <= _lastOffset && _currentOffset < _fileSize) ? true : false;
}
