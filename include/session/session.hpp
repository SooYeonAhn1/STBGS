#ifndef SESSION_HPP
#define SESSION_HPP

#include <boost/asio.hpp>
#include <memory>

#include "../game/GameManager.hpp"
#include "../database/DatabaseManager.hpp"

class Session {
public:
    Session(boost::asio::io_context& io_context,
            std::shared_ptr<GameManager> game_manager,
            std::shared_ptr<DatabaseManager> db_manager);
    boost::asio::ip::tcp::socket& socket();
    void start();
private:
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
    void handle_write(const boost::system::error_code& error, size_t bytes_transferred);
    
    boost::asio::ip::tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    std::shared_ptr<GameManager> game_manager_;
    std::shared_ptr<DatabaseManager> db_manager_;
};

#endif // SESSION_HPP