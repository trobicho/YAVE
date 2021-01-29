#include "YaveUtils.h"
#include <fstream>

std::vector<char>	YaveLib::readBinaryFile(const std::string &filename)
{
	std::ifstream	file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		throw Yave::Error(std::string("error reading binary file ") + filename)

	size_t	fileSize = (size_t)file.tellg();
	std::vector<char>	buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);

	return (buffer);
}
