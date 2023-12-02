
/* TODO

>> Stage 1

    [X] 1.1. read (at most) n*b records from tape.bin to n bufferes, b records each
    [X] 1.2. sort (ascending) all read records using QuickSort to create n*b-records-length run
    [X] 1.3. write the run to file
    [X] 1.4. if EOF tabe.bin go to Stage 2, else go to step 1.

>> Stage 2

    [ ] 2.1. get subsequent at most n-1 runs from file
    [ ] 2.2. merge these runs based on algorithm from "14.6.2 Multiway Merging", saving the result of merging to file
            (source: https://opendsa-server.cs.vt.edu/ODSA/Books/Everything/html/ExternalSort.html)
    [ ] 2.3. if all runs from file are sorted go to 2.4., else go to 2.1.
    [ ] 2.4. if only one run remained go to 2.5., else go to 2.1.
    [ ] 2.5. stop

*/


#include <iostream>
#include <vector>

#include "Utils.h"
#include "Context.h"
#include "DiskReader.h"
#include "DiskWriter.h"
#include "Buffers.h"
#include "Buffer.h"
#include "Quicksort.h"


bool StageOne(Context& ctx, Buffers& buffers)
{
    DiskReader tapeReader("tape.bin", ctx.BufferSizeInBytes());
    DiskWriter tapeWriter("afterStageOne.bin", ctx.BufferSizeInBytes());

    while (tapeReader.IsSthToRead())
    {
        // read at most n*b records
        for (std::size_t ithBufferIdx = 0; ithBufferIdx < buffers.NumberOfBuffers(); ithBufferIdx++)
        {
            data_t* toBuffer = buffers.GetBuffer(ithBufferIdx).GetPointer();

            if (tapeReader.ReadBlock((void*)toBuffer))
                ++buffers.occupiedBuffers;
        }

        // sort records
        Quicksort::Sort(buffers, 0, ctx.NumberOfRecordsInBuffer() * buffers.occupiedBuffers - 1);

        // print check
        buffers.DumpToTXT("afterStageOne.txt");

        // write records
        std::size_t buffersToWrite = buffers.occupiedBuffers;
        for (unsigned int ithBufferIdx = 0; ithBufferIdx < buffersToWrite; ithBufferIdx++)
        {
            data_t* fromBuffer = buffers.GetBuffer(ithBufferIdx).GetPointer();

            if (tapeWriter.WriteBlock((void*)fromBuffer))
                --buffers.occupiedBuffers;
        }
    }

    return true;
}

bool StageTwo()
{

}


int main()
{
    Context ctx("config.txt");
    if (ctx.IsConfigFileReadSuccessfully() == false) return 1;

    Buffers buffers(ctx);
    // check if buffers good

    bool result = StageOne(ctx, buffers);

    if (result)
        return 0;
    else
        return 1;
}
