#include <bits/stdc++.h>
#include "plants.h"

using namespace std;

int main() {
    int N, K, Q; cin >> N >> K >> Q;

    vector<int> plants(N, 0);
    for (int i = 0; i < N; i++) {
        cin >> plants[i];
    }

    vector<int> r(N, 0);
    for (int i = 0; i < N; i++) {
        int taller = 0;
        for (int j = 0; j < K; j++) {
            if (plants[(i + j) % N] > plants[i]) taller++;
        }
        r[i] = taller;
    }

    init(N, r);

    for (int i = 0, x, y; i < Q; i++) {
        cin >> x >> y;
        assert(0 <= x && x < N && 0 <= y && y < N);
        int comp = compare_plants(x, y);
        assert(-1 <= comp && comp <= 1);
        switch (comp) {
            case 0:
                cout << x << " ? " << y << " (???)" << endl;
                break;
            case 1:
                cout << x << " > " << y << " (" << (plants[x] > plants[y] ? "CORRECT" : "WRONG") << ')' << endl;
                break;
            case -1:
                cout << x << " < " << y << " (" << (plants[x] < plants[y] ? "CORRECT" : "WRONG") << ')' << endl;
                break;
        }
    }
}
