#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>


namespace System
{
namespace fs = std::filesystem;
using File = std::filesystem::directory_entry;
using FileList = std::vector<std::filesystem::directory_entry>;

}
#endif
