#include "../../include/server/server.hpp"
#include "../../include/server/config.hpp"
#include "../../include/util/logger.hpp"
#include <iostream>
#include <vector>
#include <thread>

using namespace Config;
using namespace boost::asio;
using namespace boost::asio::ip;

int main() {
    Utility::Logger::log(Utility::LogLevel::INFO, "Starting Simple Turn-based Board Game Server...");
    
    io_context io_context(IO_CONTEXT_THREADS);
    
    try {
        Server server(io_context, SERVER_PORT);
        
        
        server.start();

        std::vector<std::thread> thread_pool;
        for (int i = 0; i < IO_CONTEXT_THREADS; ++i) {
            thread_pool.emplace_back([&io_context]() {
                io_context.run();
            });
        }
        
        Utility::Logger::log(Utility::LogLevel::INFO,"Server is running on " + std::to_string(SERVER_PORT) + 
                     " using " + std::to_string(IO_CONTEXT_THREADS) + " threads.");

        for (auto& t : thread_pool) {
            if (t.joinable()) {
                t.join();
            }
        }

    } catch (const std::exception& e) {       
        Utility::Logger::log(Utility::LogLevel::FATAL, "Fatal exception during startup: " + std::string(e.what()));
        return EXIT_FAILURE;
    }

    Utility::Logger::log(Utility::LogLevel::INFO, "Server shutdown complete.");
    return EXIT_SUCCESS;
}