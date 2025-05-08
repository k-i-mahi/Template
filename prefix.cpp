#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
vector<vector<ll>> buildPrefixSum(const vector<vector<ll>> &matrix)
{
    int n = matrix.size();
    int m = matrix[0].size();
    vector<vector<ll>> prefix(n, vector<ll>(m, 0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            prefix[i][j] = matrix[i][j];

            if (i > 0)
                prefix[i][j] += prefix[i - 1][j];
            if (j > 0)
                prefix[i][j] += prefix[i][j - 1];
            if (i > 0 && j > 0)
                prefix[i][j] -= prefix[i - 1][j - 1];
        }
    }

    return prefix;
}

ll getSum(const vector<vector<ll>> &prefix, int x1, int y1, int x2, int y2)
{
    ll sum = prefix[x2][y2];

    if (x1 > 0)
        sum -= prefix[x1 - 1][y2];
    if (y1 > 0)
        sum -= prefix[x2][y1 - 1];
    if (x1 > 0 && y1 > 0)
        sum += prefix[x1 - 1][y1 - 1];

    return sum;
}
int main()
{
    vector<vector<ll>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};

    // Build the prefix sum matrix
    vector<vector<ll>> prefix = buildPrefixSum(matrix);

    // Query the sum of a submatrix from (0, 0) to (2, 2)
    ll sum = getSum(prefix, 0, 0, 2, 2); // This should return the sum of the entire matrix

    cout << "Sum of submatrix from (0, 0) to (2, 2): " << sum << endl;

    // Example: Query the sum of a submatrix from (1, 1) to (2, 2)
    sum = getSum(prefix, 1, 1, 2, 2); // This should return 28 (5 + 6 + 8 + 9)
    cout << "Sum of submatrix from (1, 1) to (2, 2): " << sum << endl;

    return 0;
}

vector<vector<ll>> buildPrefixSum(int n, int m, const vector<vector<ll>> arr)
{
    vector<vector<ll>> prefix(n + 1, vector<ll>(m + 1, 0));

    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            prefix[i][j] = arr[i][j] + prefix[i - 1][j] + prefix[i][j - 1] - prefix[i - 1][j - 1];
        }
    }

    return prefix;
}

// Function to get the sum of elements in the sub-matrix from (x1, y1) to (x2, y2).
// (1,1) is the top-left corner of the matrix.
int query(const vector<vector<ll>> &prefix, int x1, int y1, int x2, int y2)
{
    return prefix[x2][y2] - prefix[x1 - 1][y2] - prefix[x2][y1 - 1] + prefix[x1 - 1][y1 - 1];
}

int main()
{
    int n, m;
    cin >> n >> m;

    // Input matrix (1-based).
    vector<vector<ll>> arr(n + 1, vector<ll>(m + 1, 0));

    // Input the array values.
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            cin >> arr[i][j];
        }
    }

    // Build the prefix sum matrix.
    vector<vector<ll>> prefix = buildPrefixSum(n, m, arr);

    // Example query: Sum of elements from (x1, y1) to (x2, y2).
    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    cout << "Sum from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ") is: "
         << query(prefix, x1, y1, x2, y2) << endl;

    return 0;
}
