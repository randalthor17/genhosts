#include "file_utils.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <system_error>
#include <vector>

#if defined(_WIN32) || defined(__CYGWIN__)
#define DIR_SEPARATOR "\\"
#else
#define DIR_SEPARATOR "/"
#endif

int mkdir_p(std::string path) {
  try {
    std::error_code ec;
    std::filesystem::create_directories(std::filesystem::absolute(path, ec),
                                        ec);
    if (ec.value() != 0) {
      throw ec;
    } else {
      return 0;
    }
  } catch (std::error_code ec) {
    std::cout << ec.message() << std::endl;
    return ec.value();
  }
}

bool file_exists(std::string filepath) {
  if (std::filesystem::exists(filepath)) {
    return std::filesystem::is_regular_file(filepath);
  } else {
    return false;
  }
}

bool dir_exists(std::string dirpath) {
  if (std::filesystem::exists(dirpath)) {
    return std::filesystem::is_directory(dirpath);
  } else {
    return false;
  }
}

int cp(std::string from, std::string to) {
  try {
    std::error_code ec;
    std::filesystem::copy(std::filesystem::absolute(from, ec),
                          std::filesystem::absolute(to, ec), ec);
    if (ec.value() != 0) {
      throw ec;
    } else {
      return 0;
    }
  } catch (std::error_code ec) {
    std::cout << ec.message() << std::endl;
    return ec.value();
  }
}

int mv(std::string from, std::string to) {
  try {
    std::error_code ec;
    std::filesystem::rename(std::filesystem::absolute(from),
                            std::filesystem::absolute(to, ec), ec);
    if (ec.value() != 0) {
      throw ec;
    } else {
      return 0;
    }
  } catch (std::error_code ec) {
    std::cout << ec.message() << std::endl;
    return ec.value();
  }
}

int rm(std::string filename) {
  try {
    std::error_code ec;
    std::filesystem::remove(std::filesystem::absolute(filename), ec);
    if (ec.value() != 0) {
      throw ec;
    } else {
      return 0;
    }
  } catch (std::error_code ec) {
    std::cout << ec.message() << std::endl;
    return ec.value();
  }
}

int rm_r(std::string dirname) {
  try {
    std::error_code ec;
    std::filesystem::remove_all(std::filesystem::absolute(dirname), ec);
    if (ec.value() != 0) {
      throw ec;
    } else {
      return 0;
    }
  } catch (std::error_code ec) {
    std::cout << ec.message() << std::endl;
    return ec.value();
  }
}

int sed(std::string filename, std::string pattern,
        std::string replace_pattern) {
  std::ifstream input_file(std::filesystem::absolute(filename));
  std::string tmp_file =
      std::filesystem::absolute(std::filesystem::path(filename))
          .parent_path()
          .string()
          .append(DIR_SEPARATOR)
          .append(std::filesystem::path(filename).filename())
          .append(".tmp");
  std::cout << tmp_file << std::endl;
  std::ofstream output_file(tmp_file);
  std::string line;
  while (std::getline(input_file, line)) {
    while (true) {
      size_t position = line.find(pattern);
      if (position != std::string::npos) {
        line.replace(position, pattern.length(), replace_pattern);
      } else {
        break;
      }
    }
    output_file << line << std::endl;
  }
  input_file.close();
  output_file.close();
  mv(tmp_file, std::filesystem::absolute(filename));
  return 0;
}

int cat(std::string to, std::vector<std::string> files) {
  std::ofstream output_file(std::filesystem::absolute(to));
  for (std::string file : files) {
    std::ifstream input_file(std::filesystem::absolute(file));
    output_file << "\n--------\n#\t" << file << " starts here\t\n--------\n"
                << input_file.rdbuf() << "\n--------\n#\t" << file
                << " ends here\t\n--------\n";
    input_file.close();
  }
  output_file.close();
  return 0;
}
