#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

using ll = long long;

struct segt
{
    struct Node
    {
        ll sum;
        Node() : sum(0) {}
        Node(ll val) : sum(val) {}
    };

    vector<Node> tre;
    int n;
    void init(int n)
    {
        this->n = n;
        tre.resize(4 * n + 5);
    }

    Node merge(const Node &left, const Node &right)
    {
        Node res;
        res.sum = left.sum + right.sum;
        return res;
    }
    void build(int nd, int st, int ed, const vector<ll> &v)
    {
        if (st == ed)
        {
            tre[nd] = Node(v[st - 1]);
            return;
        }
        int mid = (st + ed) / 2;
        build(2 * nd, st, mid, v);
        build(2 * nd + 1, mid + 1, ed, v);
        tre[nd] = merge(tre[2 * nd], tre[2 * nd + 1]);
    }

    void build(const vector<ll> &v)
    {
        build(1, 1, n, v);
    }

    Node query(int nd, int st, int ed, int l, int r)
    {
        if (st > r || ed < l)
        {
            return Node();
        }
        if (st >= l && ed <= r)
        {
            return tre[nd];
        }
        int mid = (st + ed) / 2;
        Node leftQuery = query(2 * nd, st, mid, l, r);
        Node rightQuery = query(2 * nd + 1, mid + 1, ed, l, r);
        return merge(leftQuery, rightQuery);
    }

    Node query(int l, int r)
    {
        return query(1, 1, n, l, r);
    }
    void update(int nd, int st, int ed, int idx, ll val)
    {
        if (st == ed && st == idx)
        {
            tre[nd] = Node(val);
            return;
        }
        int mid = (st + ed) / 2;
        if (idx <= mid)
        {
            update(2 * nd, st, mid, idx, val);
        }
        else
        {
            update(2 * nd + 1, mid + 1, ed, idx, val);
        }
        tre[nd] = merge(tre[2 * nd], tre[2 * nd + 1]);
    }
    void update(int idx, ll val)
    {
        update(1, 1, n, idx, val);
    }
};
