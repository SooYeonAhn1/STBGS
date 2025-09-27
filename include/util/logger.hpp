#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <mutex>

namespace Utility {
    enum class LogLevel { DEBUG, INFO, WARNING, ERROR, FATAL };
    
    class Logger {
    public:
        static void log(LogLevel level, const std::string& message);
    private:
        static std::mutex log_mutex_;
    };
}

#endif // LOGGER_HPP