#include "DiskWriter.h"


DiskWriter::DiskWriter(std::string outputFile, std::size_t bufferSizeInBytes)
	: _outputFile(outputFile), _bufferSize(bufferSizeInBytes)
{}


bool DiskWriter::WriteBlock(void* fromBuffer)
{
	if (fromBuffer == nullptr)
	{
		std::cout << "[!] DiskWriter::WriteBlock - fromBuffer == nullptr" << std::endl;
		return false;
	}

	std::ofstream outputFile(_outputFile, std::ios::binary | std::ios::app);
	if (!outputFile.is_open())
	{
		std::cout << "[!] DiskWriter::WriteBlock - Unable to open outputFile file" << std::endl;
		return false;
	}

	outputFile.write(reinterpret_cast<const char*>(fromBuffer), _bufferSize);

	outputFile.close();

	return true;
}
