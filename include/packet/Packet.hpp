#ifndef PACKET_HPP
#define PACKET_HPP

#include "./PacketDefinitions.hpp"

#include <cstdint>
#include <vector>
#include <string>
#include <boost/asio/buffer.hpp>

namespace Network {

    struct PacketHeader {
        PacketDefinitions::PacketID size;
        uint32_t packet_id;     
    };

    constexpr size_t HEADER_SIZE = sizeof(PacketHeader);

    class Packet {
    public:
        template <typename T>
        static void write_data(std::vector<uint8_t>& buffer, const T& data);

        static void write_string(std::vector<uint8_t>& buffer, const std::string& str);

        template <typename T>
        static bool read_data(const std::vector<uint8_t>& buffer, size_t& offset, T& data);

        static bool read_string(const std::vector<uint8_t>& buffer, size_t& offset, std::string& str);
    
        static PacketHeader create_header(PacketDefinitions::PacketID id, uint32_t body_size);

        static bool extract_header(const std::vector<uint8_t>& buffer, PacketHeader& header);
        
    private:
        static bool check_read_bounds(size_t buffer_size, size_t current_offset, size_t requested_size);
    };
}
#endif // PACKET_HPP