#include "toml_utils.hpp"
#include "toml.hpp"
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

url_list::url_list(toml::table config) {
  base = (url_holder){.name = toml::at_path(config, "base.name").value_or(""),
                      .url = toml::at_path(config, "base.url").value_or("")};
  if (config["extra"].is_array_of_tables()) {
    auto toml_extra = config["extra"].as_array();
    for (auto &extra_table : *toml_extra) {
      extra.push_back((url_holder){
          .name = extra_table.as_table()->get("name")->value_or(""),
          .url = extra_table.as_table()->get("url")->value_or("")});
    }
  }
};

toml::table set_default_config() {
  return toml::table{
      {"prefs", toml::table{{"hosts-save-dir", "/etc/hosts.d"},
                            {"config-file", "/etc/genhosts/config.toml"}}},
      {"base", toml::table{{"name", "StevenBlack's Unified Hosts"},
                           {"url", "https://raw.githubusercontent.com/"
                                   "StevenBlack/hosts/master/hosts"}}}};
}

toml::table parse_toml(std::string filename, toml::table default_config) {
  try {
    auto toml_obj = toml::parse_file(filename);
    if (toml_obj.empty()) {
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
  } catch (const toml::parse_error &err) {
    std::cerr << "Parsing failed:\n"
              << err << "\nUsing default settings instead\n";
    return default_config;
  }
}
