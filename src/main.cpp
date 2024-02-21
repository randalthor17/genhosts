#include "toml_utils.hpp"

int main(int argc, char *argv[]) {
  toml::table config = set_default_config();
  if (argc > 1) {
    config = parse_toml(std::string(argv[1]), config);
  }
  url_list test(config);
  return 0;
}
