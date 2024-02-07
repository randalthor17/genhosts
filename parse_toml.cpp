#include "parse_toml.hpp"
#include "toml.hpp"
#include <iostream>

toml::table set_default_config() {
  return toml::table{
      {"prefs", toml::table{{"hosts-save-dir", "/etc/hosts.d"},
                            {"config-file", "/etc/genhosts/config.toml"}}}};
}

toml::table parse_toml(std::string filename, toml::table default_config) {
  try {
    auto toml_obj = toml::parse_file(filename);
    if (!toml_obj.empty()) {
      return toml_obj;
    } else {
      return default_config;
    }
  } catch (const toml::parse_error &err) {
    std::cerr << "Parsing failed:\n"
              << err << "\nUsing default settings instead\n";
    return default_config;
  }
}

/* int main(int argc, char *argv[]) { */
/*   auto default_config = toml::table{ */
/*       {"prefs", toml::table{{"hosts-save-dir", "/etc/hosts.d"}, */
/*                             {"config-file", "/etc/genhosts/config.toml"}}}};
 */
/**/
/*   toml::table config = parse_toml(argv[1], default_config); */
/*   std::cout << config << std::endl; */
/*   return 0; */
/* } */
