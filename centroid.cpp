#include <bits/stdc++.h>
using namespace std;
const int INF = 1e9;

struct CentroidDecomposition
{
    int n;                       // number of nodes (1-indexed)
    vector<vector<int>> adj;     // original tree
    vector<bool> centroidMarked; // whether a node is already chosen as a centroid
    vector<int> subSize;         // subtree sizes computed via DFS
    vector<int> parent;          // parent in the centroid tree
    // For each node, store pairs (centroid, distance from the node to that centroid)
    vector<vector<pair<int, int>>> centroidPath;
    // For each centroid (node id), store a multiset of distances from special nodes.
    vector<multiset<int>> bestSet;

    // Constructor: initializes the tree data structures for 1-indexed nodes.
    CentroidDecomposition(int n) : n(n)
    {
        adj.resize(n + 1);
        centroidMarked.assign(n + 1, false);
        subSize.assign(n + 1, 0);
        parent.assign(n + 1, -1);
        centroidPath.resize(n + 1);
        bestSet.resize(n + 1); // Each node could become a centroid.
    }

    // Adds an undirected edge between u and v.
    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // DFS to compute subtree sizes for the current component.
    void dfs(int u, int p)
    {
        subSize[u] = 1;
        for (int v : adj[u])
        {
            if (v == p || centroidMarked[v])
                continue;
            dfs(v, u);
            subSize[u] += subSize[v];
        }
    }

    // Finds the centroid of the current subtree.
    int findCentroid(int u, int p, int totalSize)
    {
        for (int v : adj[u])
        {
            if (v == p || centroidMarked[v])
                continue;
            if (subSize[v] > totalSize / 2)
                return findCentroid(v, u, totalSize);
        }
        return u;
    }

    // For the current centroid 'centroid', this DFS populates the centroidPath
    // for every node in its component with a pair (centroid, distance).
    void addCentroidDistances(int centroid, int u, int p, int d)
    {
        centroidPath[u].push_back({centroid, d});
        for (int v : adj[u])
        {
            if (v == p || centroidMarked[v])
                continue;
            addCentroidDistances(centroid, v, u, d + 1);
        }
    }

    // Recursively decomposes the tree into a centroid tree.
    // 'root' is the current component's root and 'par' is its parent in the centroid tree.
    int decompose(int root, int par = -1)
    {
        dfs(root, -1);
        int c = findCentroid(root, -1, subSize[root]);
        centroidMarked[c] = true;
        parent[c] = (par == -1 ? c : par); // for the root of the centroid tree, assign its parent as itself

        // For every node in the component, record the distance from the current centroid c.
        addCentroidDistances(c, c, -1, 0);

        // Recursively decompose the remaining subtrees.
        for (int v : adj[c])
        {
            if (!centroidMarked[v])
                decompose(v, c);
        }
        return c;
    }

    // Update function: mark node v as special (color it red)
    // For each centroid in v's centroid path, insert the corresponding distance.
    void update(int v)
    {
        for (auto &p : centroidPath[v])
        {
            int centroid = p.first;
            int d = p.second;
            bestSet[centroid].insert(d);
        }
    }

    // Remove function: turn back the color from node v (i.e. unmark it)
    // For each centroid in v's centroid path, remove one instance of its distance.
    void removeColor(int v)
    {
        for (auto &p : centroidPath[v])
        {
            int centroid = p.first;
            int d = p.second;
            auto it = bestSet[centroid].find(d);
            if (it != bestSet[centroid].end())
                bestSet[centroid].erase(it);
        }
    }

    // Query function: returns the minimum distance from node v to any special node.
    int query(int v)
    {
        int res = INF;
        for (auto &p : centroidPath[v])
        {
            int centroid = p.first;
            int d = p.second;
            if (!bestSet[centroid].empty())
            {
                res = min(res, d + *bestSet[centroid].begin());
            }
        }
        return res;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    CentroidDecomposition cd(n);

    // Read n-1 edges (assuming the tree is given as 1-indexed)
    for (int i = 1; i <= n - 1; i++)
    {
        int u, v;
        cin >> u >> v;
        cd.addEdge(u, v);
    }

    // Build the centroid decomposition starting from node 1.
    int centroidRoot = cd.decompose(1);

    // To avoid duplicate updates, we track the current "special" status of nodes.
    vector<bool> special(n + 1, false);

    // For demonstration, assume initially node 1 is special.
    cd.update(1);
    special[1] = true;

    // Process queries.
    // Query format (example):
    //   Type 1 v : mark (update) node v as special.
    //   Type 2 v : remove the special color from node v.
    //   Type 3 v : query the minimum distance from node v to any special node.
    int q;
    cin >> q;
    while (q--)
    {
        int type, v;
        cin >> type >> v;
        if (type == 1)
        { // Mark node v as special
            if (!special[v])
            {
                cd.update(v);
                special[v] = true;
            }
        }
        else if (type == 2)
        { // Turn back the color (unmark node v)
            if (special[v])
            {
                cd.removeColor(v);
                special[v] = false;
            }
        }
        else if (type == 3)
        { // Query the minimum distance from node v to any special node
            int ans = cd.query(v);
            if (ans >= INF)
                ans = -1; // If no special node is reachable, return -1.
            cout << ans << "\n";
        }
    }
    return 0;
}
