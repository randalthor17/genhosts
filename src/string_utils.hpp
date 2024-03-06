#include <string>

#if defined(_WIN32) || defined(__CYGWIN__)
#define DIR_SEPARATOR "\\"
#else
#define DIR_SEPARATOR "/"
#endif

char first_non_space_char(std::string line);

std::string file_abs_name_from_dirname(std::string filename,
                                       std::string dirname);
