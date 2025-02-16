#ifndef TWO_SAT_H
#define TWO_SAT_H

#include <vector>
#include <stack>

const int MAX = 100000;

void addEdges(int a, int b);
void addEdgesInverse(int a, int b);
bool is2Satisfiable(int n, int m, int a[], int b[]);
void dfsSecond(int u);
void dfsFirst(int u);
void resetGlobals(int n);

#endif //TWO_SAT_H
