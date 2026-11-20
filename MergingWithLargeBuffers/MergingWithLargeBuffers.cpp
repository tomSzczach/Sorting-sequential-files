
/* TODO

>> Stage 1

    [X] 1.1. read (at most) n*b records from tape.bin to n bufferes, b records each
    [X] 1.2. sort (ascending) all read records using QuickSort to create n*b-records-length run
    [X] 1.3. write the run to file
    [X] 1.4. if EOF tabe.bin go to Stage 2, else go to step 1.

>> Stage 2

    [X] 2.1. get subsequent at most n-1 runs from file
    [X] 2.2. merge these runs based on algorithm from "14.6.2 Multiway Merging", saving the result of merging to file
            (source: https://opendsa-server.cs.vt.edu/ODSA/Books/Everything/html/ExternalSort.html)
    [X] 2.3. if all runs from file are sorted go to 2.4., else go to 2.1.
    [X] 2.4. if only one run remained go to 2.5., else go to 2.1.
    [X] 2.5. stop

*/


#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <ctime>

#include "Utils.h"
#include "Context.h"
#include "DiskReader.h"
#include "DiskWriter.h"
#include "Buffers.h"
#include "Buffer.h"
#include "Quicksort.h"


uint64_t NUM_OF_READS = 0;
uint64_t NUM_OF_WRITES = 0;
uint64_t NUM_OF_CYCLES_DURING_STAGE_2 = 0;


struct heap_data_t {
    data_t key;
    uint64_t bufferIdx;
    uint64_t recordIdx;

    heap_data_t(data_t _key, uint64_t _bufferIdx, uint64_t _recordIdx) : key(_key), bufferIdx(_bufferIdx), recordIdx(_recordIdx) {}

    bool operator<(const heap_data_t& d) const {
        return key > d.key;
    }
};


void CopyRecord(Buffers& buffers, uint64_t fromBufferIdx, uint64_t fromRecordIdx, uint64_t toBufferIdx, uint64_t toRecordIdx)
{
    const std::size_t recordSize = buffers.RecordSize();

    for (std::size_t recordValueIdx = 0; recordValueIdx < recordSize; recordValueIdx++)
    {
        data_t value = buffers.GetBuffer(fromBufferIdx).GetRefTo(fromRecordIdx, recordValueIdx);
        data_t& dest = buffers.GetBuffer(toBufferIdx).GetRefTo(toRecordIdx, recordValueIdx);
        dest = value;
    }

    buffers.GetBuffer(toBufferIdx).occupiedRecords++;
}

void PrintHeap(std::vector<heap_data_t> heapVector)
{
    for (heap_data_t entry : heapVector)
        std::cout << " key = " << entry.key << " pos = " << entry.bufferIdx << "+" << entry.recordIdx << std::endl;
}


uint64_t StageOne(Context& ctx, Buffers& buffers)
{
    DiskReader tapeReader("tape.bin", ctx.BufferSizeInBytes(), 0, ctx.AllRecordsSizeInBytes());
    DiskWriter tapeWriter("beforeStage_2_0.bin", ctx.BufferSizeInBytes(), ctx.AllRecordsSizeInBytes());

    uint64_t numberOfRuns = 0;

    while (tapeReader.IsSthToRead())
    {
        // read at most n*b records
        for (uint64_t ithBufferIdx = 0; ithBufferIdx < buffers.NumberOfBuffers(); ithBufferIdx++)
        {
            Buffer& toBuffer = buffers.GetBuffer(ithBufferIdx);

            std::size_t readBytes = tapeReader.ReadBlock((void*)toBuffer.GetPointer());

            if (readBytes > 0)
            {
                NUM_OF_READS++;

                std::size_t readRecords = readBytes / ctx.RecordSizeInBytes();

                buffers.occupiedBuffers += 1;
                buffers.occupiedRecords += readRecords;
                toBuffer.occupiedRecords = readRecords;
            }
            else
            {
                //std::cout << "[!] StageOne - read 0 bytes" << std::endl;
            }       
        }

        // sort records
        Quicksort::Sort(buffers, 0, buffers.occupiedRecords - 1, 0);

        // print check
        buffers.DumpToTXT("afterStageOne.txt");

        // write records
        std::size_t buffersToWrite = buffers.occupiedBuffers;
        for (uint64_t ithBufferIdx = 0; ithBufferIdx < buffersToWrite; ithBufferIdx++)
        {
            Buffer& fromBuffer = buffers.GetBuffer(ithBufferIdx);

            std::size_t writtenBytes = tapeWriter.WriteBlock((void*)fromBuffer.GetPointer());

            if (writtenBytes > 0)
            {
                NUM_OF_WRITES++;

                std::size_t writtenRecords = writtenBytes / ctx.RecordSizeInBytes();

                buffers.occupiedBuffers -= 1;
                buffers.occupiedRecords -= writtenRecords;
                fromBuffer.occupiedRecords -= writtenRecords;
            }
            else
            {
                //std::cout << "[!] StageOne - write 0 bytes" << std::endl;
            }
        }

        numberOfRuns++;
    }

    return numberOfRuns;
}


