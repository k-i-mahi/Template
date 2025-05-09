#include <bits/stdc++.h>
using namespace std;

struct AhoCorasick
{
    struct Node
    {
        int children[26], go[26], fail;  // 0-based index for 26 characters
        set<int> output;

        Node()
        {
            memset(children, -1, sizeof(children));  // Initialize all children as -1
            memset(go, -1, sizeof(go));              // Initialize go transitions as -1
            fail = -1;
        }
    };

    vector<Node> nodes;
    int root;

    AhoCorasick()
    {
        root = 0;
        nodes.push_back(Node());  // Root node
    }

    int charToIndex(char c)
    {
        return c - 'a';  // Zero-based index (0 for 'a', 1 for 'b', ...)
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
        build();  // Rebuild failure links after insertion
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

            for (int i = 0; i < 26; ++i)  // Check all 26 characters
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

            // Compute transitions using the fail links
            for (int i = 0; i < 26; ++i)
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

    // Search text dynamically (only matches active patterns)
    vector<pair<int, int>> search(const string &text)
    {
        vector<pair<int, int>> matches;  // {pattern id, position}
        int curr = root;

        for (int j = 0; j < text.size(); ++j)
        {
            int i = charToIndex(text[j]);
            curr = nodes[curr].go[i];

            int temp = curr;
            while (temp != root)
            {
                for (int id : nodes[temp].output)
                {
                    matches.push_back({id, j});
                }
                temp = nodes[temp].fail;
            }
        }
        return matches;
    }
};
