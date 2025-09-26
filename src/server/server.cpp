#include "../../include/server/server.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <boost/bind/bind.hpp>

Server::Server(boost::asio::io_context& io_context, unsigned short port)
    : io_context_(io_context),
      acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      game_manager_(std::make_shared<GameManager>()),
      db_manager_(std::make_shared<DatabaseManager>()) {
    std::cout << "Server initialized on port " << port << std::endl;
}

void Server::start() {
    std::cout << "Server starting..." << std::endl;
    start_accept();
}

void Server::stop() {
    std::cout << "Server stopping..." << std::endl;
    io_context_.stop();
}

void Server::start_accept() {
    std::shared_ptr<Session> new_session = std::make_shared<Session>(io_context_, game_manager_, db_manager_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&Server::handle_accept, this, new_session,
            boost::asio::placeholders::error));
}

void Server::handle_accept(std::shared_ptr<Session> new_session, const boost::system::error_code& error) {
    if (!error) {
        std::cout << "New connection accepted." << std::endl;
        new_session->start();
    } else {
        std::cerr << "Accept error: " << error.message() << std::endl;
    }
    start_accept();
}