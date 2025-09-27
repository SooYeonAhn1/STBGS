#include "../../include/packet/Packet.hpp"

namespace Network {

    template <typename T>
    void Packet::write_data(std::vector<uint8_t>& buffer, const T& data) {
        static_assert(std::is_trivially_copyable<T>::value, "T must be a trivially copyable type.");
        const uint8_t* raw_data = reinterpret_cast<const uint8_t*>(&data);
        buffer.insert(buffer.end(), raw_data, raw_data + sizeof(T));
    }

    void Packet::write_string(std::vector<uint8_t>& buffer, const std::string& str) {
        uint32_t length = static_cast<uint32_t>(str.size());
        write_data(buffer, length);
        buffer.insert(buffer.end(), str.begin(), str.end());
    }

    template <typename T>
    bool Packet::read_data(const std::vector<uint8_t>& buffer, size_t& offset, T& data) {
        static_assert(std::is_trivially_copyable<T>::value, "T must be a trivially copyable type.");
        if (!check_read_bounds(buffer.size(), offset, sizeof(T))) {
            return false;
        }
        std::memcpy(&data, buffer.data() + offset, sizeof(T));
        offset += sizeof(T);
        return true;
    }

    bool Packet::read_string(const std::vector<uint8_t>& buffer, size_t& offset, std::string& str) {
        uint32_t length;
        if (!read_data(buffer, offset, length)) {
            return false;
        }
        if (!check_read_bounds(buffer.size(), offset, length)) {
            return false;
        }
        str.assign(reinterpret_cast<const char*>(buffer.data() + offset), length);
        offset += length;
        return true;
    }

    PacketHeader Packet::create_header(PacketDefinitions::PacketID id, uint32_t body_size) {
        PacketHeader header;
        header.size = static_cast<PacketDefinitions::PacketID>(HEADER_SIZE + body_size);
        header.packet_id = static_cast<uint32_t>(id);
        return header;
    }

    bool Packet::extract_header(const std::vector<uint8_t>& buffer, PacketHeader& header) {
        if (buffer.size() < HEADER_SIZE) {
            return false;
        }
        std::memcpy(&header, buffer.data(), HEADER_SIZE);
        return true;
    }

    bool Packet::check_read_bounds(size_t buffer_size, size_t current_offset, size_t requested_size) {
        return (current_offset + requested_size <= buffer_size);
    } 
} // namespace Network