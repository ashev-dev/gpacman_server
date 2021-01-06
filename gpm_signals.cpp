#include "gpm_signals.hpp"

#include <csignal>
#include <iostream>
#include <queue>

#include "gpm_logging.hpp"

int gpm_signals::g_signal = 0;
std::queue<int> gpm_signals::g_signals;

gpm_signals::gpm_signals() {}

gpm_signals::~gpm_signals() {}

bool gpm_signals::signal_available() {
  if (gpm_signals::g_signal > 0) {
    gpm_signals::g_signals.push(gpm_signals::g_signal);
    gpm_signals::g_signal = 0;
  }
  if (!gpm_signals::g_signals.empty()) {
    return true;
  }
  return false;
}

int gpm_signals::get_signal() {
  int ret_val = 0;
  if (gpm_signals::signal_available()) {
    ret_val = gpm_signals::g_signals.front();
    gpm_signals::g_signals.pop();
  }
  return ret_val;
}
void gpm_signals::signal_handler(int signum) { gpm_signals::g_signal = signum; }

void gpm_signals::setup_signal_handler() {
  std::list<int> sys_signals = {SIGINT,  SIGILL, SIGABRT, SIGFPE, SIGSEGV,
                                SIGTERM, SIGHUP, SIGQUIT, /*SIGTRAP, SIGKILL, */
                                SIGPIPE, SIGALRM};
  for (const int &sig : sys_signals) {
    if (signal(sig, gpm_signals::signal_handler) == SIG_ERR) {
      throw SignalException("Error setting up signal handler");
    } else {
      logging::DEBUG("Signal " + std::to_string(sig) + " captured!");
    }
  }
}
