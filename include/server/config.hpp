#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace Config {
    constexpr unsigned short SERVER_PORT = 8080;
    
    constexpr int MAX_SESSIONS = 5000;
    
    constexpr int IO_CONTEXT_THREADS = 4;

    struct Database {
        static constexpr const char* HOST = "127.0.0.1";
        static constexpr const char* USER = "stbgs_user";
        static constexpr const char* PASS = "SecurePassword123"; // load from external file for deployment
        static constexpr const char* NAME = "stbgs_db";
    };

    constexpr int PLAYERS_PER_GAME = 2; 
    
    constexpr int TURN_TIMEOUT_SECONDS = 30; 

    enum LogLevel { DEBUG, INFO, WARNING, ERROR };
    constexpr LogLevel MIN_LOG_LEVEL = LogLevel::INFO;
}
#endif // CONFIG_HPP