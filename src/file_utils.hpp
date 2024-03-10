#include <string>
#include <vector>

#if defined(_WIN32) || defined(__CYGWIN__)
#define DIR_SEPARATOR "\\"
#else
#define DIR_SEPARATOR "/"
#endif

int mkdir_p(std::string path);

bool file_exists(std::string filepath);

bool dir_exists(std::string dirpath);

std::pmr::vector<std::string> ls(std::string dirpath);

std::pmr::vector<std::string> ls_files(std::string dirpath);

std::pmr::vector<std::string> ls_dirs(std::string dirpath);

int cp(std::string from, std::string to);

int mv(std::string from, std::string to);

int rm(std::string filename);

int rm_r(std::string dirname);

int sed(std::string filename, std::string pattern, std::string replace_pattern);

int cat_file_to_file(std::string to, std::pmr::vector<std::string> files);

int cat_string_view_to_file(std::string to, std::string_view stringview);

int remove_dup_lines(std::string filename);
