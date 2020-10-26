#include <unistd.h>

#include <csignal>
#include <cstdlib>
#include <iostream>

#include "gpm_config.hpp"
#include "gpm_logging.hpp"
#include "gpm_signals.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  int exitcode = 19;
  try {
    gpm_signals sig;

    sig.setup_signal_handler();
    int sig_get(0);

    // start parsing config file
    gpm_config cfg(argc, argv);

    /*while (sig_get != SIGINT) {
      while (!sig.signal_available()) {
        sleep(1);
      }

      sig_get = sig.get_signal();
      logging::INFO("Got signal: " + std::to_string(sig_get));
    }*/
    exitcode = 0;
  } catch (SignalException &e) {
    logging::ERROR("Error Handler");
  }
  logging::INFO("Exiting...");
  return exitcode;
}
