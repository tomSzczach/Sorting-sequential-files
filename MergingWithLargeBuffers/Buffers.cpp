#include "Buffers.h"


// private methods

uint64_t Buffers::BufferIdxFromGlobalRecordIdx(uint64_t ithGlobalRecordIdx)
{
	return ithGlobalRecordIdx / _maxNumberOfRecordsInBuffer;
}

uint64_t Buffers::LocalRecordIdxFromGlobalRecordIdx(uint64_t ithGlobalRecordIdx)
{
	return ithGlobalRecordIdx % _maxNumberOfRecordsInBuffer;
}


// constructors and destructors

Buffers::Buffers(Context& ctx) :
	_maxNumberOfRecordsInBuffer(ctx.NumberOfRecordsInBuffer()),
	_recordSize(ctx.RecordSizeInType()),
	occupiedBuffers(0),
	occupiedRecords(0)
{
	const std::size_t numberOfBuffers = ctx.NumberOfBuffers();

	for (std::size_t ithBufferIdx = 0; ithBufferIdx < numberOfBuffers; ithBufferIdx++)
	{
		_buffers.push_back(Buffer(ctx));
	}
}


// trivial getters and setters

std::size_t Buffers::RecordSize() const
{
	return _recordSize;
}


// main methods

data_t& Buffers::GetRefTo(uint64_t ithGlobalRecordIdx, uint64_t ithValueIdx)
{
	uint64_t ithBufferIdx = BufferIdxFromGlobalRecordIdx(ithGlobalRecordIdx);
	uint64_t ithLocalRecordIdx = LocalRecordIdxFromGlobalRecordIdx(ithGlobalRecordIdx);

	return _buffers[ithBufferIdx].GetRefTo(ithLocalRecordIdx, ithValueIdx);
}


data_t Buffers::GetKeyOf(uint64_t ithGlobalRecordIdx)
{
	uint64_t ithBufferIdx = BufferIdxFromGlobalRecordIdx(ithGlobalRecordIdx);
	uint64_t ithLocalRecordIdx = LocalRecordIdxFromGlobalRecordIdx(ithGlobalRecordIdx);

	return _buffers[ithBufferIdx].GetKeyOf(ithLocalRecordIdx);
}


Buffer& Buffers::GetBuffer(uint64_t ithBufferIdx)
{
	return _buffers[ithBufferIdx];
}


std::size_t Buffers::NumberOfBuffers() const
{
	return _buffers.size();
}


// additional methods

void Buffers::Print()
{
	std::cout << "(" << occupiedBuffers << ") ";

	for (Buffer buffer : _buffers)
	{
		std::cout << "\t";
		buffer.Print();
		std::cout << std::endl;
	}

	std::cout << std::endl;
}


void Buffers::DumpToTXT(std::string filename)
{
	std::ofstream file(filename, std::ios::out | std::ios::app);
	if (!file.is_open()) 
	{
		std::cout << "[!] Buffers::DumpToTXT - Unable to open '" << filename << "' file" << std::endl;
		return;
	}

	for (std::size_t ithBufferIdx = 0; ithBufferIdx < occupiedBuffers; ithBufferIdx++)
	{
		_buffers[ithBufferIdx].DumpToTXT(file);
	}

	file.close();
}
