#include "string_utils.hpp"
#include <filesystem>

char first_non_space_char(std::string line) {
  if (line.front() == 32 ||
      line.front() == 9) { /* 32 is ascii for SPACE, and 9 is for TAB */
    line.erase(0, 1);
    first_non_space_char(line);
  }
  return line.front();
}

std::string file_abs_name_from_dirname(std::string filename,
                                       std::string dirname) {
  return std::filesystem::absolute(dirname)
      .string()
      .append(DIR_SEPARATOR)
      .append(std::filesystem::path(filename).filename());
}
