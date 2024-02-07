#include "parse_toml.hpp"
#include "toml.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  auto default_config = set_default_config();
  toml::table config = parse_toml(argv[1], default_config);
  std::cout << config << std::endl;
  return 0;
}
