#include "restore_hosts.hpp"
#include "file_utils.hpp"
#include <string>
#include <string_view>

using namespace std::string_view_literals;

constexpr std::string_view default_hosts_file = R"(
# Loopback entries; do not change.
# For historical reasons, localhost precedes localhost.localdomain:
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
::1         localhost localhost.localdomain localhost6 localhost6.localdomain6
# See hosts(5) for proper format and other examples:
# 192.168.1.10 foo.mydomain.org foo
# 192.168.1.13 bar.mydomain.org bar
)"sv;

int restore_hosts(std::string filepath) {
  return cat_string_view_to_file(filepath, default_hosts_file);
}
