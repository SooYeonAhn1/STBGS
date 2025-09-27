#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace Config {
    
    // The port to listen to client connections
    constexpr unsigned short SERVER_PORT = 8080;
    
    // The maximum number of concurrent sessions the server should handle
    constexpr int MAX_SESSIONS = 5000;
    
    // The number of threads the io_context will use for concurrent processing
    constexpr int IO_CONTEXT_THREADS = 4;


    // Structure to hold sensitive but necessary DB connection details
    struct Database {
        static constexpr const char* HOST = "127.0.0.1";
        static constexpr const char* USER = "stbgs_user";
        static constexpr const char* PASS = "SecurePassword123"; // NOTE: In production, load this from a file!
        static constexpr const char* NAME = "stbgs_db";
    };

    
    // The number of players required to start a match
    constexpr int PLAYERS_PER_GAME = 2; 
    
    // Maximum time (in seconds) a player has to make a move
    constexpr int TURN_TIMEOUT_SECONDS = 30; 

    // Severity levels for logging
    enum LogLevel { DEBUG, INFO, WARNING, ERROR };
    constexpr LogLevel MIN_LOG_LEVEL = LogLevel::INFO;
}
#endif // CONFIG_HPP