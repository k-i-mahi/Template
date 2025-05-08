#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct DynamicConvexHull
{
    struct Line
    {
        mutable ll m, b, p;
        bool operator<(const Line &o) const { return m < o.m; }
        bool operator<(ll x) const { return p < x; }
    };

    multiset<Line, less<>> hull;
    const ll INF = 1e18;

    ll div(ll a, ll b)
    {
        return a / b - ((a ^ b) < 0 && a % b);
    }

    ll intersect(const Line &a, const Line &b)
    {
        if (a.m == b.m)
            return a.b > b.b ? INF : -INF;
        return div(b.b - a.b, a.m - b.m);
    }

    bool isect(auto a, auto b)
    {
        if (b == hull.end())
        {
            a->p = INF;
            return false;
        }
        a->p = intersect(*a, *b);
        return a->p >= b->p;
    }

    void add(ll m, ll b)
    {
        auto z = hull.insert({m, b, 0});
        auto y = z++;
        auto x = y;

        while (isect(y, z))
            z = hull.erase(z);

        if (x != hull.begin() && isect(--x, y))
        {
            isect(x, hull.erase(y));
            return;
        }

        y = x;
        while (y != hull.begin() && (--x)->p >= y->p)
        {
            isect(x, hull.erase(y));
            y = x;
        }
    }

    ll query(ll x)
    {
        auto l = hull.lower_bound(x);
        return l->m * x + l->b;
    }
};
