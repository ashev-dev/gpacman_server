#include "gpm_config.hpp"

#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "gpm_logging.hpp"

void gpm_config::load_config() {
  try {
    logging::DEBUG("Starting argument parsing");
    config_file = vm["file"].as<std::string>();

    if (std::filesystem::exists(config_file)) {
      logging::INFO("Using " + config_file + " as config");
    } else {
      throw std::invalid_argument("Config file not found");
    }

    std::stringstream ss;
    {
      std::ifstream file(config_file, std::ios::in);
      if (!file.is_open()) {
        throw std::runtime_error("Could not open config file");
      }
      ss << file.rdbuf() << std::endl;
    }
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss, pt);

    _port = pt.get<uint32_t>("port", 8080);
    if (pt.count("port") == 0) {
      logging::DEBUG("Using default port 8080");
    }
    logging::INFO("Using port " + std::to_string(_port));

    threads = pt.get<uint32_t>("threads", 4);
    if (pt.count("threads") == 0) {
      logging::DEBUG("Using default threads 4");
    }
    logging::INFO("Using threads: " + std::to_string(threads));

    ip = pt.get<std::string>("ip", "127.0.0.1");
    if (pt.count("ip") == 0) {
      logging::DEBUG("Using default ip 127.0.0.1");
    }
    logging::INFO("Using ip " + ip);

  } catch (std::logic_error &e) {
    logging::ERROR("Config file " + config_file + " not exist");
    exit(0);
  } catch (boost::property_tree::json_parser::json_parser_error &e) {
    logging::ERROR("Check config file style");
    logging::ERROR(e.what());
    exit(0);
  }

  catch (std::exception &e) {
    logging::ERROR(e.what());
    exit(0);
  }

  ready = 1;
}
gpm_config::gpm_config(int argc, char *argv[]) {
  logging::DEBUG("Arg count: " + std::to_string(argc));
  uint8_t i = 0;
  for (auto &s : std::vector<char *>(argv, argv + argc))
    logging::DEBUG("Arg " + std::to_string(i++) + " is " + std::string(s));

  std::string config, auth;
  boost::program_options::options_description desc("General options");
  desc.add_options()("help,h", "Show help")(
      "file,f",
      boost::program_options::value<std::string>(&config)->default_value(
          "gpacman_server.conf"),
      "Path to config file")(
      "auth,a",
      boost::program_options::value<std::string>(&auth)->default_value(
          "gpacman_server.db"),
      "Path to auth file");

  // boost::program_options::variables_map vm;
  try {
    boost::program_options::parsed_options parsed =
        boost::program_options::command_line_parser(argc, argv)
            .options(desc)
            .run();
    boost::program_options::store(parsed, vm);
    boost::program_options::notify(vm);
    if (vm.count("help")) {
      std::cout << desc << std::endl;
      exit(0);
    }

    logging::INFO("Using config file: " +
                  std::string(vm["file"].as<std::string>()));
    logging::INFO("Using auth file: " +
                  std::string(vm["auth"].as<std::string>()));
    vm_ready = true;
    load_config();
  } catch (std::invalid_argument &ex) {
    logging::WARN(ex.what());
    exit(EXIT_FAILURE);
  } catch (std::exception &ex) {
    logging::ERROR(ex.what());
  }
}

unsigned short gpm_config::port() { return _port; }
