#ifndef GAMESRVMSG_HPP
#define GAMESRVMSG_HPP

#include <SFML/Graphics.hpp>
#include <string>
class gamesrvmsg {
 public:
  gamesrvmsg();
  std::string to_string() { return login_ + message_string; };
  void from_string(std::string s);
  void SetLogin(std::string login) { login_ = login; };
  bool login = false;
  bool loc = false;
  std::string GetLogin() { return login_; };
  sf::Vector2u GetLocation() { return coord; };

 private:
  std::string message_string;
  std::string login_;
  sf::Vector2u coord;
};

#endif  // GAMESRVMSG_HPP
