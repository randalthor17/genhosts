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

int cp(std::string from, std::string to);

int mv(std::string from, std::string to);

int rm(std::string filename);

int rm_r(std::string dirname);

int sed(std::string filename, std::string pattern, std::string replace_pattern);

int cat(std::string to, std::vector<std::string> files);

int remove_dup_lines(std::string filename);
