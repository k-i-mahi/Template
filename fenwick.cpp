#include <bits/stdc++.h>
using namespace std;

struct FenwickTree
{
    vector<int> tree;
    int n;
    FenwickTree(int n)
    {
        this->n = n;
        tree.resize(n + 1, 0);
    }
    void update(int idx, int val)
    {
        while (idx <= n)
        {
            tree[idx] += val;
            idx += idx & (-idx);
        }
    }
    int query(int idx)
    {
        int sum = 0;
        while (idx > 0)
        {
            sum += tree[idx];
            idx -= idx & (-idx);
        }
        return sum;
    }
    int rangeQuery(int l, int r)
    {
        return query(r) - query(l - 1);
    }
    int inversion_count(vector<int> &arr)
    {
        int icount = 0;
        for (int i = n; i > 0; i--)
        {
            int smallerCount = query(arr[i] - 1);
            icount += smallerCount;
            update(arr[i], 1);
        }
        return icount;
    }
};

int main()
{
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }
    vector<int> sorted_arr = arr;
    sort(sorted_arr.begin(), sorted_arr.end());
    sorted_arr.erase(unique(sorted_arr.begin(), sorted_arr.end()), sorted_arr.end());
    map<int, int> compress;
    for (int i = 0; i < sorted_arr.size(); i++)
    {
        compress[sorted_arr[i]] = i + 1;
    }
    vector<int> v(n + 1);
    for (int i = 0; i < n; i++)
    {
        v[i + 1] = compress[arr[i]];
    }
    int maxVal = *max_element(v.begin(), v.end());
    FenwickTree fenwick(maxVal);
    int inversionCount = fenwick.inversion_count(arr);

    cout << "Number of inversions: " << inversionCount << endl;

    return 0;
}
