#ifndef GPM_SIGNALS_HPP
#define GPM_SIGNALS_HPP

#include <csignal>
#include <list>
#include <queue>
#include <stdexcept>

class SignalException : public std::runtime_error {
public:
  SignalException(const std::string &_message) : std::runtime_error(_message) {}
};

class gpm_signals {

private:
  static std::queue<int> g_signals;
  static int g_signal;

public:
  gpm_signals();
  ~gpm_signals();

  void setup_signal_handler();

  static void signal_handler(int signum);
  static bool signal_available();

  static int get_signal();
};

#endif // GPM_SIGNALS_HPP
