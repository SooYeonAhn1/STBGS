#include "../../include/database/DatabaseManager.hpp"

#include <iostream>
#include <stdexcept>
#include <mutex>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

using namespace DB;

bool DatabaseManager::initialize_pool(const std::string& host, const std::string& user, 
                                      const std::string& password, const std::string& db_name, 
                                      int pool_size) {
    std::lock_guard<std::mutex> lock(pool_mutex_);
    host_ = host;
    user_ = user;
    password_ = password;
    db_name_ = db_name;

    try {
        sql::Driver* driver = get_driver_instance();
        for (int i = 0; i < pool_size; ++i) {
            std::shared_ptr<sql::Connection> conn(driver->connect(host_, user_, password_));
            conn->setSchema(db_name_);
            connection_pool_.push_back(conn);
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "Error initializing connection pool: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void DatabaseManager::shutdown_pool() {
    std::lock_guard<std::mutex> lock(pool_mutex_);
    connection_pool_.clear();
}

std::shared_ptr<sql::Connection> DatabaseManager::get_connection() {
    std::lock_guard<std::mutex> lock(pool_mutex_);
    if (connection_pool_.empty()) {
        throw std::runtime_error("No available connections in the pool");
    }
    auto conn = connection_pool_.back();
    connection_pool_.pop_back();
    return conn;
}

void DatabaseManager::release_connection(std::shared_ptr<sql::Connection> conn) {
    std::lock_guard<std::mutex> lock(pool_mutex_);
    connection_pool_.push_back(conn);
}

QueryResult DatabaseManager::execute_select(const std::string& sql_query, 
                                            const std::vector<std::string>& params) {
    QueryResult result;
    try {
        auto conn = get_connection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(sql_query));
        for (size_t i = 0; i < params.size(); ++i) {
            pstmt->setString(static_cast<int>(i + 1), params[i]);
        }
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while (res->next()) {
            std::unordered_map<std::string, std::string> row;
            for (int i = 1; i <= res->getMetaData()->getColumnCount(); ++i) {
                std::string colName = res->getMetaData()->getColumnName(i);
                row[colName] = res->getString(i);
            }
            result.rows.push_back(row);
        }
        result.success = true;
        release_connection(conn);
    } catch (const sql::SQLException& e) {
        std::cerr << "Error executing select: " << e.what() << std::endl;
        result.success = false;
    }
    return result;
}

int DatabaseManager::execute_update(const std::string& sql_query, 
                                    const std::vector<std::string>& params) {
    int affected_rows = 0;
    try {
        auto conn = get_connection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(sql_query));
        for (size_t i = 0; i < params.size(); ++i) {
            pstmt->setString(static_cast<int>(i + 1), params[i]);
        }
        affected_rows = pstmt->executeUpdate();
        release_connection(conn);
    } catch (const sql::SQLException& e) {
        std::cerr << "Error executing update: " << e.what() << std::endl;
        affected_rows = -1;
    }
    return affected_rows;
}

bool DatabaseManager::execute_transaction(std::function<bool(std::shared_ptr<sql::Connection>)> transaction_logic) {
    bool success = false;
    try {
        auto conn = get_connection();
        conn->setAutoCommit(false);
        success = transaction_logic(conn);
        if (success) {
            conn->commit();
        } else {
            conn->rollback();
        }
        conn->setAutoCommit(true);
        release_connection(conn);
    } catch (const sql::SQLException& e) {
        std::cerr << "Error executing transaction: " << e.what() << std::endl;
        success = false;
    }
    return success;
}