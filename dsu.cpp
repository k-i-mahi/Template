#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct DSU
{
    vector<ll> par, sz;
    int nn;

    DSU(int n) : par(n + 1), sz(n + 1, 1), nn(n)
    {
        for (int i = 1; i <= n; ++i)
        {
            par[i] = i;
        }
    }

    void make(int v)
    {
        par[v] = v;
        sz[v] = 1;
    }
    ll find(int v)
    {
        if (par[v] == v)
        {
            return v;
        }
        return par[v] = find(par[v]);
    }

    bool same(int u, int v)
    {
        return find(u) == find(v);
    }
    bool merge(int u, int v)
    {
        u = find(u);
        v = find(v);
        if (u == v)
        {
            return 0;
        }
        nn--;

        if (sz[u] < sz[v])
        {
            swap(u, v);
        }
        par[v] = u;
        sz[u] += sz[v];
        return 1;
    }
    int get_size(int v)
    {
        return sz[find(v)];
    }
    int count()
    {
        return nn;
    }
};

struct DSUR
{
    vector<ll> par, rank;
    int nn;
    DSUR(int n) : par(n + 1), rank(n + 1, 0), nn(n)
    {
        for (int i = 1; i <= n; ++i)
        {
            par[i] = i;
        }
    }
    void make(int v)
    {
        par[v] = v;
        rank[v] = 0;
    }
    ll find(int v)
    {
        if (par[v] == v)
        {
            return v;
        }
        return par[v] = find(par[v]);
    }
    bool same(int u, int v)
    {
        return find(u) == find(v);
    }
    void merge(int u, int v)
    {
        u = find(u);
        v = find(v);
        if (u == v)
        {
            return;
        }
        nn--;
        if (rank[u] < rank[v])
        {
            swap(u, v);
        }
        par[v] = u;
        if (rank[u] == rank[v])
        {
            rank[u]++;
        }
    }
    int count()
    {
        return nn;
    }
};
