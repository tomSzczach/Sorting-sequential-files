#include "Buffer.h"


// constructors and destructors

Buffer::Buffer() :
	_recordSize(0),
	occupiedRecords(0),
	_maxNumberOfRecords(0)
{}


Buffer::Buffer(Context& ctx) :
	_recordSize(ctx.RecordSizeInType()),
	occupiedRecords(0),
	_maxNumberOfRecords(ctx.NumberOfRecordsInBuffer())
{
	_buffer = std::vector<data_t>(_recordSize * _maxNumberOfRecords);
}


// trivial getters and setters

std::size_t Buffer::MaxNumberOfRecords() const
{
	return _maxNumberOfRecords;
}


// main methods

data_t& Buffer::GetRefTo(uint64_t ithRecord, uint64_t ithValue)
{
	return _buffer[ithRecord * _recordSize + ithValue];
}


data_t Buffer::GetKeyOf(uint64_t ithRecord)
{
	return GetRefTo(ithRecord, 0);
}


data_t* Buffer::GetPointer()
{
	return _buffer.data();
}


// additional methods

void Buffer::Print()
{
	for (int value : _buffer)
		std::cout << value << " ";
}


void Buffer::DumpToTXT(std::ofstream& fileTXT)
{
	const std::size_t valuesToDump = occupiedRecords * _recordSize;

	for (std::size_t ithValueIdx = 0; ithValueIdx < valuesToDump; ithValueIdx++)
	{
		fileTXT << _buffer[ithValueIdx] << " ";
		if (ithValueIdx % _recordSize == 0) fileTXT << "| ";
		if ((ithValueIdx + 1) % _recordSize == 0) fileTXT << std::endl;
	}
}
