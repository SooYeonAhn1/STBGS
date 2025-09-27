#ifndef HASH_HPP
#define HASH_HPP

#include <string>
#include <cstddef>

class Hash {
public:
    static size_t compute(const std::string& input);
    bool verify(const std::string& input, size_t hash) const;
};
#endif // HASH_HPP