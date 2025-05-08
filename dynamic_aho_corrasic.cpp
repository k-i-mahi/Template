#include <bits/stdc++.h>
using namespace std;

struct AhoCorasick
{
    struct Node
    {
        int children[27], go[27], fail;
        set<int> output;

        Node()
        {
            memset(children, -1, sizeof(children));
            memset(go, -1, sizeof(go));
            fail = -1;
        }
    };

    vector<Node> nodes;
    int root;

    AhoCorasick()
    {
        root = 0;
        nodes.push_back(Node());
    }

    int charToIndex(char c)
    {
        return c - 'a' + 1;
    }

    // Insert a pattern dynamically
    void insert(const string &s, int id)
    {
        int curr = root;
        for (char c : s)
        {
            int i = charToIndex(c);
            if (nodes[curr].children[i] == -1)
            {
                nodes[curr].children[i] = nodes.size();
                nodes.push_back(Node());
            }
            curr = nodes[curr].children[i];
        }
        nodes[curr].output.insert(id);
        build(); // Rebuild failure links after insertion
    }

    // Build failure links using BFS
    void build()
    {
        queue<int> q;
        nodes[root].fail = -1;
        q.push(root);

        while (!q.empty())
        {
            int curr = q.front();
            q.pop();

            for (int i = 1; i <= 26; ++i)
            {
                int child = nodes[curr].children[i];
                if (child == -1)
                    continue;

                int fail = nodes[curr].fail;
                while (fail != -1 && nodes[fail].children[i] == -1)
                    fail = nodes[fail].fail;

                nodes[child].fail = (fail == -1) ? root : nodes[fail].children[i];

                // Merge output from failure link
                nodes[child].output.insert(nodes[nodes[child].fail].output.begin(),
                                           nodes[nodes[child].fail].output.end());

                q.push(child);
            }

            // Compute transitions
            for (int i = 1; i <= 26; ++i)
            {
                if (nodes[curr].children[i] != -1)
                {
                    nodes[curr].go[i] = nodes[curr].children[i];
                }
                else
                {
                    nodes[curr].go[i] = (curr == root) ? root : nodes[nodes[curr].fail].go[i];
                }
            }
        }
    }

    // Search text dynamically
    unordered_map<int, vector<int>> search(const string &text)
    {
        unordered_map<int, vector<int>> matches;
        int curr = root;

        for (int j = 0; j < text.size(); ++j)
        {
            int i = charToIndex(text[j]);
            curr = nodes[curr].go[i];

            int temp = curr;
            while (temp != root)
            {
                for (int id : nodes[temp].output)
                    matches[id].push_back(j - id + 1);
                temp = nodes[temp].fail;
            }
        }
        return matches;
    }

    // Remove a pattern dynamically
    void remove(const string &s, int id)
    {
        int curr = root;
        for (char c : s)
        {
            int i = charToIndex(c);
            if (nodes[curr].children[i] == -1)
                return; // Pattern doesn't exist
            curr = nodes[curr].children[i];
        }

        nodes[curr].output.erase(id);

        // If no other patterns rely on this node, remove it
        cleanUp();
    }

    // Cleanup function to remove unused nodes (only called after deletion)
    void cleanUp()
    {
        for (int i = nodes.size() - 1; i >= 1; --i)
        {
            if (nodes[i].output.empty())
            {
                bool hasChildren = false;
                for (int j = 1; j <= 26; ++j)
                {
                    if (nodes[i].children[j] != -1)
                    {
                        hasChildren = true;
                        break;
                    }
                }
                if (!hasChildren)
                    nodes.pop_back();
            }
        }
        build(); // Rebuild failure links after cleanup
    }
};
