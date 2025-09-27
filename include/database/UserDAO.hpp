#ifndef USERDAO_HPP
#define USERDAO_HPP

#include "./DatabaseManager.hpp"

#include <string>
#include <memory>

namespace DB {
    class UserDAO {
        public:
            UserDAO(DatabaseManager& db_manager);
            bool createUser(const std::string& username, const std::string& password);
            bool validateUser(const std::string& username, const std::string& password);
            bool deleteUser(const std::string& username);
        private:
            DatabaseManager& db_manager_;
    };
}
#endif // USERDAO_HPP