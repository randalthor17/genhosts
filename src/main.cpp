#include "file_utils.hpp"

int main(int argc, char *argv[]) {
  return remove_dup_lines(std::string(argv[1]));
}
