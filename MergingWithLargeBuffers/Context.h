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
	*	record type
	*/
	RecordType _recordType;

	/*
	*	R - size of a record in type units
	*/
	unsigned int _recordSizeInType;

	/*
	*	size of a record in bytes
	*/
	unsigned int _recordSizeInBytes;


public:

	Context(std::string pathToConfigFile);


};

#endif // !Context_H