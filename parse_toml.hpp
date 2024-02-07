#include "toml.hpp"
#include <iostream>
#include <string>

toml::table set_default_config();

toml::table parse_toml(std::string filename, toml::table default_config);
