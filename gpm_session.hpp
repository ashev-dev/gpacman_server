#include <boost/asio.hpp>

using boost::asio::ip::tcp;
#include <boost/asio.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>

#include "gamers.hpp"
#include "gamesrvmsg.hpp"
#include "gpm_logging.hpp"
#include "gpm_message.hpp"
//----------------------------------------------------------------------

typedef std::deque<gpm_message> gpm_message_queue;

//----------------------------------------------------------------------

//----------------------------------------------------------------------

class gpm_room {
 public:
  Gamers m_gamers;
  void join(gpm_participant_ptr participant) {
    participants_.insert(participant);
    gpm_message msg("Welcome");
    participant->deliver(msg);
    logging::DEBUG(participant->get_addr());
    for (auto msg : recent_msgs_) participant->deliver(msg);
    //    if (participants_.size() > 2) {
    //      for (auto i : participants_) {
    //        i->deliver(gpm_message("srv:start"));
    //      }
    //    }
  }

  void leave(gpm_participant_ptr participant) {
    participants_.erase(participant);
    m_gamers.RemovePlayer(participant);
    logging::DEBUG("Participant leaved");
  }

  void deliver(const gpm_message& msg) {
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs) recent_msgs_.pop_front();

    for (auto participant : participants_) participant->deliver(msg);
  }

 private:
  std::set<gpm_participant_ptr> participants_;
  enum { max_recent_msgs = 100 };
  gpm_message_queue recent_msgs_;
};

//----------------------------------------------------------------------

class gpm_session : public gpm_participant,
                    public std::enable_shared_from_this<gpm_session> {
 public:
  std::string get_addr() {
    return socket_.remote_endpoint().address().to_string();
  }
  gpm_session(tcp::socket socket, gpm_room& room)
      : socket_(std::move(socket)), room_(room) {}

  void start() {
    room_.join(shared_from_this());
    do_read_header();
  }

  void deliver(const gpm_message& msg) {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress) {
      do_write();
    }
  }

 private:
  void do_read_header() {
    auto self(shared_from_this());
    boost::asio::async_read(
        socket_,
        boost::asio::buffer(read_msg_.data(), gpm_message::header_length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
          if (!ec && read_msg_.decode_header()) {
            do_read_body();
          } else {
            room_.leave(shared_from_this());
          }
        });
  }

  void do_read_body() {
    auto self(shared_from_this());
    boost::asio::async_read(
        socket_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
          if (!ec) {
            room_.deliver(read_msg_);
            std::string s(read_msg_.body(), read_msg_.body_length());
            // gamesrvmsg m;
            // m.from_string(s);
            room_.m_gamers.AddMessage(s, shared_from_this());

            do_read_header();
          } else {
            room_.leave(shared_from_this());
          }
        });
  }

  void do_write() {
    auto self(shared_from_this());
    boost::asio::async_write(
        socket_,
        boost::asio::buffer(write_msgs_.front().data(),
                            write_msgs_.front().length()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
          if (!ec) {
            write_msgs_.pop_front();
            if (!write_msgs_.empty()) {
              do_write();
            }
          } else {
            room_.leave(shared_from_this());
          }
        });
  }

  tcp::socket socket_;
  gpm_room& room_;
  gpm_message read_msg_;
  gpm_message_queue write_msgs_;
};
