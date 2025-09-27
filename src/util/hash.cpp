#include "../../include/util/hash.hpp"

size_t Hash::compute(const std::string& input) {
    std::hash<std::string> hasher;
    return hasher(input);
}

bool Hash::verify(const std::string& input, size_t hash) const {
    return compute(input) == hash;
}