#include "toml_utils.hpp"
#include "toml.hpp"
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

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
    auto extra_table_placeholder = extra_table.as_table();
    if (!extra_table_placeholder->contains("disabled") ||
        !extra_table_placeholder->get_as<bool>("disabled")) {

      extra.push_back((url_holder){
          .name = extra_table_placeholder->get("name")->value_or(""),
          .url = extra_table_placeholder->get("url")->value_or("")});
    }
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
