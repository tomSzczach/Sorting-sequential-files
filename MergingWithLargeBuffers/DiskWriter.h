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


public:

	DiskWriter(std::string outputFile, std::size_t bufferSizeInBytes);

	bool WriteBlock(void* fromBuffer);
};

#endif // !DiskWriter_H