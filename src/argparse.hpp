#include "toml_utils.hpp"
#include <optional>
#include <string>

struct argparse_result {
  bool help_only;
  bool dump_only;
  std::optional<std::string> dump_location;
  std::optional<config_obj> argparse_config_obj;
  std::optional<std::string> output_location;
};

argparse_result parse_args(int argc, char *argv[]);
