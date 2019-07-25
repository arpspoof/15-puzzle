#include "index.h"
#include "puzzle.h"
#include "dpd.h"

#include <vector>
#include <bitset>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

using namespace std;

char actionList[] = { 'u', 'd', 'l', 'r' };

static long long searchSize;
static long long searchProgress;

bool DisjointPatternDB::checkMove(int pos, char dir) const {
	switch (dir) {
	case 'u':
		return (pos / n != 0);
	case 'd':
		return (pos / n != n - 1);
	case 'l':
		return (pos % n != 0);
	case 'r':
		return (pos % n != n - 1);
	default:
		assert(false);
		return false;
	}
}

PuzzleState DisjointPatternDB::moveAndCopy(PuzzleState s, int pos, char dir) const {
	PuzzleState res(s);
	switch (dir) {
	case 'u':
		res.exchange(pos, pos - n);
		break;
	case 'd':
		res.exchange(pos, pos + n);
		break;
	case 'l':
		res.exchange(pos, pos - 1);
		break;
	case 'r':
		res.exchange(pos, pos + 1);
		break;
	default:
		assert(false);
	}
	return res;
}

string DisjointPatternDB::getFilePathById(int Id) const {
	return "tmp/dpd_worker_" + to_string(Id) + string(".tmp.db");
}

unsigned int DisjointPatternDB::getSearchStateIndex(PuzzleState state) const {
	int emptyTileIndex = 0;
	for (int i = 0; i < nn; i++) {
		int x = state.get(i);
		if (x == emptyTile) break;
		else if (x == dummyTile) emptyTileIndex++;
	}
	return getStateIndex(state) * (nn - r) + emptyTileIndex;
}

void DisjointPatternDB::bfs_thread(int fileId, int dataId) {
	vector<pair<PuzzleState, unsigned char>> &data = dataArray[dataId];
	vector<pair<PuzzleState, unsigned char>> nextdata;

	for (size_t i = 0; i < data.size(); i++) {
		auto &p = data[i];
		auto state = p.first;
		auto cost = p.second;
		for (int i = 0; i < nn; i++) {
			if (state.get(i) != emptyTile) continue;
			for (char action : actionList) {
				if (!checkMove(i, action)) continue;
				PuzzleState newState = moveAndCopy(state, i, action);
				unsigned char newCost = (newState.get(i) != dummyTile) + cost;
				unsigned char oldCost = db[getStateIndex(newState)];
				db[getStateIndex(newState)] = min(oldCost, newCost);
				unsigned int newSearchIndex = getSearchStateIndex(newState);
				if (!searchHash.get(newSearchIndex)) {
					searchHash.set(newSearchIndex);
					auto newp = make_pair(newState, newCost);
					if (cost == newCost) {
						data.push_back(newp);
					}
					else {
						nextdata.push_back(newp);
					}

					searchProgress++;
					if (searchProgress % 10000000 == 0) {
						printf("compute db process %f%%\n", (double)searchProgress / searchSize * 100.0);
					}
				}
			}
		}
	}

	int sizePair = sizeof(pair<PuzzleState, unsigned char>);
	ofstream o(getFilePathById(fileId), ios::app | ios::out | ios::binary);
	o.write((const char *)nextdata.data(), sizePair * nextdata.size());
	o.close();
}

void DisjointPatternDB::compute(int nThreads, int batchSize) {
	namespace fs = boost::filesystem;
	fs::path path_to_remove("./tmp");
	for (fs::directory_iterator end_dir_it, it(path_to_remove); it != end_dir_it; ++it) {
		fs::remove_all(it->path());
	}
	
	searchProgress = nn - r;
	searchSize = db.size() * (nn - r);
	searchHash = Bitmap(searchSize);

	PuzzleState rootCommon;
	for (int i = 0; i < nn; i++) {
		if (tileGroupMap[i] == emptyTile || i == emptyTile) {
			rootCommon.set(i, dummyTile);
		}
		else {
			rootCommon.set(i, i);
		}
	}

	db[getStateIndex(rootCommon)] = 0;
	
	vector<pair<PuzzleState, unsigned char>> roots;
	for (int i = 0; i < nn; i++) {
		if (tileGroupMap[i] == emptyTile || i == emptyTile) {
			PuzzleState copy(rootCommon);
			copy.set(i, emptyTile);
			roots.push_back(make_pair(copy, 0));
			searchHash.set(getSearchStateIndex(copy));
		}
	}

	int sizePair = sizeof(pair<PuzzleState, unsigned char>);

	ofstream rootOutput(getFilePathById(0), ios::out | ios::binary);
	rootOutput.write((const char *)roots.data(), sizePair * roots.size());
	rootOutput.close();

	int fileId = 0;

	vector<boost::thread *> workers(nThreads);
	dataArray = vector<vector<pair<PuzzleState, unsigned char>>>(nThreads);

	while (true) {
		bool processed = false;

		for (int t = 0; t < nThreads; t++) {
			ifstream in(getFilePathById(t + fileId), ios::in | ios::binary);

			in.seekg(0, ios::end);
			long long fileSize = in.tellg();

			if (fileSize <= 0) {
				in.close();
				continue;
			}

			assert((fileSize % sizePair) == 0);

			processed = true;
			in.seekg(ios::beg);

			long long entries = fileSize / sizePair;
			long long batches = entries / batchSize + ((entries % batchSize) != 0);

			while (true) {
				if (batches == 0) break;

				int nWorkers = 0;
				for (int i = 0; i < nThreads; i++) {
					if (batches == 0) break;
					batches--;

					nWorkers++;
					int elements = (batches == 0) ? (entries % batchSize) : batchSize;
					dataArray[i] = vector<pair<PuzzleState, unsigned char>>(elements);
					in.read((char *)dataArray[i].data(), elements * sizePair);

					workers[i] = new boost::thread{ [=]() { bfs_thread(fileId + nThreads + i, i); } };
				}

				for (int i = 0; i < nWorkers; i++) {
					workers[i]->join();
				}
				for (int i = 0; i < nWorkers; i++) {
					delete workers[i];
				}
			}

			in.close();
		}

		fileId += nThreads;

		if (!processed) break;
	}
}
