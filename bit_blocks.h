#pragma once

#include <vector>

template <
	typename TStorage, 
	int blockSize, 
	int elementMask = (1 << blockSize) - 1> 
class BitBlock {
	TStorage storage;
public:
	BitBlock() :storage(0) {
		static_assert(blockSize <= 5 && blockSize >= 1,
			"blockSize must be positive and less or equal to 5");
	}
public:
	int get(int i) const {
		return (int)((storage >> (i * blockSize)) & elementMask);
	}
	void set(int i, int v) {
		storage ^= (TStorage)((v & elementMask) ^ get(i)) << (i * blockSize);
	}
	void exchange(int i, int j) {
		int xi = get(i), xj = get(j);
		set(i, xj);
		set(j, xi);
	}
	TStorage getStorage() const {
		return storage;
	}
};

class Bitmap {
	std::vector<long long> data;
public:
	Bitmap() :data(0) { }
	Bitmap(long long n) :data(n / 64 + (n % 64 != 0), 0) { }

	bool get(long long i) {
		return data[i / 64] & (1LL << (i % 64));
	}

	void set(long long i) {
		data[i / 64] |= (1LL << (i % 64));
	}
};
