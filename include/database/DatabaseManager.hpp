#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>

namespace sql {
    class Connection;
    class Statement;
    class PreparedStatement;
    class ResultSet;
}

namespace DB {

    struct QueryResult {
        bool success = false;
        std::vector<std::unordered_map<std::string, std::string>> rows;
    };

    class DatabaseManager {
        public:
            bool initialize_pool(const std::string& host, const std::string& user, 
                                const std::string& password, const std::string& db_name, 
                                int pool_size = 5);


            void shutdown_pool();
            
            QueryResult execute_select(const std::string& sql_query, 
                                    const std::vector<std::string>& params);

            int execute_update(const std::string& sql_query, 
                            const std::vector<std::string>& params);
                            
            bool execute_transaction(std::function<bool(std::shared_ptr<sql::Connection>)> transaction_logic);


        private:
            std::shared_ptr<sql::Connection> get_connection();
            
            void release_connection(std::shared_ptr<sql::Connection> conn);

            std::vector<std::shared_ptr<sql::Connection>> connection_pool_;
            
            std::mutex pool_mutex_;
            
            std::string host_;
            std::string user_;
            std::string password_;
            std::string db_name_;
    };

}

#endif // DATABASEMANAGER_HPP