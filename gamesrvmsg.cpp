#include "gamesrvmsg.hpp"

#include "gpm_logging.hpp"
gamesrvmsg::gamesrvmsg() {}

void gamesrvmsg::from_string(std::string s) {
  std::string delimiter = ":";

  std::vector<std::string> msg;

  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    msg.push_back(token);
    s.erase(0, pos + delimiter.length());
  }
  msg.push_back(s);

  if (msg.size() == 2) {
    if (msg[0] == "login") {
      login = true;
      login_ = msg[1];
    }
  }
  if (msg.size() == 6) {
    login_ = msg[0];

    if (msg[1] == "loc") {
      loc = true;
      coord.x = std::stoi(msg[3]);
      coord.y = std::stoi(msg[5]);

      logging::DEBUG("From: " + login_ + " type: loc x:" +
                     std::to_string(coord.x) + " y:" + std::to_string(coord.y));
    }
  }
}
