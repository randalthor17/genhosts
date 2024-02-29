#include "toml_utils.hpp"
#include "toml.hpp"
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std::string_view_literals;

static constexpr std::string_view default_config_str =
    R"(# preferences; where to save downloaded hosts files, where to look for config files, etc
[prefs]
# location of downloaded hosts files
hosts_save_dir = '/etc/hosts.d'

# base hosts file; stevenblack's unified hosts by default
[base]
name = "StevenBlack's Unified Hosts"
url = "https://raw.githubusercontent.com/StevenBlack/hosts/master/hosts")"sv;

static inline toml::table set_default_config() {
  toml::table default_config = toml::parse(default_config_str);
  return default_config;
}

static toml::table parse_toml(std::string config_filename) {
  toml::table default_config = set_default_config();
  try {
    if (std::filesystem::exists(std::filesystem::absolute(config_filename))) {
      toml::table toml_obj = toml::parse_file(config_filename);
      if (toml_obj.empty()) {
        std::cout << "File empty, using default config instead\n";
        return default_config;
      } else {
        if (!toml_obj.contains("prefs")) {
          toml_obj.insert("prefs", default_config["prefs"]);
        };
        if (!toml_obj.contains("base")) {
          toml_obj.insert("base", default_config["base"]);
        };
        return toml_obj;
      }
    }
  } catch (const toml::parse_error &err) {
    std::cerr << err.description() << std::endl;
  }
  return default_config;
}

struct pref config_obj::gen_prefs(toml::table config) {
  return (struct pref){
      .hosts_save_dir =
          toml::at_path(config, "prefs.hosts_save_dir").value_or("")};
}

url_holder config_obj::gen_base(toml::table config) {
  return (url_holder){.name = toml::at_path(config, "base.name").value_or(""),
                      .url = toml::at_path(config, "base.url").value_or("")};
}

std::pmr::vector<url_holder> config_obj::gen_extra(toml::table config) {
  std::pmr::vector<url_holder> extra;
  auto toml_extra = config["extra"].as_array();
  for (auto &extra_table : *toml_extra) {
    extra.push_back(
        (url_holder){.name = extra_table.as_table()->get("name")->value_or(""),
                     .url = extra_table.as_table()->get("url")->value_or("")});
  }
  return extra;
}

config_obj::config_obj(std::string filename) {
  toml::table config = parse_toml(filename);
  prefs = gen_prefs(config);
  base = gen_base(config);
  if (!toml::path("extra").empty() && config["extra"].is_array_of_tables()) {
    extra = gen_extra(config);
  }
};

/* int main(int argc, char *argv[]) { */
/*   if (argc == 2) { */
/*     url_list test((std::string(argv[1]))); */
/*     std::cout << "Prefs" << std::endl */
/*               << test.prefs.hosts_save_dir << std::endl */
/*               << "Base" << test.base.name << " " << test.base.url <<
 * std::endl; */
/*     if (test.extra.has_value()) { */
/*       for (url_holder extra_unit : test.extra.value()) { */
/*         std::cout << extra_unit.name << " " << extra_unit.url << std::endl;
 */
/*       } */
/*     } */
/*   } */
/*   return 0; */
/* } */
