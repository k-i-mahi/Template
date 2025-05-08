#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct SegmentTree2D
{
    struct Node
    {
        int sum;
        Node() : sum(0) {}
        Node(int value) : sum(value) {}
    };
    vector<vector<Node>> tree;
    int n;

    void init(int size)
    {
        n = size;
        tree.assign(4 * n + 1, vector<Node>(4 * n + 1, Node()));
    }
    Node merge(const Node &left, const Node &right)
    {
        return Node(left.sum + right.sum);
    }
    void build_y(int ndx, int stx, int edx, int ndy, int sty, int edy, const vector<vector<int>> &grid)
    {
        if (sty == edy)
        {
            if (stx == edx)
            {
                tree[ndx][ndy] = Node(grid[stx][sty]);
            }
            else
            {
                tree[ndx][ndy] = merge(tree[2 * ndx][ndy], tree[2 * ndx + 1][ndy]);
            }
        }
        else
        {
            int midy = (sty + edy) / 2;
            build_y(ndx, stx, edx, 2 * ndy, sty, midy, grid);
            build_y(ndx, stx, edx, 2 * ndy + 1, midy + 1, edy, grid);
            tree[ndx][ndy] = merge(tree[ndx][2 * ndy], tree[ndx][2 * ndy + 1]);
        }
    }
    void build_x(int ndx, int stx, int edx, const vector<vector<int>> &grid)
    {
        if (stx != edx)
        {
            int midx = (stx + edx) / 2;
            build_x(2 * ndx, stx, midx, grid);
            build_x(2 * ndx + 1, midx + 1, edx, grid);
        }
        build_y(ndx, stx, edx, 1, 1, n, grid); 
    }
    void build(const vector<vector<int>> &grid)
    {
        build_x(1, 1, n, grid);
    }
    void update_y(int ndx, int stx, int edx, int ndy, int sty, int edy, int x, int y, int value)
    {
        if (sty == edy)
        {
            if (stx == edx)
            {
                tree[ndx][ndy] = Node(value);
            }
            else
            {
                tree[ndx][ndy] = merge(tree[2 * ndx][ndy], tree[2 * ndx + 1][ndy]);
            }
        }
        else
        {
            int midy = (sty + edy) / 2;
            if (y <= midy)
            {
                update_y(ndx, stx, edx, 2 * ndy, sty, midy, x, y, value);
            }
            else
            {
                update_y(ndx, stx, edx, 2 * ndy + 1, midy + 1, edy, x, y, value);
            }
            tree[ndx][ndy] = merge(tree[ndx][2 * ndy], tree[ndx][2 * ndy + 1]);
        }
    }

    void update_x(int ndx, int stx, int edx, int x, int y, int value)
    {
        if (stx != edx)
        {
            int midx = (stx + edx) / 2;
            if (x <= midx)
            {
                update_x(2 * ndx, stx, midx, x, y, value);
            }
            else
            {
                update_x(2 * ndx + 1, midx + 1, edx, x, y, value);
            }
        }
        update_y(ndx, stx, edx, 1, 1, n, x, y, value);
    }

    void update(int x, int y, int value)
    {
        update_x(1, 1, n, x, y, value); 
    }
    Node query_y(int ndx, int ndy, int sty, int edy, int y1, int y2)
    {
        if (y1 > edy || y2 < sty)
        {
            return Node();
        }
        if (y1 <= sty && edy <= y2)
        {
            return tree[ndx][ndy];
        }
        int midy = (sty + edy) / 2;
        Node left = query_y(ndx, 2 * ndy, sty, midy, y1, y2);
        Node right = query_y(ndx, 2 * ndy + 1, midy + 1, edy, y1, y2);
        return merge(left, right);
    }
    Node query_x(int ndx, int stx, int edx, int x1, int x2, int y1, int y2)
    {
        if (x1 > edx || x2 < stx)
        {
            return Node();
        }
        if (x1 <= stx && edx <= x2)
        {
            return query_y(ndx, 1, 1, n, y1, y2);
        }
        int midx = (stx + edx) / 2;
        Node left = query_x(2 * ndx, stx, midx, x1, x2, y1, y2);
        Node right = query_x(2 * ndx + 1, midx + 1, edx, x1, x2, y1, y2);
        return merge(left, right);
    }
    int query(int x1, int x2, int y1, int y2)
    {
        return query_x(1, 1, n, x1, x2, y1, y2).sum;
    }
};