#include "../../include/game/GameRoom.hpp"

GameRoom::GameRoom(uint32_t id, const std::vector<int>& initial_player_ids) : roomId(id) {
    boardState.initialize(initial_player_ids);
}

bool GameRoom::checkWinCondition() const {
    for (const auto& row : boardState.tile_owner) {
        for (int owner : row) {
            if (owner == Game::NO_OWNER) {
                return false;
            }
        }
    }
    return true;
}

bool GameRoom::handleMoveRequest(int playerId, int fromY, int fromX, int toY, int toX) {
    if (playerId != boardState.current_turn_player_id) {
        return false;
    }

    if (fromY < 0 || fromY >= Game::BOARD_SIZE || fromX < 0 || fromX >= Game::BOARD_SIZE ||
        toY < 0 || toY >= Game::BOARD_SIZE || toX < 0 || toX >= Game::BOARD_SIZE) {
        return false;
    }

    if (boardState.tile_owner[fromY][fromX] != playerId) {
        return false;
    }

    if (boardState.tile_owner[toY][toX] != Game::NO_OWNER) {
        return false;
    }

    int distance = abs(toY - fromY) + abs(toX - fromX);
    if (distance > 2) {
        return false;
    }

    int apCost = (distance == 1) ? 1 : 2;
    if (boardState.current_player_ap < apCost) {
        return false;
    }

    boardState.tile_owner[toY][toX] = playerId;
    boardState.current_player_ap -= apCost;

    boardState.player_scores[playerId] += 1;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int ny = toY + dy;
            int nx = toX + dx;
            if (ny >= 0 && ny < Game::BOARD_SIZE && nx >= 0 && nx < Game::BOARD_SIZE) {
                if (boardState.tile_owner[ny][nx] != Game::NO_OWNER && boardState.tile_owner[ny][nx] != playerId) {
                    int prevOwner = boardState.tile_owner[ny][nx];
                    boardState.tile_owner[ny][nx] = playerId;
                    boardState.player_scores[playerId] += 1;
                    boardState.player_scores[prevOwner] -= 1;
                }
            }
        }
    }

    return true;
}

void GameRoom::endTurn(int playerId) {
    if (playerId != boardState.current_turn_player_id) {
        return;
    }

    auto it = boardState.player_scores.find(playerId);
    if (it != boardState.player_scores.end()) {
        it->second += boardState.current_player_ap;
    }

    std::vector<int> playerIds;
    for (const auto& entry : boardState.player_scores) {
        playerIds.push_back(entry.first);
    }

    auto itCurrent = std::find(playerIds.begin(), playerIds.end(), playerId);
    if (itCurrent != playerIds.end()) {
        size_t currentIndex = std::distance(playerIds.begin(), itCurrent);
        size_t nextIndex = (currentIndex + 1) % playerIds.size();
        boardState.current_turn_player_id = playerIds[nextIndex];
    }

    boardState.current_player_ap = 5;
    boardState.total_turns_played += 1;
}