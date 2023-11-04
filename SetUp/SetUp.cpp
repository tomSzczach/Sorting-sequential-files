
#include <iostream>
#include <fstream>
#include <random>

const int MAX_VAL = 1000;
const int MIN_VAL = -1000;

void GenerateDataAutomatically(std::ofstream& tape, std::ofstream& beforeSorting, unsigned int N, unsigned int maxNoDim);
void GenerateDataManually(std::ofstream& tape, std::ofstream& beforeSorting, unsigned int N, unsigned int maxNoDim);

int main()
{
    /*
    * b - blocking factor, number of records on one disk page (buffers have the same size)
    * N - number of records
    * n - number of buffers in the main memory available for the sorting process
    * d - max number of vector dimentions
    * R - record size in number of unsigned int values (assumption: record contains only int values)
    */
    unsigned int b, N, n, d, R;
    std::cout << "----" << std::endl;
    std::cout << "Number of records in one disk page (b): ";
    std::cin >> b;
    std::cout << "Number of all records (N): ";
    std::cin >> N;
    std::cout << "Number of buffers (n): ";
    std::cin >> n;
    std::cout << "----" << std::endl;
    std::cout << "Max number vector dimentions (d): ";
    std::cin >> d;

    // A record is an array of maxNoDim+1 int values:
    // first is a header containing Euclidean length of vector
    // rest of values are length of vector cmponents in each dimension
    R = d + 1;

    /*
    * isManualMode - set up records manually (true) or automatically (false) 
    * isStepMode -  run sorting program in step mode (true) or normally (false)
    */
    char decision = 'n';
    bool isManualMode, isStepMode;
    std::cout << "----" << std::endl;
    std::cout << "Set up records manually? [y/n]: ";
    std::cin >> decision;
    isManualMode = (decision == 'y') ? true : false;
    std::cout << "Run sorting program in step mode? [y/n]: ";
    std::cin >> decision;
    isStepMode = (decision == 'y') ? true : false;
    std::cout << "----" << std::endl;


    // write to config.txt file
    // clear previous content
    std::ofstream configFile("config.txt", std::ios::out | std::ios::trunc);
    if (configFile.is_open())
    {
        configFile << "b = " << b << std::endl;
        configFile << "N = " << N << std::endl;
        configFile << "n = " << n << std::endl;
        configFile << "R = " << R << std::endl;
        configFile << "d = " << d << std::endl;
        configFile << "isManualMode = " << isManualMode << std::endl;
        configFile << "isStepMode = " << isStepMode << std::endl;
        configFile.close();
    }
    else std::cout << "[!] Unable to open 'config.txt' file" << std::endl;


    // write records to files: tape.bin and beforeSorting.txt
    std::ofstream tape("tape.bin", std::ios::out | std::ios::trunc | std::ios::binary);
    std::ofstream beforeSorting("beforeSorting.txt", std::ios::out | std::ios::trunc);
    if (tape.is_open() && beforeSorting.is_open())
    {
        if (isManualMode)
        {
            GenerateDataManually(tape, beforeSorting, N, d);
        }
        else
        {
            GenerateDataAutomatically(tape, beforeSorting, N, d);
        }
    }
    else std::cout << "[!] Unable to open 'tape.bin' or 'beforeSorting.txt' file" << std::endl;

    return 0;
}

void GenerateDataAutomatically(std::ofstream& tape, std::ofstream& beforeSorting, unsigned int N, unsigned int maxNoDim)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> noDim(1, maxNoDim);
    std::uniform_int_distribution<> randVal(MIN_VAL, MAX_VAL);

    std::vector<int> V(1 + maxNoDim);

    for (unsigned int ithRecord = 1; ithRecord <= N; ithRecord++)
    {
        // randomly generate a number of vector dimensions [0,noDim]
        int currNoDim = noDim(gen);

        // randomly generate values in these dimensions
        // calculate Euclidean length of vector
        // length of vector as key V[0], values of record stored int he rest of vector V
        V[0] = 0;
        for (unsigned int ithDim = 1; ithDim <= maxNoDim; ithDim++)
        {
            V[ithDim] = (ithDim <= currNoDim) ? randVal(gen) : 0;
            V[0] += V[ithDim] * V[ithDim];
        }
        V[0] = int(sqrt(double(V[0])));

        // write to files
        for (unsigned int i = 0; i <= maxNoDim; i++)
        {
            tape << V[i];
            beforeSorting << V[i] << " ";
            if (i == 0) beforeSorting << "| ";
            if (i == maxNoDim) beforeSorting << std::endl;
        }
    }
}

void GenerateDataManually(std::ofstream& tape, std::ofstream& beforeSorting, unsigned int N, unsigned int maxNoDim)
{
    std::vector<int> V(1 + maxNoDim);

    std::cout << "Type " << maxNoDim << " numbers in range [" << MIN_VAL << "," << MAX_VAL << "] for all records:" << std::endl;

    for (unsigned int ithRecord = 1; ithRecord <= N; ithRecord++)
    {
        std::cout << "Record " << ithRecord << " : ";
        for (unsigned int ithDim = 1; ithDim <= maxNoDim; ithDim++)
        {
            std::cin >> V[ithDim];
            V[0] += V[ithDim] * V[ithDim];
        }
        V[0] = int(sqrt(double(V[0])));

        // write to files
        for (unsigned int i = 0; i <= maxNoDim; i++)
        {
            tape << V[i];
            beforeSorting << V[i] << " ";
            if (i == 0) beforeSorting << "| ";
            if (i == maxNoDim) beforeSorting << std::endl;
        }
    }
}
