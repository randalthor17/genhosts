#include "argparse.hpp"
#include "file_utils.hpp"
#include "string_utils.hpp"
#include "url_utils.hpp"
#include <cctype>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::string hosts_location = "/etc/hosts";

std::string snakecase(std::string orig_str) {
  std::string new_str;
  for (char &ch : orig_str) {
    if ((32 <= ch && ch <= 47) || (58 <= ch && ch <= 64) ||
        (91 <= ch && ch <= 96) || (123 <= ch && ch <= 126)) {
      new_str.push_back(*"_");
    } else {
      new_str.push_back(std::tolower(ch));
    }
  }
  return new_str;
}

inline std::string gen_hosts_name(url_holder url, std::string dir,
                                  int priority) {
  return file_abs_name_from_dirname(
      std::to_string(priority).append("_").append(snakecase(url.name)), dir);
}

int download_url_holder(url_holder url, std::string dir, int priority) {
  if (!dir_exists(dir)) {
    mkdir_p(dir);
  }
  std::string file_location = gen_hosts_name(url, dir, priority);
  std::cout << "Downloading " << url.name << "..." << std::endl;
  return download_file(url.url, file_location);
}

std::pmr::vector<std::string> ls_files_ignore(std::string dirpath) {
  std::pmr::vector<std::string> ls_files_list = ls_files(dirpath);
  // This matches any filename that has .orig or ,bak in the end of the file
  std::regex pattern("\\.(orig|bak|new)$");
  // This newEnd uses a lambda to search and move any string that matches the
  // pattern to the end of the vector, essentially removing it
  // newEnd points to the first string that matches the pattern
  auto new_end = std::remove_if(ls_files_list.begin(), ls_files_list.end(),
                                [&](const std::string &file) {
                                  return std::regex_search(file, pattern);
                                });
  ls_files_list.erase(new_end, ls_files_list.end());
  return ls_files_list;
}

inline int dump_config(std::string dump_location) {
  std::string dumpdir = std::filesystem::absolute(dump_location).parent_path();
  if (!dir_exists(dumpdir)) {
    mkdir_p(dumpdir);
  }
  return cat_string_view_to_file(dump_location, default_config_str);
}

int main_not_dump_only(argparse_result result) {
  if (result.output_location.has_value()) {
    hosts_location = result.output_location.value();
  }
  std::cout << hosts_location << std::endl;
  std::string hosts_new_location = hosts_location + ".new";
  config_obj config = result.argparse_config_obj.value();
  std::string hosts_save_dir = config.prefs.hosts_save_dir;
  download_url_holder(config.base, hosts_save_dir, 00);
  if (config.extra.has_value()) {
    for (url_holder extra_url : config.extra.value()) {
      download_url_holder(extra_url, hosts_save_dir, 99);
    }
  }
  std::pmr::vector<std::string> hosts_list = ls_files_ignore(hosts_save_dir);
  // std::cout << "Removing old " << hosts_new_location << "..." << std::endl;
  // rm(hosts_new_location);
  std::cout << "Concatenating the hosts files to " << hosts_new_location
            << "..." << std::endl;
  cat_file_to_file(hosts_new_location, hosts_list);
  std::cout << "Removing duplicate entries from " << hosts_new_location << "..."
            << std::endl;
  remove_dup_lines(hosts_new_location);
  std::cout << "Moving " << hosts_new_location << " to " << hosts_location
            << "..." << std::endl;
  mv(hosts_new_location, hosts_location);
  std::cout << "Done!" << std::endl;
  return 0;
}

int main(int argc, char *argv[]) {
  argparse_result result = parse_args(argc, argv);
  if (!result.help_only) {
    if (result.dump_only) {
      return dump_config(result.dump_location.value());
    } else {
      return main_not_dump_only(result);
    }
    return 0;
  }
}
