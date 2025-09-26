#ifndef SERVER_HPP
#define SERVER_HPP

#include "../session/session.hpp"
#include "../game/GameManager.hpp"
#include "../database/DatabaseManager.hpp"

#include <boost/asio.hpp>
#include <memory>

class Server {
public:
    Server(boost::asio::io_context& io_context, unsigned short port);
    void start();
    void stop();

private:
    void start_accept();
    void handle_accept(std::shared_ptr<Session> new_session, const boost::system::error_code& error);

    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    
    std::shared_ptr<GameManager> game_manager_;
    std::shared_ptr<DatabaseManager> db_manager_;
};

#endif // SERVER_HPP