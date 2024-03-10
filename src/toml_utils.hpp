#include "toml.hpp"
#include <optional>
#include <string>
#include <vector>
#define TOML_EXCEPTIONS 1

/* toml::table parse_toml(std::string filename, toml::table default_config); */

using namespace std::string_view_literals;

const std::string config_location = "/etc/genhosts/config.toml";

constexpr std::string_view default_config_str =
    R"(
# preferences; where to save downloaded hosts files, where to look for config files, etc
[prefs]
# location of downloaded hosts files
hosts_save_dir = '/etc/hosts.d'

# base hosts file; stevenblack's unified hosts by default
[base]
name = "StevenBlack's Unified Hosts"
url = "https://raw.githubusercontent.com/StevenBlack/hosts/master/hosts"

# extra hosts (optional)
# extras should be added like the following:
# [[extra]]
# name = "d3ward's hosts"
# url = "https://raw.githubusercontent.com/d3ward/toolz/master/src/d3host.txt"

# extra hosts can be disabled like the following:
[[extra]]
name = "test"
url = "test"
disabled = true
)"sv;

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
