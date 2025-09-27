#include "../../include/util/logger.hpp"

#include <iostream>

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(log_mutex_);
    std::string prefix;
    switch (level) {
        case LogLevel::DEBUG: prefix = "[DEBUG] "; break;
        case LogLevel::INFO: prefix = "[INFO] "; break;
        case LogLevel::WARNING: prefix = "[WARNING] "; break;
        case LogLevel::ERROR: prefix = "[ERROR] "; break;
        case LogLevel::FATAL: prefix = "[FATAL] "; break;
    }
    std::cout << prefix << message << std::endl;
    if (level == LogLevel::FATAL) {
        std::cerr << "A fatal error occurred. Exiting." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}   