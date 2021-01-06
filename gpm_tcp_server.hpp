#ifndef GPM_TCP_SERVER_HPP
#define GPM_TCP_SERVER_HPP

#include <boost/asio.hpp>

#include "gpm_config.hpp"
#include "gpm_logging.hpp"
#include "gpm_session.hpp"
class gpm_tcp_server {
 public:
  gpm_tcp_server(boost::asio::io_context& io_context,
                 const boost::asio::ip::tcp::endpoint& endpoint)
      : acceptor_(io_context, endpoint) {
    do_accept();
  }

 private:
  void do_accept() {
    acceptor_.async_accept([this](boost::system::error_code ec,
                                  boost::asio::ip::tcp::socket socket) {
      if (!ec) {
        logging::DEBUG("Connected from: " +
                       socket.remote_endpoint().address().to_string() + ":" +
                       std::to_string(socket.remote_endpoint().port()));

        // запускаем сессию для ассептед сокет
        std::make_shared<gpm_session>(std::move(socket), room_)->start();
      }

      do_accept();
    });
  }

  boost::asio::ip::tcp::acceptor acceptor_;
  gpm_room room_;
};

#endif  // GPM_TCP_SERVER_HPP
