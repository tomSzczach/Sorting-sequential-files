#ifndef DiskReader_H
#define DiskReader_H


#include <fstream>
#include <iostream>


class DiskReader
{
private:

	/*
	*	path to tape/file with binary input data
	*/
	const std::string _inputFile;

	/*
	*	size of buffer in bytes
	*/
	const std::size_t _bufferSize;

	/*
	*	first position to read in _inputFile
	*/
	unsigned int _readFromOffset;

	/*
	*	size of _inputFile in bytes
	*/
	std::size_t _fileSize;


public:

	DiskReader(std::string inputFile, std::size_t bufferSizeInBytes);

	bool ReadBlock(void* toBuffer);

	bool IsSthToRead() const;
};

#endif // !DiskReader_H