#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct mert
{
    vector<vector<ll>> tre;
    int n;
    void init(int n)
    {
        this->n = n;
        tre.resize(4 * n + 5);
    }
    ll compare(ll a, ll b)
    {
        return a + b; // Change this to min, max, OR, AND, XOR, or any other operation.
    }

    void build(int nd, int st, int ed, vector<ll> &v)
    {
        if (st == ed)
        {
            tre[nd].push_back(v[st]);
            return;
        }
        int mid = (st + ed) / 2;
        build(2 * nd, st, mid, v);
        build(2 * nd + 1, mid + 1, ed, v);
        int i = 0, j = 0;
        while (i < tre[2 * nd].size() && j < tre[2 * nd + 1].size())
        {
            if (tre[2 * nd][i] <= tre[2 * nd + 1][j])
            {
                tre[nd].push_back(tre[2 * nd][i]);
                i++;
            }
            else
            {
                tre[nd].push_back(tre[2 * nd + 1][j]);
                j++;
            }
        }
        while (i < tre[2 * nd].size())
        {
            tre[nd].push_back(tre[2 * nd][i]);
            i++;
        }
        while (j < tre[2 * nd + 1].size())
        {
            tre[nd].push_back(tre[2 * nd + 1][j]);
            j++;
        }
    }

    void build(vector<ll> &v)
    {
        build(1, 1, n, v);
    }

    ll query(int nd, int st, int ed, int l, int r, ll val)
    {
        if (st > r || ed < l)
        {
            return 0;
        }
        if (st >= l && ed <= r)
        {
            return (upper_bound(tre[nd].begin(), tre[nd].end(), val) - tre[nd].begin());
        }
        int mid = (st + ed) / 2;
        ll leftQuery = query(2 * nd, st, mid, l, r, val);
        ll rightQuery = query(2 * nd + 1, mid + 1, ed, l, r, val);
        return compare(leftQuery, rightQuery);
    }
    ll query(int l, int r, ll val)
    {
        return query(1, 1, n, l, r, val);
    }
};