#ifndef Buffer_H
#define Buffer_H


#include <vector>
#include <iostream>

#include "Utils.h"
#include "Context.h"


class Buffer
{
private:

	/*
	*	buffer as an data_t dynamic array on heap, but with constant length
	*/
	std::vector<data_t> _buffer;

	/*
	*	R - length of record (in data_t unit)
	*/
	const std::size_t _recordSize;

	/*
	*	current number of non-empty records stored in buffer
	*/
	std::size_t _numberOfOccupiedRecords;

	/*
	*	b - maximum possible numer of records in buffer
	*/
	const std::size_t _maxNumberOfRecords;


public:

	// constructors and destructors

	Buffer();

	Buffer(Context& ctx);


	// trivial getters and setters

	std::size_t NumberOfOccupiedRecords() const;

	std::size_t MaxNumberOfRecords() const;


	// main methods

	data_t& GetRefTo(uint64_t ithRecord, uint64_t ithValue);
	
	data_t GetKeyOf(uint64_t ithRecord);

	data_t* GetPointer();


	// additional methods

	void Print();

	void DumpToTXT(std::ofstream& fileTXT);

};

#endif // !Buffer_H