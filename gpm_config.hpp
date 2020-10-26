#ifndef GPM_CONFIG_HPP
#define GPM_CONFIG_HPP
#include <boost/program_options.hpp>
#include <cstdint>
#include <string>

class gpm_config {
 private:
  uint8_t threads;
  uint8_t max_clients;
  uint32_t port;
  std::string ip;
  std::string config_file;
  std::string auth_file;
  bool ready;
  bool error;
  void load_config();
  boost::program_options::variables_map vm;
  bool vm_ready;

 public:
  gpm_config(int argc, char *argv[]);
  bool is_ready();
};

#endif  // GPM_CONFIG_HPP
