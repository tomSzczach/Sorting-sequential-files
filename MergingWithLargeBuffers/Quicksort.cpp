#include "Quicksort.h"


// private methods

uint64_t Quicksort::Partition(Buffers& buffers, uint64_t leftRecordIdx, uint64_t rightRecordIdx)
{
	data_t pivot = buffers.GetKeyOf((leftRecordIdx + rightRecordIdx) / 2);

	--leftRecordIdx;
	++rightRecordIdx;

	while (true)
	{
		while (buffers.GetKeyOf(++leftRecordIdx) < pivot);

		while (buffers.GetKeyOf(--rightRecordIdx) > pivot);

		if (leftRecordIdx < rightRecordIdx)
			SwapRecord(buffers, leftRecordIdx, rightRecordIdx);
		else
			return rightRecordIdx;
	}
}


void Quicksort::SwapRecord(Buffers& buffers, uint64_t leftRecordIdx, uint64_t rightRecordIdx)
{
	const std::size_t recordSize = buffers.RecordSize();

	for (std::size_t recordValueIdx = 0; recordValueIdx < recordSize; recordValueIdx++)
	{
		std::swap(buffers.GetRefTo(leftRecordIdx, recordValueIdx), buffers.GetRefTo(rightRecordIdx, recordValueIdx));
	}		
}


// public methods

void Quicksort::Sort(Buffers& buffers, uint64_t leftRecordIdx, uint64_t rightRecordIdx, int i)
{
	if (leftRecordIdx < rightRecordIdx)
	{
		uint64_t dividingRecordIdx = Partition(buffers, leftRecordIdx, rightRecordIdx);
		Sort(buffers, leftRecordIdx, dividingRecordIdx, i+1);
		Sort(buffers, dividingRecordIdx + 1, rightRecordIdx, i+1);
	}
}
