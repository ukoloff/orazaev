/**
 * Bit sampling for hamming distance.
 *
 * See:
 * http://en.wikipedia.org/wiki/Locality-sensitive_hashing#Bit_sampling_for_Hamming_distance
 */
#pragma once

#include <bitset>
#include <string>
#include <algorithm>
#include <stdexcept>


/**
 * Creates random hash function from family LHS family.
 */
template <size_t DIM, size_t HASH_SIZE>
class BitSamplingHash {
public:
    BitSamplingHash();

    std::bitset<HASH_SIZE> operator()(std::string str) const;

private:
    size_t sample_[HASH_SIZE];

private:
    static std::bitset<DIM*8> toBitSet_(const std::string& str);
};


template <size_t DIM, size_t HASH_SIZE>
BitSamplingHash<DIM, HASH_SIZE>::BitSamplingHash() {
    size_t positions[DIM * 8];

    for (int i = 0; i < DIM * 8; ++i) {
        positions[i] = i;
    }

    std::random_shuffle(positions, positions + DIM*8);

    for (int i = 0; i < HASH_SIZE; ++i) {
        sample_[i] = positions[i];
    }
}


template <size_t DIM, size_t HASH_SIZE>
std::bitset<DIM*8> BitSamplingHash<DIM, HASH_SIZE>::toBitSet_(const std::string& str) {
    if (str.size() != DIM) {
        throw std::runtime_error("toBitSet: Incorrect string length.");
    }

    std::bitset<DIM*8> result;
    int pos = 0;
    for (std::string::const_reverse_iterator c = str.rbegin();
         c != str.rend(); ++c)
    {
        for (int i = 0; i < 8; ++i) {
            result[pos++] = *c >> i & 1;
        }
    }

    return result;
}


template <size_t DIM, size_t HASH_SIZE>
std::bitset<HASH_SIZE> BitSamplingHash<DIM, HASH_SIZE>::operator()(std::string str) const {
    while (str.size() < DIM) str += char(255);
    std::bitset<DIM*8> source = toBitSet_(str);

    std::bitset<HASH_SIZE> result;

    for (int i = 0; i < HASH_SIZE; ++i) {
        result[i] = source[sample_[i]];
    }

    return result;
}
