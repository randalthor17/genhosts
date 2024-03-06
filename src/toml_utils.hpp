#include "toml.hpp"
#include <optional>
#include <string>
#include <vector>
#define TOML_EXCEPTIONS 1

/* toml::table parse_toml(std::string filename, toml::table default_config); */

const std::string config_location = {"/etc/genhosts/config.toml"};

struct pref {
  std::string hosts_save_dir;
};

struct url_holder {
  std::string name;
  std::string url;
};

class config_obj {
public:
  struct pref prefs;
  url_holder base;
  std::optional<std::pmr::vector<url_holder>> extra;

  config_obj(std::string filename = config_location);

private:
  struct pref gen_prefs(toml::table config);
  url_holder gen_base(toml::table config);
  std::pmr::vector<url_holder> gen_extra(toml::table config);
};
