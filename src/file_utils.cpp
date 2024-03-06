#include "file_utils.hpp"
#include "string_utils.hpp"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string.h>
#include <string>
#include <system_error>
#include <unordered_map>
#include <vector>

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

std::pmr::vector<std::string> ls(std::string dirpath) {
  std::pmr::vector<std::string> ls_list;
  for (const auto &entry : std::filesystem::directory_iterator(
           std::filesystem::absolute(dirpath))) {
    ls_list.emplace_back(entry.path().string());
  }
  return ls_list;
}

std::pmr::vector<std::string> ls_files(std::string dirpath) {
  std::pmr::vector<std::string> ls_list = ls(dirpath);
  std::pmr::vector<std::string> ls_files_list;
  for (std::string name : ls_list) {
    if (!dir_exists(name)) {
      ls_files_list.emplace_back(name);
    }
  }
  return ls_files_list;
}

std::pmr::vector<std::string> ls_dirs(std::string dirpath) {
  std::pmr::vector<std::string> ls_list = ls(dirpath);
  std::pmr::vector<std::string> ls_dirs_list;
  for (std::string name : ls_list) {
    if (!file_exists(name)) {
      ls_dirs_list.emplace_back(name);
    }
  }
  return ls_dirs_list;
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
  std::string tmp_file = file_abs_name_from_dirname(
      filename, std::filesystem::path(filename).parent_path().append(".tmp"));
  std::ofstream output_file(tmp_file);
  std::string line;
  if (input_file.is_open() && output_file.is_open()) {
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
  } else {
    std::cout << "Error opening file" << std::endl;
    return 1;
  }
  input_file.close();
  output_file.close();
  mv(tmp_file, std::filesystem::absolute(filename));
  return 0;
}

int cat(std::string to, std::pmr::vector<std::string> files) {
  std::ofstream output_file(std::filesystem::absolute(to));
  for (std::string file : files) {
    std::ifstream input_file(std::filesystem::absolute(file));
    if (input_file.is_open() && output_file.is_open()) {
      output_file << std::endl
                  << "#\t--------" << std::endl
                  << "#\t" << file << " starts here\t" << std::endl
                  << "#\t--------" << std::endl
                  << input_file.rdbuf() << std::endl
                  << "#\t--------" << std::endl
                  << "#\t" << file << " ends here\t" << std::endl
                  << "#\t--------" << std::endl;
    } else {
      std::cout << "Error opening file" << std::endl;
      return 1;
    }
    input_file.close();
  }
  output_file.close();
  return 0;
}

static inline bool is_comment(std::string line) {
  return first_non_space_char(line) == 35; /* 35 is ascii for # */
}

int remove_dup_lines(std::string filename) {
  std::ifstream input_file(std::filesystem::absolute(filename));
  std::string tmp_file = file_abs_name_from_dirname(
      filename, std::filesystem::path(filename).parent_path().append(".tmp"));
  std::ofstream output_file(tmp_file);
  std::unordered_map<std::string, bool> unique_lines;
  std::string line;
  if (input_file.is_open() && output_file.is_open()) {
    while (std::getline(input_file, line)) {
      if (is_comment(line)) {
        output_file << line << std::endl;
      } else if (unique_lines.find(line) == unique_lines.end()) {
        unique_lines[line] = true;
        output_file << line << std::endl;
      }
    }
  } else {
    std::cout << "Error opening file" << std::endl;
    return 1;
  }
  input_file.close();
  output_file.close();
  mv(tmp_file, std::filesystem::absolute(filename));
  return 0;
}
