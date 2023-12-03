#ifndef DiskWriter_H
#define DiskWriter_H


#include <fstream>
#include <iostream>


class DiskWriter
{
private:

	/*
	*	path to tape/file with binary input data
	*/
	const std::string _outputFile;

	/*
	*	size of buffer in bytes
	*/
	const std::size_t _bufferSize;

	/*
	*	limit of bytes to write
	*/
	const std::size_t _bytesToWrite;

	/*
	*	already written bytes
	*/
	std::size_t _writtenBytes;


public:

	DiskWriter(std::string outputFile, std::size_t bufferSizeInBytes, std::size_t bytesToWrite);

	std::size_t WriteBlock(void* fromBuffer);
};

#endif // !DiskWriter_H