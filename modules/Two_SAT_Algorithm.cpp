#include "../include/Two_SAT_Algorithm.h"
#include <iostream>
#include <cstring>

using namespace std;

vector<int> adj[MAX];
vector<int> adjInv[MAX];
bool visited[MAX];
bool visitedInv[MAX];
stack<int> s;
int scc[MAX];
int counter = 1;

void addEdges(int a, int b) {
    adj[a].push_back(b);
}

void addEdgesInverse(int a, int b) {
    adjInv[b].push_back(a);
}

void dfsFirst(int u) {
    if (visited[u]) return;
    visited[u] = true;

    for (int v : adj[u]) dfsFirst(v);
    s.push(u);
}

void dfsSecond(int u) {
    if (visitedInv[u]) return;
    visitedInv[u] = true;

    for (int v : adjInv[u]) dfsSecond(v);
    scc[u] = counter;
}

bool is2Satisfiable(int n, int m, int a[], int b[]) {
    memset(visited, false, sizeof(visited));
    memset(visitedInv, false, sizeof(visitedInv));

    for (int i = 0; i < m; i++) {
        if (a[i] > 0 && b[i] > 0) {
            addEdges(a[i] + n, b[i]);
            addEdgesInverse(a[i] + n, b[i]);
            addEdges(b[i] + n, a[i]);
            addEdgesInverse(b[i] + n, a[i]);
        } else if (a[i] > 0 && b[i] < 0) {
            addEdges(a[i] + n, n - b[i]);
            addEdgesInverse(a[i] + n, n - b[i]);
            addEdges(-b[i], a[i]);
            addEdgesInverse(-b[i], a[i]);
        } else if (a[i] < 0 && b[i] > 0) {
            addEdges(-a[i], b[i]);
            addEdgesInverse(-a[i], b[i]);
            addEdges(b[i] + n, n - a[i]);
            addEdgesInverse(b[i] + n, n - a[i]);
        } else {
            addEdges(-a[i], n - b[i]);
            addEdgesInverse(-a[i], n - b[i]);
            addEdges(-b[i], n - a[i]);
            addEdgesInverse(-b[i], n - a[i]);
        }
    }

    for (int i = 1; i <= 2 * n; i++) {
        if (!visited[i]) dfsFirst(i);
    }

    while (!s.empty()) {
        int v = s.top();
        s.pop();

        if (!visitedInv[v]) {
            dfsSecond(v);
            counter++;
        }
    }

    for (int i = 1; i <= n; i++) {
        if (scc[i] == scc[i + n]) {
            resetGlobals(n);
            return false;
        }
    }
    resetGlobals(n);
    return true;
}

void resetGlobals(int n) {
    for (int i = 0; i <= 2*n; i++) {
        adj[i].clear();
        adjInv[i].clear();
        visited[i] = false;
        visitedInv[i] = false;
        scc[i] = 0;
    }
    while (!s.empty()) s.pop();
    counter = 1;
}
