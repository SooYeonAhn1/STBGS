#ifndef BOARDSTATE_HPP
#define BOARDSTATE_HPP

#include <vector>
#include <map>
#include <cstdint>

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

        // need to set up initial tiles per player for a real game
    }
};

}
#endif // BOARDSTATE_HPP