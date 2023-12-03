#ifndef Buffers_H
#define Buffers_H


#include <vector>
#include <iostream>

#include "Context.h"
#include "Buffer.h"


class Buffers
{
private:

	/*
	*	array of Buffer objects
	*/
	std::vector<Buffer> _buffers;

	/*
	*	b - maximum possible numer of records in buffer
	*/
	const std::size_t _maxNumberOfRecordsInBuffer;

	/*
	*	R - length of record (in data_t unit)
	*/
	const std::size_t _recordSize;


	// private methods

	uint64_t BufferIdxFromGlobalRecordIdx(uint64_t ithGlobalRecordIdx);

	uint64_t LocalRecordIdxFromGlobalRecordIdx(uint64_t ithGlobalRecordIdx);

public:

	/*
	*	number of buffers storing valid data
	*/
	std::size_t occupiedBuffers;

	/*
	*	number of records storing in all buffers
	*/
	std::size_t occupiedRecords;


	// constructors and destructors

	Buffers(Context& ctx);


	// trivial getters and setters

	std::size_t RecordSize() const;


	// main methods

	data_t& GetRefTo(uint64_t ithGlobalRecordIdx, uint64_t ithValueIdx);

	data_t GetKeyOf(uint64_t ithGlobalRecordIdx);

	Buffer& GetBuffer(uint64_t ithBufferIdx);

	std::size_t NumberOfBuffers() const;


	// additional methods

	void Print();

	void DumpToTXT(std::string filename);

};

#endif // !Buffers_H