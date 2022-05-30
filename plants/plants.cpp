#include <bits/stdc++.h>
#include "plants.h"

using namespace std;

inline bool cyclic_interval(int a, int b, int x) {
    if (a <= b) return a <= x && x <= b;
    return x <= b || x >= a;
}

struct PlantInfo {
    int id = -1;
    bool processed = false;
    int unprocessedTaller = 0;
    int unprocessedSmaller = 0;
    int visited = 0;
    vector<PlantInfo*> tallerThan;
    vector<PlantInfo*> smallerThan;
};

vector<PlantInfo> plants;
int N, K;

void prepare(queue<PlantInfo*>& process, const vector<int>& r) {
    for (int i = 0; i < N; i++) {
        plants[i].id = i;
        if (r[i] == 0) {
            plants[i].processed = true;
            for (int j = 1; j < K; j++) {
                plants[i].tallerThan.push_back(&plants[(i + j) % N]);
                plants[(i + j) % N].smallerThan.push_back(&plants[i]);
            }
            process.push(&plants[(N + i - 1) % N]);
        }
        if (r[i] == K - 1) {
            plants[i].processed = true;
            for (int j = 1; j < K; j++) {
                plants[i].smallerThan.push_back(&plants[(i + j) % N]);
                plants[(i + j) % N].tallerThan.push_back(&plants[i]);
            }
            process.push(&plants[(N + i - 1) % N]);
        }
    }
}

void create_subtree(PlantInfo* curr, vector<PlantInfo>& subtree) {
    int a = (curr->id + 1) % N;
    int b = (curr->id + K - 1) % N;
    for (int i = 1; i < K; i++) {
        PlantInfo& add = plants[(curr->id + i) % N];
        subtree[i - 1].id = add.id;
        for (PlantInfo* p : add.tallerThan) {
            if (cyclic_interval(a, b, p->id)) {
                subtree[i - 1].tallerThan.push_back(&subtree[(N + p->id - curr->id - 1) % N]);
            }
        }
        for (PlantInfo* p : add.smallerThan) {
            if (cyclic_interval(a, b, p->id)) {
                subtree[i - 1].smallerThan.push_back(&subtree[(N + p->id - curr->id - 1) % N]);
            }
        }
    }
}

void split_subtree(vector<PlantInfo>& subtree, vector<PlantInfo*>& tall, vector<PlantInfo*>& middle, vector<PlantInfo*>& small) {
    vector<PlantInfo*> tallest;
    vector<PlantInfo*> smallest;
    for (PlantInfo& p : subtree) {
        p.unprocessedSmaller = p.smallerThan.size();
        p.unprocessedTaller = p.tallerThan.size();
        if (p.smallerThan.size() == 0) {
            tallest.push_back(&p);
        }
        if (p.tallerThan.size() == 0) {
            smallest.push_back(&p);
        }
    }
    while (tallest.size() == 1) {
        PlantInfo* t = tallest[0];
        tallest.clear();
        t->processed = true;
        tall.push_back(t);
        for (PlantInfo* p : t->tallerThan) {
            if (--p->unprocessedTaller == 0) {
                tallest.push_back(p);
            }
        }
    }
    while (smallest.size() == 1) {
        PlantInfo* t = smallest[0];
        smallest.clear();
        t->processed = true;
        tall.push_back(t);
        for (PlantInfo* p : t->smallerThan) {
            if (--p->unprocessedSmaller == 0) {
                smallest.push_back(p);
            }
        }
    }
    for (PlantInfo& p : subtree) {
        if (!p.processed) middle.push_back(&p);
    }
    return;
}

void insert(PlantInfo* curr, int r, const vector<PlantInfo*>& tall, const vector<PlantInfo*>& middle, const vector<PlantInfo*>& small, bool recurse = true) {
    if (r == 0) {
        curr->tallerThan.push_back(&plants[tall[0]->id]);
        plants[tall[0]->id].smallerThan.push_back(curr);
        return;
    }
    if (r < tall.size()) {
        curr->tallerThan.push_back(&plants[tall[r]->id]);
        plants[tall[r]->id].smallerThan.push_back(curr);
        curr->smallerThan.push_back(&plants[tall[r - 1]->id]);
        plants[tall[r - 1]->id].tallerThan.push_back(curr);
        return;
    }
    if (r == tall.size()) {
        curr->smallerThan.push_back(&plants[tall.back()->id]);
        plants[tall.back()->id].tallerThan.push_back(curr);
        for (PlantInfo* p : middle) {
            curr->tallerThan.push_back(&plants[p->id]);
            plants[p->id].smallerThan.push_back(curr);
        }
        return;
    }
    if (recurse) {
        insert(curr, K - r - 1, small, middle, tall, false);
        return;
    }
    curr->tallerThan.push_back(&plants[small.back()->id]);
    plants[small.back()->id].smallerThan.push_back(curr);
    curr->smallerThan.push_back(&plants[tall.back()->id]);
    plants[tall.back()->id].tallerThan.push_back(curr);
}

void init(int k, vector<int> r) {
    N = r.size();
    plants.resize(N);
    K = k;

    queue<PlantInfo*> process;
    prepare(process, r);

    while (!process.empty()) {
        PlantInfo* curr = process.front();
        process.pop();
        if (curr->processed) continue;
        vector<PlantInfo> subtree(K - 1);
        create_subtree(curr, subtree);

        vector<PlantInfo*> tall;
        vector<PlantInfo*> middle;
        vector<PlantInfo*> small;
        split_subtree(subtree, tall, middle, small);

        insert(curr, r[curr->id], tall, middle, small);

        curr->processed = true;
        process.push(&plants[(N + curr->id - 1) % N]);
    }
}

int compare_plants(int x, int y) {
    static int visits = 0;

    plants[x].visited = ++visits;
    queue<PlantInfo*> bfsQueue;
    for (PlantInfo* p : plants[x].tallerThan) {
        bfsQueue.push(p);
    }
    while (!bfsQueue.empty()) {
        PlantInfo* curr = bfsQueue.front();
        bfsQueue.pop();
        if (curr->id == y) return 1;
        if (curr->visited >= visits) continue;
        curr->visited = visits;
        for (PlantInfo* p : curr->tallerThan) {
            bfsQueue.push(p);
        }
    }

    plants[x].visited = ++visits;
    for (PlantInfo* p : plants[x].smallerThan) {
        bfsQueue.push(p);
    }
    while (!bfsQueue.empty()) {
        PlantInfo* curr = bfsQueue.front();
        bfsQueue.pop();
        if (curr->id == y) return -1;
        if (curr->visited >= visits) continue;
        curr->visited = visits;
        for (PlantInfo* p : curr->smallerThan) {
            bfsQueue.push(p);
        }
    }

    return 0;
}
