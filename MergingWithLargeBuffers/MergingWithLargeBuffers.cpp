
/* TODO

>> Stage 1

    [ ] 1.1. read (at most) n*b records from tape.bin to n bufferes, b records each
    [ ] 1.2. sort (ascending) all read records using QuickSort to create n*b-records-length run
    [ ] 1.3. write the run to file
    [ ] 1.4. if EOF tabe.bin go to Stage 2, else go to step 1.

>> Stage 2

    [ ] 2.1. get subsequent at most n-1 runs frmo file
    [ ] 2.2. merge these runs based on algorithm from "14.6.2 Multiway Merging", saving the result of merging to file
            (source: https://opendsa-server.cs.vt.edu/ODSA/Books/Everything/html/ExternalSort.html)
    [ ] 2.3. if all runs from file are sorted go to 2.4., else go to 2.1.
    [ ] 2.4. if only one run remained go to 2.5., else go to 2.1.
    [ ] 2.5. stop

*/


#include <iostream>


#include "Context.h"


int main()
{
    Context ctx("config.txt");
    return 0;
}
