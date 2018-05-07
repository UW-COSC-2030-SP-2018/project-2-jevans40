#pragma once
#include <vector>
class bloomFilter {
private:
	unsigned char m_numHashes;
	std::vector<bool> m_bits;


public:
	bloomFilter(unsigned long long int size, unsigned char numhashes);

	void add(uint8_t *data, std::size_t len);
	bool possiblyContains(const uint8_t *data, std::size_t len) const;


	std::array<unsigned long long int, 2> hash(const uint8_t *data, std::size_t len) const;
	inline uint64_t nthHash(uint8_t n, uint64_t hashA, uint64_t hashB, uint64_t filterSize) const;

};