void StageTwo(Context& ctx, Buffers& buffers, uint64_t numberOfRunsAfterStageOne)
{
    uint64_t numberOfRuns = numberOfRunsAfterStageOne;
    uint64_t doneIterations = 0;
    const uint64_t outputBufferIdx = buffers.NumberOfBuffers() - 1;

    // max run length after stage 1
    std::size_t maxRunLengthInRecords = ctx.NumberOfRecordsInBuffer() * ctx.NumberOfBuffers();
    std::size_t maxRunLengthInBytes = maxRunLengthInRecords * ctx.RecordSizeInBytes();

    while (numberOfRuns > 1)
    {
        // each run gives own reader
        std::vector<DiskReader> readers;
        for (std::size_t ithReader = 0; ithReader < numberOfRuns; ithReader++)
        {
            std::size_t readFromOffset = ithReader * maxRunLengthInBytes;
            std::size_t howManyBytes = std::min(maxRunLengthInBytes, ctx.AllRecordsSizeInBytes() - readFromOffset);

            std::string filename;
            filename.append("beforeStage_2_").append(std::to_string(doneIterations)).append(".bin");
            readers.push_back(DiskReader(filename, ctx.BufferSizeInBytes(), readFromOffset, howManyBytes)); 
        }

        // all runs gives the same writer
        std::string filename;
        filename.append("beforeStage_2_").append(std::to_string(doneIterations+1)).append(".bin");
        DiskWriter writer(filename, ctx.BufferSizeInBytes(), ctx.AllRecordsSizeInBytes());

        // heap vector
        std::vector<heap_data_t> heapVector;
        
        uint64_t processedRuns = 0;
        
        while (processedRuns < numberOfRuns)
        {
            // read b records from max n-1 subsequent runs
            for (uint64_t runIdx = processedRuns, ithBufferIdx = 0; (ithBufferIdx < buffers.NumberOfBuffers() - 1) && (runIdx < numberOfRuns); runIdx++, ithBufferIdx++)
            {
                Buffer& toBuffer = buffers.GetBuffer(ithBufferIdx);
                std::size_t readBytes = readers.at(runIdx).ReadBlock(toBuffer.GetPointer());

                if (readBytes > 0)
                {
                    NUM_OF_READS++;

                    std::size_t readRecords = readBytes / ctx.RecordSizeInBytes();

                    toBuffer.occupiedRecords = readRecords;

                    heapVector.push_back(heap_data_t(toBuffer.GetKeyOf(0), ithBufferIdx, 0));
                }
                else
                {
                    //std::cout << "[!] StageTwo - read 0 bytes" << std::endl;
                }
            }
            uint64_t processingRuns = heapVector.size();

            // heapify
            std::make_heap(heapVector.begin(), heapVector.end());

            do {

                // copy smallest record to output buffer
                CopyRecord(buffers, heapVector.front().bufferIdx, heapVector.front().recordIdx, outputBufferIdx, buffers.GetBuffer(outputBufferIdx).occupiedRecords);

                // if output buffer is full, write it to output file
                if (buffers.GetBuffer(outputBufferIdx).occupiedRecords == ctx.NumberOfRecordsInBuffer())
                {
                    Buffer& fromBuffer = buffers.GetBuffer(outputBufferIdx);

                    std::size_t writtenBytes = writer.WriteBlock((void*)fromBuffer.GetPointer());

                    // print check
                    std::string filename;
                    filename.append("afterStage_2_").append(std::to_string(doneIterations)).append(".txt");
                    std::ofstream file(filename, std::ios::out | std::ios::app);
                    if (file.is_open()) {
                        fromBuffer.DumpToTXT(file);
                        file.close();                        
                    }

                    if (writtenBytes > 0)
                    {
                        NUM_OF_WRITES++;

                        std::size_t writtenRecords = writtenBytes / ctx.RecordSizeInBytes();
                        fromBuffer.occupiedRecords -= writtenRecords;
                    }
                    else
                    {
                        //std::cout << "[!] StageOne - write 0 bytes" << std::endl;
                    }
                }

                // get rid of the smallest from heap
                std::pop_heap(heapVector.begin(), heapVector.end());

                // if are next records in buffer with that smallest
                if (heapVector.back().recordIdx + 1 < buffers.GetBuffer(heapVector.back().bufferIdx).occupiedRecords)
                {
                    // apply next record from that buffer to heap
                    heapVector.back().recordIdx++;
                    heapVector.back().key = buffers.GetBuffer(heapVector.back().bufferIdx).GetKeyOf(heapVector.back().recordIdx);
                    std::push_heap(heapVector.begin(), heapVector.end());
                }
                // if not, then read block to that buffer from assigned run
                else
                {
                    // read next block to buffer from assigned run
                    uint64_t bufferIdx = heapVector.back().bufferIdx;
                    Buffer& toBuffer = buffers.GetBuffer(bufferIdx);
                    std::size_t readBytes = readers.at(processedRuns + bufferIdx).ReadBlock(toBuffer.GetPointer());

                    // if read successfully
                    if (readBytes > 0)
                    {
                        NUM_OF_READS++;

                        // apply first record to heap
                        std::size_t readRecords = readBytes / ctx.RecordSizeInBytes();
                        toBuffer.occupiedRecords = readRecords;

                        heapVector.back().recordIdx = 0;
                        heapVector.back().key = toBuffer.GetKeyOf(0);
                        std::push_heap(heapVector.begin(), heapVector.end());
                    }
                    // if not, it means that run has ended
                    else
                    {
                        // end of run, buffer assigned with that run is not nedded anymore on the heap
                        heapVector.pop_back();
                    }
                }

            } while (!heapVector.empty());

            // write remained records from output buffer if there are any, write it to output file
            if (buffers.GetBuffer(outputBufferIdx).occupiedRecords > 0)
            {
                Buffer& fromBuffer = buffers.GetBuffer(outputBufferIdx);

                std::size_t writtenBytes = writer.WriteBlock((void*)fromBuffer.GetPointer());

                // print check
                std::string filename;
                filename.append("afterStage_2_").append(std::to_string(doneIterations)).append(".txt");
                std::ofstream file(filename, std::ios::out | std::ios::app);
                if (file.is_open()) {
                    fromBuffer.DumpToTXT(file);
                    file.close();
                }

                if (writtenBytes > 0)
                { 
                    NUM_OF_WRITES++;

                    std::size_t writtenRecords = writtenBytes / ctx.RecordSizeInBytes();
                    fromBuffer.occupiedRecords -= writtenRecords;
                }
                else
                {
                    //std::cout << "[!] StageOne - write 0 bytes" << std::endl;
                }
            }

            processedRuns += processingRuns;
        }

        // delete all readers
        readers.clear();

        // max run length after ith iteration of stage 2
        maxRunLengthInRecords = maxRunLengthInRecords * (ctx.NumberOfBuffers() - 1);
        maxRunLengthInBytes = maxRunLengthInRecords * ctx.RecordSizeInBytes();

        // number of runs after ith iteration of stage 2
        numberOfRuns = (std::size_t)ceil((long double)ctx.NumberOfRecords() / (long double)maxRunLengthInRecords);

        doneIterations++;
    }

    NUM_OF_CYCLES_DURING_STAGE_2 = doneIterations;
}


int main()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();


    Context ctx("config.txt");
    if (ctx.IsConfigFileReadSuccessfully() == false) return 1;

    Buffers buffers(ctx);
    // check if buffers good

    uint64_t numberOfRuns = StageOne(ctx, buffers);
    StageTwo(ctx, buffers, numberOfRuns);


    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;


    std::cout << " - - - - " << std::endl;
    std::cout << "NUM_OF_READS = " << NUM_OF_READS << std::endl;
    std::cout << "NUM_OF_WRITES = " << NUM_OF_WRITES << std::endl;
    std::cout << "NUM_OF_I/O_OPERATIONS = " << NUM_OF_READS + NUM_OF_WRITES << std::endl;
    std::cout << "NUM_OF_CYCLES_DURING_STAGE_2 = " << NUM_OF_CYCLES_DURING_STAGE_2 << std::endl;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    std::cout << " - - - - " << std::endl;

    getchar();

    return 0;
}
