#include "../../include/database/GameDAO.hpp"
#include "../../include/util/logger.hpp"

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>

using namespace DB;

GameDAO::GameDAO(DatabaseManager& db_manager)
    : db_manager_(db_manager) {
}

bool GameDAO::saveGameResult(int winnerId, 
                            const std::vector<int>& loserIds, 
                            int durationSeconds,
                            const std::string& gameDataJson) {
    auto transaction_logic = 
        [&](std::shared_ptr<sql::Connection> conn) -> bool 
    {
        try {
            std::unique_ptr<sql::PreparedStatement> pstmt_history(
                conn->prepareStatement(
                    "INSERT INTO game_history (winner_id, start_time, duration_sec, game_data) VALUES (?, NOW(), ?, ?)"
                )
            );
            pstmt_history->setInt(1, winnerId);
            pstmt_history->setInt(2, durationSeconds);
            pstmt_history->setString(3, gameDataJson);
            pstmt_history->execute();

            std::unique_ptr<sql::PreparedStatement> pstmt_winner(
                conn->prepareStatement(
                    "UPDATE users SET total_wins = total_wins + 1 WHERE user_id = ?"
                )
            );
            pstmt_winner->setInt(1, winnerId);
            pstmt_winner->execute();

            std::unique_ptr<sql::PreparedStatement> pstmt_loser(
                conn->prepareStatement(
                    "UPDATE users SET total_losses = total_losses + 1 WHERE user_id = ?"
                )
            );
            
            for (int loserId : loserIds) {
                pstmt_loser->setInt(1, loserId);
                pstmt_loser->execute();
            }

            return true; 
            
        } catch (sql::SQLException &e) {
            Utility::Logger::log(Utility::LogLevel::ERROR,"GameDAO Transaction FAILED: " + std::string(e.what()));
            return false; 
        }
    };
    bool result = db_manager_.execute_transaction(transaction_logic);

    if (result) {
        Utility::Logger::log(Utility::LogLevel::INFO,"Game result successfully saved (Winner ID: " + std::to_string(winnerId) + ")");
    }

    return result;
}