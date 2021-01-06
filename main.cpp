#include <unistd.h>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <csignal>
#include <cstdlib>
#include <iostream>

#include "gpm_config.hpp"
#include "gpm_logging.hpp"
#include "gpm_signals.hpp"
#include "gpm_tcp_server.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  int exitcode = 19;
  try {
    gpm_signals sig;
    std::srand(std::time(0));
    sig.setup_signal_handler();
    int sig_get(0);

    // start parsing config file
    gpm_config cfg(argc, argv);

    boost::asio::io_context io_context;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
        wg = boost::asio::make_work_guard(io_context);

    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), cfg.port());

    gpm_tcp_server srv(io_context, ep);

    boost::thread t(boost::bind(&boost::asio::io_context::run, &io_context));
    t.detach();
    // std::thread thr(&boost::asio::io_context::run, &io_context);
    // io_context.run();

    while (sig_get != SIGINT) {
      while (!sig.signal_available()) {
        sleep(1);
      }
      // std::cout << std::endl;
      sig_get = sig.get_signal();
      logging::INFO("Got signal: " + std::to_string(sig_get));
      if (sig_get == 11) {
        exit(11);
      }
      wg.reset();
    }
    exitcode = 0;
  } catch (const boost::system::system_error &e) {
    logging::ERROR(e.what());
  } catch (SignalException &e) {
    logging::ERROR("Error Handler");
  }
  logging::INFO("Exiting...");
  return exitcode;
}
