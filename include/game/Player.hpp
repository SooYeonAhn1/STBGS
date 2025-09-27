#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <cstdint>

struct Player {
    uint64_t userId;
    std::string sessionId;
    bool isReady;
    bool isHost;
    int score;
    int turnOrder;
    bool isDisconnected;
};
#endif // PLAYER_HPP