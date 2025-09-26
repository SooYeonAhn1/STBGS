#include "../../include/session/session.hpp"

#include <iostream>
#include <boost/bind/bind.hpp>

Session::Session(boost::asio::io_context& io_context,
                 std::shared_ptr<GameManager> game_manager,
                 std::shared_ptr<DatabaseManager> db_manager)
    : socket_(io_context),
      game_manager_(game_manager),
      db_manager_(db_manager) {
    std::cout << "Session created." << std::endl;
}
