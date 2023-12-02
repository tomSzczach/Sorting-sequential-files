#include "Quicksort.h"


// private methods

uint64_t Quicksort::Partition(Buffers& buffers, uint64_t leftRecordIdx, uint64_t rightRecordIdx)
{
	data_t pivot = buffers.GetKeyOf((leftRecordIdx + rightRecordIdx) / 2);

	while (true)
	{
		while (buffers.GetKeyOf(leftRecordIdx) < pivot)
			++leftRecordIdx;

		while (buffers.GetKeyOf(rightRecordIdx) > pivot)
			--rightRecordIdx;

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

void Quicksort::Sort(Buffers& buffers, uint64_t leftRecordIdx, uint64_t rightRecordIdx)
{
	if (leftRecordIdx < rightRecordIdx)
	{
		uint64_t dividingRecordIdx = Partition(buffers, leftRecordIdx, rightRecordIdx);
		Sort(buffers, leftRecordIdx, dividingRecordIdx);
		Sort(buffers, dividingRecordIdx + 1, rightRecordIdx);
	}
}
