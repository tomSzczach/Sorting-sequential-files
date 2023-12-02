#ifndef Quicksort_H
#define Quicksort_H


#include "Utils.h"
#include "Buffers.h"


class Quicksort
{
private:

	static uint64_t Partition(Buffers& buffers, uint64_t firstRecordIdx, uint64_t lastRecordIdx);

	static void SwapRecord(Buffers& buffers, uint64_t leftRecordIdx, uint64_t rightRecordIdx);


public:

	static void Sort(Buffers& buffers, uint64_t firstRecordIdx, uint64_t lastRecordIdx);

};

#endif // !Quicksort_H
