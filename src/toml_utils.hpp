#include "toml.hpp"
#include <string>

toml::table set_default_config();

toml::table parse_toml(std::string filename, toml::table default_config);

struct url_holder {
  std::string name;
  std::string url;
};

class url_list {
  url_holder base;
  std::pmr::vector<url_holder> extra;

public:
  url_list(toml::table config);
};
