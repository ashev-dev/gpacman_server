#include "gamers.hpp"

#include <iostream>
#include <thread>

#include "gamesrvmsg.hpp"
#include "gpm_logging.hpp"
Gamers::Gamers() {}

void Gamers::RemovePlayer(gpm_participant_ptr participant) {
  auto iter = players.begin();   // получаем итератор
  while (iter != players.end())  // пока не дойдем до конца
  {
    if ((*iter).second == participant) players.erase(iter);
    ++iter;  // перемещаемся вперед на один элемент
  }
}

void Gamers::AddMessage(std::string s, gpm_participant_ptr prt) {
  // std::cout << "Add message: " + s << std::endl;
  gamesrvmsg m;
  m.from_string(s);
  if (m.login) {
    // std::cout << "Mlogin: " + m.GetLogin() << std::endl;
    this->players.push_back(std::pair(m.GetLogin(), prt));
    if (players.size() > 1) {
      for (auto i : players) {
        i.second->deliver(gpm_message("srv:start"));
      }
      std::thread th(&Gamers::run, this);
      th.detach();
    }
  }
  if (m.loc) {
    // gamers location
    locations[m.GetLogin()] = m.GetLocation();
    logging::DEBUG("user: " + m.GetLogin() +
                   " at: " + std::to_string(m.GetLocation().x) + ":" +
                   std::to_string(m.GetLocation().y));
  }
}

void Gamers::run() {
  auto t1 = std::chrono::high_resolution_clock::now();
  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (!this->messages_tx.empty()) {
      for (auto i : players) {
        i.second->deliver(gpm_message(messages_tx.back()));
      }
      //  std::cout << messages_tx.back() << std::endl;
      messages_tx.pop_back();
    }
    // rx parser

    // food generator
    auto t2 = std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(abs(t2 - t1))
            .count() > 1000) {
      uint8_t x = std::rand() & 30;
      uint8_t y = std::rand() & 15;
      messages_tx.push_back("srv:fda:x:" + std::to_string(x + 1) +
                            ":y:" + std::to_string(y + 1));
      t1 = std::chrono::high_resolution_clock::now();
    }
    for (auto x : locations)
      for (auto y : locations) {
        if ((x.first != y.first) & (x.second == y.second)) {
          logging::INFO("Damage users:" + x.first + " vs " + y.first);
          messages_tx.push_back("srv:acc:" + x.first + ":" + y.first);
        }
      }

    if (players.size() <= 1) {
      for (auto i : players) {
        i.second->deliver(gpm_message("srv:stop"));
      }
    }
  }
}
