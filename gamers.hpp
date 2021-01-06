#ifndef GAMERS_HPP
#define GAMERS_HPP
#include <memory>

#include "gamesrvmsg.hpp"
#include "gpm_message.hpp"
class gpm_participant {
 public:
  virtual ~gpm_participant() {}
  virtual void deliver(const gpm_message& msg) = 0;
  virtual std::string get_addr() = 0;
};

typedef std::shared_ptr<gpm_participant> gpm_participant_ptr;
class Gamers {
 public:
  Gamers();
  void RemovePlayer(gpm_participant_ptr participant);
  void AddPlayer(std::string login, gpm_participant_ptr prt);
  void AddMessage(std::string s, gpm_participant_ptr prt);

  void run();

 private:
  std::vector<std::string> messages_rx;
  std::vector<std::string> messages_tx;
  std::vector<std::pair<std::string, gpm_participant_ptr>> players;
  std::map<std::string, sf::Vector2u> locations;
};

#endif  // GAMERS_HPP
