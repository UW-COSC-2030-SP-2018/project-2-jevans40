//Using the hash function that he used, because it specifically sais its in the public domain, and I trust it
//also the bloom filter code from that website
#include "BloomFilter.h"
#include "MurmurHash3.h"
#include <array>

bloomFilter::bloomFilter(unsigned long long int size, unsigned char numhashes) : m_bits(size), m_numHashes(numhashes) {}


std::array<uint64_t, 2> bloomFilter::hash(const uint8_t *data, std::size_t len) const {
	std::array<uint64_t, 2> hashValue;
	MurmurHash3_x64_128(data, len, 0, hashValue.data());

	return hashValue;
}

inline uint64_t bloomFilter::nthHash(uint8_t n, uint64_t hashA, uint64_t hashB, uint64_t filterSize) const {
	return (hashA + n * hashB) % filterSize;
}

void bloomFilter::add(uint8_t *data, std::size_t len) {
	auto hashValues = hash(data, len);

	for (int n = 0; n < m_numHashes; n++) {
		m_bits[nthHash(n, hashValues[0], hashValues[1], m_bits.size())] = true;
	}
}

bool bloomFilter::possiblyContains(const uint8_t *data, std::size_t len) const {
	auto hashValues = hash(data, len);

	for (int n = 0; n < m_numHashes; n++) {
		if (!m_bits[nthHash(n, hashValues[0], hashValues[1], m_bits.size())]) {
			return false;
		}
	}

	return true;
}