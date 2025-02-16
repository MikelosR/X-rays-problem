// CPP program to find Edit Distance (when only two
// operations are allowed, insert and delete) using LCS.
#include<bits/stdc++.h>
using namespace std;

int editDistanceWith2Ops(string &s1, string &s2)
{
    int m = s1.size();
    int n = s2.size();

    // Initializing a matrix of size (m+1)*(n+1)
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    // Building dp[m+1][n+1] in bottom-up fashion
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    int lcs  = dp[m][n];
    cout<<"lcs: "<<lcs<<endl;
    // Edit distance is delete operations + 
    // insert operations.
    return (m - lcs) + (n - lcs);
}

/* Driver program to test above function */
int main()
{
    string X = "AGGTAB", Y = "GXTXAYB";
    //         2    one  4
    cout << editDistanceWith2Ops(X, Y)<<endl;
    return 0;
}
