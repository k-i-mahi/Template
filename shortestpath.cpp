#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

class ShortestPath
{
public:
    vector<vector<pair<int, long long>>> gr;
    vector<int> par;
    vector<long long> dis;
    vector<bool> vis, negc;
    int n;
    ShortestPath(int n)
    {
        this->n = n;
        gr.resize(n + 1);
        par.resize(n + 1, -1);
        dis.resize(n + 1, LLONG_MAX);
        vis.resize(n + 1, false);
        negc.resize(n + 1, false);
    }

    void addEdge(int u, int v, long long w)
    {
        gr[u].push_back({v, w});
        gr[v].push_back({u, w});
    }

    void dijkstra(int source)
    {
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        pq.push({0, source});
        dis[source] = 0;
        while (!pq.empty())
        {
            auto node = pq.top();
            pq.pop();
            int v = node.second;
            if (vis[v])
                continue;
            vis[v] = true;
            for (auto &child : gr[v])
            {
                int child_v = child.first;
                long long wt = child.second;
                if (dis[v] + wt < dis[child_v])
                {
                    dis[child_v] = dis[v] + wt;
                    par[child_v] = v;
                    pq.push({dis[child_v], child_v});
                }
            }
        }
    }
    vector<int> getPath(int source, int dest)
    {
        vector<int> path;
        if (dis[dest] == LLONG_MAX)
            return path;
        for (int v = dest; v != -1; v = par[v])
            path.push_back(v);
        reverse(path.begin(), path.end());
        return path;
    }
    bool bellmanFord(int source)
    {
        dis[source] = 0;
        for (int i = 1; i <= n - 1; ++i)
        {
            for (int u = 1; u <= n; ++u)
            {
                for (auto &child : gr[u])
                {
                    int v = child.first;
                    long long w = child.second;
                    if (dis[u] != LLONG_MAX && dis[u] + w < dis[v])
                    {
                        dis[v] = dis[u] + w;
                        par[v] = u;
                    }
                }
            }
        }
        bool flag = 0;
        for (int u = 1; u <= n; ++u)
        {
            for (auto &child : gr[u])
            {
                int v = child.first;
                long long w = child.second;
                if (dis[u] != LLONG_MAX && dis[u] + w < dis[v])
                {
                    negc[v] = true;
                    flag = 1;
                }
            }
        }
        return flag;
    }
    void negnode()
    {
        queue<int> q;
        for (int i = 1; i <= n; i++)
        {
            if (negc[i])
            {
                q.push(i);
            }
        }
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (auto &edge : gr[u])
            {
                int v = edge.first;
                if (!negc[v])
                {
                    negc[v] = true;
                    q.push(v);
                }
            }
        }
    }
    void floydWarshall(vector<vector<long long>> &dist)
    {
        dist.resize(n + 1, vector<long long>(n + 1, LLONG_MAX));
        for (int i = 1; i <= n; ++i)
            dist[i][i] = 0;
        for (int u = 1; u <= n; ++u)
        {
            for (auto &child : gr[u])
            {
                int v = child.first;
                long long w = child.second;
                dist[u][v] = w;
            }
        }
        for (int k = 1; k <= n; ++k)
        {
            for (int i = 1; i <= n; ++i)
            {
                for (int j = 1; j <= n; ++j)
                {
                    if (dist[i][k] != LLONG_MAX && dist[k][j] != LLONG_MAX)
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    void reset(int n)
    {
        fill(dis.begin(), dis.end(), LLONG_MAX);
        fill(vis.begin(), vis.end(), false);
        fill(par.begin(), par.end(), -1);
    }
};

