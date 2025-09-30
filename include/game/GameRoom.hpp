#ifndef GAMEROOM_HPP
#define GAMEROOM_HPP

#include "BoardState.hpp"
#include "../session/session.hpp"

#include <memory>
#include <map>
#include <vector>

class GameRoom {
    public:
        GameRoom(uint32_t id, const std::vector<int>& initial_player_ids);

        bool handleMoveRequest(int playerId, int fromY, int fromX, int toY, int toX);
        bool checkWinCondition() const;
        void endTurn(int playerId);

        uint32_t getRoomId() const { return roomId; }
        const Game::BoardState& getBoardState() const { return boardState; }
        Game::BoardState& getMutableBoardState() { return boardState; }
        
    private:
        uint32_t roomId;
        Game::BoardState boardState;
        std::map<int, std::shared_ptr<GameRoom>> room_;
        std::map<int, std::shared_ptr<Session>> players_;
};
#endif // GAMEROOM_HPP