#ifndef Context_H
#define Context_H


#include <string>
#include <iostream>
#include <fstream>


/*
*	describes what type the record is built from
*/
enum RecordType {
	RECORD_TYPE_INT
};


/*
*	keeps information about configuration
*/
class Context
{
private:

	/*
	*	N - number of all records
	*/
	unsigned int _numberOfAllRecords;

	/*
	*	b - number of records in (buffer / disk page), blocking factor
	*/
	unsigned int _numberOfRecordsInBuffer;

	/*
	*	n - number of buffers
	*/
	unsigned int _numberOfBuffers;

	/*
	*	size of each buffer in bytes
	*/
	std::size_t _bufferSizeInBytes;

	/*
	*	size of all buffers summed up
	*/
	std::size_t _allBuffersSizeInBytes;

	/*
	*	record type
	*/
	RecordType _recordType;

	/*
	*	R - size of a record in type units
	*/
	std::size_t _recordSizeInType;

	/*
	*	size of a record in bytes
	*/
	std::size_t _recordSizeInBytes;

	/*
	*	is config file read successfully?
	*/
	bool _isConfigFileRead;


public:

	Context(std::string pathToConfigFile);

	bool IsConfigFileReadSuccessfully() const;

	std::size_t RecordSizeInBytes() const;

	std::size_t RecordSizeInType() const;

	std::size_t BufferSizeInBytes() const;

	std::size_t BufferSizeInType() const;

	std::size_t AllBuffersSizeInBytes() const;

	std::size_t AllBuffersSizeInType() const;

	std::size_t AllRecordsSizeInBytes() const;

	unsigned int NumberOfRecords() const;

	unsigned int NumberOfBuffers() const;

	unsigned int NumberOfRecordsInBuffer() const;

};

#endif // !Context_H