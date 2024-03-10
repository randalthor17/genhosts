#include "user_utils.hpp"

std::string whoami() {
  /* gets username */
  uid_t uid = geteuid();
  struct passwd *pw = getpwuid(uid);
  return std::string(pw->pw_name);
}

bool is_root() {
  /* checks if the user running the program is root */
  std::string username = whoami();
  return username.compare("root") == 0;
}
