#include "file_utils.hpp"
#include "string_utils.hpp"
#include "toml_utils.hpp"
#include "url_utils.hpp"
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

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
  auto file = ls_files_list.begin();
  while (file != ls_files_list.end()) {
    if ((file->find(".bak")) || (file->find("orig"))) {
      file = ls_files_list.erase(file);
    } else {
      file++;
    }
  }
  return ls_files_list;
}

int main(int argc, char *argv[]) {
  config_obj config;
  std::string hosts_save_dir = config.prefs.hosts_save_dir;
  download_url_holder(config.base, hosts_save_dir, 00);
  if (config.extra.has_value()) {
    for (url_holder extra_url : config.extra.value()) {
      download_url_holder(extra_url, hosts_save_dir, 99);
    }
  }
  std::pmr::vector<std::string> hosts_list = ls_files(hosts_save_dir);
  cat(std::string("/etc/hosts"), hosts_list);
  return 0;
}
