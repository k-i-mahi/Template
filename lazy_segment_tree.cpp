#include <vector>
#include <algorithm>
using namespace std;

using ll = long long;

struct LazySegTree
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
        LazyNode() : value(0) {}
        LazyNode(ll val) : value(val) {}
    };

    vector<Node> tree;
    vector<LazyNode> lazy;
    int n;

    void init(int n)
    {
        this->n = n;
        tree.resize(4 * n + 5);
        lazy.resize(4 * n + 5);
    }

    Node merge(const Node &left, const Node &right)
    {
        Node res;
        res.sum = max(left.sum, right.sum);
        return res;
    }

    void applyLazy(int nd, int st, int ed)
    {
        tree[nd].sum = lazy[nd].value;
    }

    void propagateLazy(int nd, int st, int ed)
    {
        if (lazy[nd].value != 0)
        {
            applyLazy(nd, st, ed);
            if (st != ed)
            {
                lazy[2 * nd].value += lazy[nd].value;
                lazy[2 * nd + 1].value += lazy[nd].value;
            }
            lazy[nd] = LazyNode();
        }
    }

    void build(int nd, int st, int ed, const vector<ll> &v)
    {
        if (st == ed)
        {
            tree[nd] = Node(v[st]);
            return;
        }
        int mid = (st + ed) / 2;
        build(2 * nd, st, mid, v);
        build(2 * nd + 1, mid + 1, ed, v);
        tree[nd] = merge(tree[2 * nd], tree[2 * nd + 1]);
    }

    void build(const vector<ll> &v)
    {
        build(1, 1, n, v);
    }

    void updateRange(int nd, int st, int ed, int l, int r, ll val)
    {
        propagateLazy(nd, st, ed);
        if (st > r || ed < l)
        {
            return;
        }
        if (st >= l && ed <= r)
        {
            lazy[nd].value = val;
            propagateLazy(nd, st, ed);
            return;
        }
        int mid = (st + ed) / 2;
        updateRange(2 * nd, st, mid, l, r, val);
        updateRange(2 * nd + 1, mid + 1, ed, l, r, val);
        tree[nd] = merge(tree[2 * nd], tree[2 * nd + 1]);
    }

    void updateRange(int l, int r, ll val)
    {
        updateRange(1, 1, n, l, r, val);
    }

    Node queryRange(int nd, int st, int ed, int l, int r)
    {
        propagateLazy(nd, st, ed);
        if (st > r || ed < l)
        {
            return 0;
        }
        if (st >= l && ed <= r)
        {
            return tree[nd];
        }
        int mid = (st + ed) / 2;
        Node leftQuery = queryRange(2 * nd, st, mid, l, r);
        Node rightQuery = queryRange(2 * nd + 1, mid + 1, ed, l, r);
        return merge(leftQuery, rightQuery);
    }

    ll queryRange(int l, int r)
    {
        return queryRange(1, 1, n, l, r).sum;
    }

    void updatePoint(int idx, ll val)
    {
        updateRange(idx, idx, val);
    }

    ll queryPoint(int idx)
    {
        return queryRange(idx, idx);
    }
};
