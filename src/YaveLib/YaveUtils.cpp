#include "YaveUtils.h"
#include <fstream>

namespace YaveLib {
  std::vector<char>   readBinaryFile(const std::string &filename)
  {
    std::ifstream	file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
      throw YaveLib::Error(std::string("error reading binary file ") + filename);

    size_t  fileSize = (size_t)file.tellg();
    std::vector<char>   buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);

    return (buffer);
  }
}
