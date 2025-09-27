#include "../../include/database/UserDAO.hpp"
#include "../include/util/hash.hpp"
#include "../util/logger.hpp"
#include "../util/utils.hpp"

#include <iostream>
#include <vector>
#include <sstream>

using namespace DB;

UserDAO::UserDAO(DatabaseManager& db_manager) : db_manager_(db_manager) {
}
bool UserDAO::createUser(const std::string& username, const std::string& password) {
    if (username.empty() || password.empty()) {
        return false;
    }
    try {
        Hash hashUtil;
        const std::string hashedPassword = std::to_string(hashUtil.compute(password));
    
        const std::string currentTime = Utility::TimeUtil::getCurrentDBTimestamp();

        const std::string sql = 
            "INSERT INTO users (username, password_hash, created_at) VALUES (?, ?, ?)";
        
        std::vector<std::string> params;
        params.push_back(username);
        params.push_back(hashedPassword);
        params.push_back(currentTime);

        int affectedRows = db_manager_.execute_update(sql, params);

        if (affectedRows == 1) {
            Utility::Logger::log(Utility::LogLevel::INFO,"User successfully created: " + username);
            return true;
        } 
        
        return false;

    } catch (const std::exception& e) {
        std::stringstream ss;
        ss << "UserDAO: Failed to create user '" << username << "'. Error: " << e.what();
        Utility::Logger::log(Utility::LogLevel::ERROR, ss.str());
        
        return false; 
    }
}

bool UserDAO::validateUser(const std::string& username, const std::string& password) {
    if (username.empty() || password.empty()) {
        return false;
    }
    try {
        const std::string sql = 
            "SELECT password_hash FROM users WHERE username = ?";
        
        std::vector<std::string> params;
        params.push_back(username);

        QueryResult result = db_manager_.execute_select(sql, params);

        if (result.rows.empty()) {
            return false;
        }

        const std::string storedHashStr = result.rows[0].at("password_hash");
        size_t storedHash = std::stoull(storedHashStr);

        Hash hashUtil;
        if (hashUtil.verify(password, storedHash)) {
            Utility::Logger::log(Utility::LogLevel::INFO,"User validated: " + username);
            return true;
        } else {
            return false;
        }

    } catch (const std::exception& e) {
        std::stringstream ss;
        ss << "UserDAO: Failed to validate user '" << username << "'. Error: " << e.what();
        Utility::Logger::log(Utility::LogLevel::ERROR, ss.str());
        
        return false; 
    }
}

bool UserDAO::deleteUser(const std::string& username) {
    if (username.empty()) {
        return false;
    }
    try {
        const std::string sql = 
            "DELETE FROM users WHERE username = ?";
        
        std::vector<std::string> params;
        params.push_back(username);

        int affectedRows = db_manager_.execute_update(sql, params);

        if (affectedRows == 1) {
            Utility::Logger::log(Utility::LogLevel::INFO,"User successfully deleted: " + username);
            return true;
        } 
        
        return false;

    } catch (const std::exception& e) {
        std::stringstream ss;
        ss << "UserDAO: Failed to delete user '" << username << "'. Error: " << e.what();
        Utility::Logger::log(Utility::LogLevel::ERROR, ss.str());
        
        return false; 
    }
}