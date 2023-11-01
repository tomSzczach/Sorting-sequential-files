
#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}

// TODO
// > set up N - number of records
// > set up max number of vector dimentions (~ R - record size)
// > set up b - number of records on one disk page (buffers have the same size)
// > choose whether set up records automatically or manually
// > choose whether run sorting program in step mode
// > create tape.bin file
// > copy tabe.bin to beforeSorting.txt
// > create config.txt file

// record: [ key=lengthOfVector (int) | value in dimention 1 (int) ... value in dimention R (int) ]
