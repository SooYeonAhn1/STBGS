#ifndef GAMEDAO_HPP
#define GAMEDAO_HPP

#include "./DatabaseManager.hpp"

#include <string>
#include <vector>

namespace DB {

    class GameDAO {
    public:
        
        GameDAO(DatabaseManager& db_manager);
        bool saveGameResult(int winnerId, 
                            const std::vector<int>& loserIds, 
                            int durationSeconds,
                            const std::string& gameDataJson);

    private:
        DatabaseManager& db_manager_;
    };

}

#endif // GAMEDAO_HPP