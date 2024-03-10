#include "argparse.hpp"
#include <cxxopts.hpp>
#include <iostream>
#include <optional>
#include <string>

static std::string hosts_save_dir = toml::parse(default_config_str)
                                        .at_path("prefs.hosts_save_dir")
                                        .value_or("");

argparse_result apply_args(cxxopts::ParseResult result,
                           cxxopts::Options options) {
  argparse_result result_return;
  if (!result.unmatched().empty()) {
    result_return.unrecognized_opts = true;
    std::cout << "Unrecognized options: ";
    for (std::string unrecognized_opt : result.unmatched()) {
      std::cout << unrecognized_opt << " ";
    }
    std::cout << std::endl << options.help({""}) << std::endl;
    return result_return;
  } else {
    result_return.unrecognized_opts = false;
  }
  if (result.count("help")) {
    result_return.help_only = true;
    std::cout << options.help({""}) << std::endl;
    return result_return;
  } else {
    result_return.help_only = false;
  }
  if (result.count("dump-config")) {
    result_return.dump_only = true;
    result_return.dump_location = result["dump-config"].as<std::string>();
    return result_return;
  } else {
    result_return.dump_only = false;
  }
  if (result.count("restore")) {
    result_return.restore_only = true;
    result_return.restore_location = result["restore"].as<std::string>();
    return result_return;
  } else {
    result_return.restore_only = false;
  }
  if (result.count("config")) {
    hosts_save_dir = result["config"].as<std::string>();
  }
  result_return.argparse_config_obj = config_obj(hosts_save_dir);
  if (result.count("hosts-save-dir")) {
    result_return.argparse_config_obj->prefs.hosts_save_dir =
        result["hosts-save-dir"].as<std::string>();
  }
  if (result.count("output")) {
    result_return.output_location = result["output"].as<std::string>();
  }
  return result_return;
}

argparse_result parse_args(int argc, char *argv[]) {

  cxxopts::Options options("genhosts",
                           "A tool to generate hosts file from URLs");
  options.allow_unrecognised_options();
  options.add_options("")(
      "c,config", "Configuration file location",
      cxxopts::value<std::string>()->default_value(config_location))(
      "hosts-save-dir", "Hosts file download directory",
      cxxopts::value<std::string>()->default_value(hosts_save_dir))(
      "o,output", "Hosts output path",
      cxxopts::value<std::string>()->default_value("/etc/hosts"))(
      "s,dump-config", "Dump config to a specific path",
      cxxopts::value<std::string>()->implicit_value(config_location))(
      "restore", "Restore hosts file",
      cxxopts::value<std::string>()->implicit_value("/etc/hosts"))(
      "h,help", "Print help", cxxopts::value<bool>()->default_value("false"));
  auto result = options.parse(argc, argv);
  return apply_args(result, options);
}
