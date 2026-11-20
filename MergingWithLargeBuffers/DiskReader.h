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
	*	first byte to read
	*/
	const std::size_t _fromOffset;

	/*
	*	last byte to read
	*/
	const std::size_t _toOffset;

	/*
	*	next byte to read
	*/
	std::size_t _currentOffset;
	 
	/*
	*	size of _inputFile in bytes
	*/
	std::size_t _fileSize;


public:

	// < readFromOffset , readFromOffset + howManyBytes )
	DiskReader(std::string inputFile, std::size_t bufferSizeInBytes, std::size_t readFromOffset, std::size_t howManyBytes);

	std::size_t ReadBlock(void* toBuffer);

	bool IsSthToRead() const;
};

#endif // !DiskReader_H