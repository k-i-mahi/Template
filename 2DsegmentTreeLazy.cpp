#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct LazySegTree2D
{
    struct Node
    {
        ll sum;
        Node() : sum(0) {}
        Node(ll val) : sum(val) {}
    };

    struct LazyNode
    {
        ll value;
        bool pending;
        LazyNode() : value(0), pending(false) {}
        LazyNode(ll val) : value(val), pending(true) {}
    };

    vector<vector<Node>> tree;
    vector<vector<LazyNode>> lazy;
    int n;

    void init(int n)
    {
        this->n = n;
        tree.resize(4 * n + 5, vector<Node>(4 * n + 5));
        lazy.resize(4 * n + 5, vector<LazyNode>(4 * n + 5));
    }

    Node merge(const Node &left, const Node &right)
    {
        Node res;
        res.sum = left.sum + right.sum;
        return res;
    }

    void applyLazy(int ndx, int ndy, int stx, int edx, int sty, int edy, const LazyNode &lazyVal)
    {
        if (lazyVal.pending)
        {
            tree[ndx][ndy].sum = lazyVal.value * (edx - stx + 1) * (edy - sty + 1);
        }
    }

    void propagateLazy(int ndx, int ndy, int stx, int edx, int sty, int edy)
    {
        if (lazy[ndx][ndy].pending)
        {
            applyLazy(ndx, ndy, stx, edx, sty, edy, lazy[ndx][ndy]);
            if (stx != edx || sty != edy)
            {
                lazy[2 * ndx][ndy] = lazy[ndx][ndy];
                lazy[2 * ndx + 1][ndy] = lazy[ndx][ndy];
                lazy[ndx][2 * ndy] = lazy[ndx][ndy];
                lazy[ndx][2 * ndy + 1] = lazy[ndx][ndy];
            }
            lazy[ndx][ndy] = LazyNode();
        }
    }

    void build_y(int ndx, int stx, int edx, int ndy, int sty, int edy, const vector<vector<ll>> &v)
    {
        if (sty == edy)
        {
            if (stx == edx)
                tree[ndx][ndy] = Node(v[stx][sty]);
            else
                tree[ndx][ndy] = merge(tree[2 * ndx][ndy], tree[2 * ndx + 1][ndy]);
            return;
        }
        int midy = (sty + edy) / 2;
        build_y(ndx, stx, edx, 2 * ndy, sty, midy, v);
        build_y(ndx, stx, edx, 2 * ndy + 1, midy + 1, edy, v);
        tree[ndx][ndy] = merge(tree[ndx][2 * ndy], tree[ndx][2 * ndy + 1]);
    }
    void build_x(int ndx, int stx, int edx, const vector<vector<ll>> &v)
    {
        if (stx != edx)
        {
            int midx = (stx + edx) / 2;
            build_x(2 * ndx, stx, midx, v);
            build_x(2 * ndx + 1, midx + 1, edx, v);
        }
        build_y(ndx, stx, edx, 1, 1, n, v);
    }

    void build(const vector<vector<ll>> &v)
    {
        build_x(1, 1, n, v);
    }
    void update_y(int ndx, int stx, int edx, int ndy, int sty, int edy, int ly, int ry, ll val)
    {
        propagateLazy(ndx, ndy, stx, edx, sty, edy);
        if (sty > ry || edy < ly)
            return;
        if (sty >= ly && edy <= ry)
        {
            lazy[ndx][ndy] = LazyNode(val);
            propagateLazy(ndx, ndy, stx, edx, sty, edy);
            return;
        }
        int midy = (sty + edy) / 2;
        update_y(ndx, stx, edx, 2 * ndy, sty, midy, ly, ry, val);
        update_y(ndx, stx, edx, 2 * ndy + 1, midy + 1, edy, ly, ry, val);
        tree[ndx][ndy] = merge(tree[ndx][2 * ndy], tree[ndx][2 * ndy + 1]);
    }
    void update_x(int ndx, int stx, int edx, int lx, int rx, int ly, int ry, ll val)
    {
        propagateLazy(ndx, 1, stx, edx, 1, n);
        if (stx > rx || edx < lx)
            return;
        if (stx >= lx && edx <= rx)
        {
            update_y(ndx, stx, edx, 1, 1, n, ly, ry, val);
            return;
        }
        int midx = (stx + edx) / 2;
        update_x(2 * ndx, stx, midx, lx, rx, ly, ry, val);
        update_x(2 * ndx + 1, midx + 1, edx, lx, rx, ly, ry, val);
        for (int ndy = 1; ndy <= 4 * n; ++ndy)
            tree[ndx][ndy] = merge(tree[2 * ndx][ndy], tree[2 * ndx + 1][ndy]);
    }
    void updateRange(int lx, int rx, int ly, int ry, ll val)
    {
        update_x(1, 1, n, lx, rx, ly, ry, val);
    }
    Node query_y(int ndx, int stx, int edx, int ndy, int sty, int edy, int ly, int ry)
    {
        propagateLazy(ndx, ndy, stx, edx, sty, edy);
        if (sty > ry || edy < ly)
            return Node();
        if (sty >= ly && edy <= ry)
            return tree[ndx][ndy];
        int midy = (sty + edy) / 2;
        Node left = query_y(ndx, stx, edx, 2 * ndy, sty, midy, ly, ry);
        Node right = query_y(ndx, stx, edx, 2 * ndy + 1, midy + 1, edy, ly, ry);
        return merge(left, right);
    }
    Node query_x(int ndx, int stx, int edx, int lx, int rx, int ly, int ry)
    {
        propagateLazy(ndx, 1, stx, edx, 1, n);
        if (stx > rx || edx < lx)
            return Node();
        if (stx >= lx && edx <= rx)
            return query_y(ndx, stx, edx, 1, 1, n, ly, ry);
        int midx = (stx + edx) / 2;
        Node left = query_x(2 * ndx, stx, midx, lx, rx, ly, ry);
        Node right = query_x(2 * ndx + 1, midx + 1, edx, lx, rx, ly, ry);
        return merge(left, right);
    }
    ll queryRange(int lx, int rx, int ly, int ry)
    {
        return query_x(1, 1, n, lx, rx, ly, ry).sum;
    }
};