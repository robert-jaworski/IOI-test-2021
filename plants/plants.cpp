#include <bits/stdc++.h>
#include "plants.h"

using namespace std;

struct PlantInfo {
    int id = -1;
    bool processed = false;
    vector<PlantInfo*> tallerThan;
    vector<PlantInfo*> smallerThan;
};

vector<PlantInfo> plantHeightTree;

void init(int K, std::vector<int> r) {
    int N = r.size();
    plantHeightTree.resize(N);

    queue<PlantInfo*> process;
    for (int i = 0; i < N; i++) {
        plantHeightTree[i].id = i;
        if (r[i] == 0) {
            plantHeightTree[i].processed = true;
            for (int j = 1; j < K; j++) {
                plantHeightTree[i].tallerThan.push_back(&plantHeightTree[(i + j) % N]);
                plantHeightTree[(i + j) % N].smallerThan.push_back(&plantHeightTree[i]);
            }
            process.push(&plantHeightTree[(N + i - 1) % N]);
        }
        if (r[i] == K - 1) {
            plantHeightTree[i].processed = true;
            for (int j = 1; j < K; j++) {
                plantHeightTree[i].smallerThan.push_back(&plantHeightTree[(i + j) % N]);
                plantHeightTree[(i + j) % N].tallerThan.push_back(&plantHeightTree[i]);
            }
            process.push(&plantHeightTree[(N + i - 1) % N]);
        }
    }

    while (!process.empty()) {
        PlantInfo* curr = process.front();
        process.pop();
        vector<PlantInfo> subtree(K);
    }
}

int compare_plants(int x, int y) {
    return 0;
}
