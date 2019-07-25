#pragma once

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
	BitBlock(TStorage storage) :storage(storage) {
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
