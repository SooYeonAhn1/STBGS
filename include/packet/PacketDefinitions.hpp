#ifndef PACKETDEFINITIONS_HPP
#define PACKETDEFINITIONS_HPP

#include <cstdint>

namespace PacketDefinitions {

    enum class PacketID : uint32_t {
        // System (0-9)
        PID_SYS_PING = 1,
        PID_SYS_PONG = 2,
        
        // Authentication (10-19)
        PID_AUTH_REGISTER_REQ = 10,
        PID_AUTH_REGISTER_RES = 11,
        PID_AUTH_LOGIN_REQ = 12,
        PID_AUTH_LOGIN_RES = 13,
        
        // Lobby (100-199)
        PID_LOBBY_ROOM_CREATE_REQ = 100,
        PID_LOBBY_ROOM_CREATE_RES = 101,
        PID_LOBBY_ROOM_JOIN_REQ	= 102,
        PID_LOBBY_ROOM_JOIN_RES	= 103,
        PID_LOBBY_ROOM_LEAVE_REQ = 104,
        PID_LOBBY_ROOM_LIST_REQ	= 105,
        PID_LOBBY_ROOM_LIST_RES = 106,
        PID_LOBBY_ROOM_UPDATE_NTF = 107,
        PID_LOBBY_READY_TOGGLE_REQ = 108,
        
        // Game (200-299)
        PID_GAME_START_NTF = 200,
        PID_GAME_MOVE_REQ  = 201,
        PID_GAME_MOVE_RES  = 202,
        PID_GAME_STATE_NTF = 203,
        PID_GAME_TURN_NTF = 204,
        PID_GAME_END_NTF = 205
    };
}
#endif // PACKETDEFINITIONS_HPP