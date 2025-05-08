#include <bits/stdc++.h>
using namespace std;

struct DynamicAhoCorasick
{
    struct Node
    {
        int children[26], go[26], fail;
        vector<int> output;
        bool is_end = false; // Tracks if any word ends at this node

        Node()
        {
            memset(children, -1, sizeof(children));
            memset(go, -1, sizeof(go));
            fail = 0;
        }
    };

    vector<Node> nodes;
    int root;
    bool built = false;

    DynamicAhoCorasick()
    {
        root = 0;
        nodes.push_back(Node());
    }

    void insert(const string &s, int idx)
    {
        int curr = root;
        for (char c : s)
        {
            int i = c - 'a';
            if (nodes[curr].children[i] == -1)
            {
                nodes[curr].children[i] = nodes.size();
                nodes.push_back(Node());
            }
            curr = nodes[curr].children[i];
        }
        nodes[curr].output.push_back(idx);
        nodes[curr].is_end = true;
        built = false;
    }

    void build()
    {
        queue<int> q;
        nodes[root].fail = root;
        q.push(root);

        while (!q.empty())
        {
            int curr = q.front();
            q.pop();

            for (int i = 0; i < 26; ++i)
            {
                int child = nodes[curr].children[i];
                if (child == -1)
                    continue;

                int fail = nodes[curr].fail;
                while (fail != root && nodes[fail].children[i] == -1)
                    fail = nodes[fail].fail;

                if (nodes[fail].children[i] != -1 && nodes[fail].children[i] != child)
                    fail = nodes[fail].children[i];

                nodes[child].fail = fail;
                nodes[child].output.insert(nodes[child].output.end(),
                                           nodes[fail].output.begin(), nodes[fail].output.end());
                q.push(child);
            }

            for (int i = 0; i < 26; ++i)
            {
                if (nodes[curr].children[i] != -1)
                    nodes[curr].go[i] = nodes[curr].children[i];
                else
                    nodes[curr].go[i] = (curr == root) ? root : nodes[nodes[curr].fail].go[i];
            }
        }
        built = true;
    }

    void erase(const string &s, int idx)
    {
        int curr = root;
        vector<int> path; // Stores nodes visited along the path

        // Traverse the trie to find the end node of the word
        for (char c : s)
        {
            int i = c - 'a';
            if (nodes[curr].children[i] == -1)
            {
                return; // Word not found, nothing to erase
            }
            curr = nodes[curr].children[i];
            path.push_back(curr);
        }

        // Remove the pattern index from the output list
        auto &out = nodes[curr].output;
        out.erase(remove(out.begin(), out.end(), idx), out.end());

        // If no patterns end at this node, mark it as non-terminal
        if (out.empty())
        {
            nodes[curr].is_end = false;
        }

        // Clean up unnecessary nodes (only if they are empty and not part of another word)
        for (int i = path.size() - 1; i >= 0; --i)
        {
            int node = path[i];

            // Check if the node has no remaining children and is not a word endpoint
            bool hasChildren = false;
            for (int j = 0; j < 26; ++j)
            {
                if (nodes[node].children[j] != -1)
                {
                    hasChildren = true;
                    break;
                }
            }

            if (!nodes[node].is_end && !hasChildren)
            {
                int parent = (i == 0) ? root : path[i - 1];

                // Find the parent-child link and remove it
                for (int j = 0; j < 26; ++j)
                {
                    if (nodes[parent].children[j] == node)
                    {
                        nodes[parent].children[j] = -1;
                        break;
                    }
                }

                // Reset the node since it's no longer needed
                nodes[node] = Node();
            }
            else
            {
                break; // Stop cleanup if the node is still needed
            }
        }

        built = false; // Mark for rebuilding failure links if necessary
    }

    vector<vector<int>> search(const string &text)
    {
        if (!built)
            build();

        vector<vector<int>> matches(nodes.size());
        int curr = root;

        for (int j = 0; j < text.size(); ++j)
        {
            int i = text[j] - 'a';
            curr = nodes[curr].go[i];

            int temp = curr;
            while (temp != root)
            {
                for (int id : nodes[temp].output)
                    matches[id].push_back(j);
                temp = nodes[temp].fail;
            }
        }
        return matches;
    }
};
