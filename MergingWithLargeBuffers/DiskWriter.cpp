#include "DiskWriter.h"


DiskWriter::DiskWriter(std::string outputFile, std::size_t bufferSizeInBytes, std::size_t bytesToWrite) :
	_outputFile(outputFile),
	_bufferSize(bufferSizeInBytes),
	_bytesToWrite(bytesToWrite),
	_writtenBytes(0)
{}


std::size_t DiskWriter::WriteBlock(void* fromBuffer)
{
	if (fromBuffer == nullptr)
	{
		std::cout << "[!] DiskWriter::WriteBlock - fromBuffer == nullptr" << std::endl;
		return 0;
	}

	if (_writtenBytes > _bytesToWrite)
	{
		std::cout << "[!] DiskWriter::WriteBlock - writtenBytes > bytesToWrite" << std::endl;
		return 0;
	}

	std::size_t bytesToWrite = (_writtenBytes + _bufferSize > _bytesToWrite)
		? _bytesToWrite - _writtenBytes
		: _bufferSize
		;

	std::ofstream outputFile(_outputFile, std::ios::binary | std::ios::app);
	if (!outputFile.is_open())
	{
		std::cout << "[!] DiskWriter::WriteBlock - Unable to open outputFile file" << std::endl;
		return 0;
	}

	outputFile.write(reinterpret_cast<const char*>(fromBuffer), bytesToWrite);
	outputFile.close();

	_writtenBytes += bytesToWrite;

	return bytesToWrite;
}
