#ifndef BOARDSTATE_HPP
#define BOARDSTATE_HPP

#include <vector>
#include <map>
#include <cstdint>
#include <cstddef>

namespace Game {

constexpr int BOARD_SIZE = 8;
constexpr int NO_OWNER = 0;

struct BoardState {
    std::vector<std::vector<int>> tile_owner;

    std::map<int, int> player_scores;
    int current_turn_player_id;
    int current_player_ap;
    uint32_t total_turns_played;

    void initialize(const std::vector<int>& initial_player_ids) {
        tile_owner.assign(BOARD_SIZE, std::vector<int>(BOARD_SIZE, NO_OWNER));
        player_scores.clear();
        for (int id : initial_player_ids) {
            player_scores[id] = 0;
        }

        current_turn_player_id = NO_OWNER;
        current_player_ap = 0;
        total_turns_played = 0;

        if (!initial_player_ids.empty()) {
            std::vector<std::pair<int, int>> initial_positions = {
                {0, 0},
                {0, BOARD_SIZE - 1},
                {BOARD_SIZE - 1, 0},
                {BOARD_SIZE - 1, BOARD_SIZE - 1}
            };

            for (size_t i = 0; i < initial_player_ids.size() && i < initial_positions.size(); ++i) {
                int player_id = initial_player_ids[i];
                int y = initial_positions[i].first;
                int x = initial_positions[i].second;

                tile_owner[y][x] = player_id;
                player_scores[player_id] = 1;
            }
            current_turn_player_id = initial_player_ids[0];
            current_player_ap = 5;
        }
    }
};

}
#endif // BOARDSTATE_HPP