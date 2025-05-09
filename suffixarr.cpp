#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct SuffixArray
{
    string s;
    int n;
    vector<int> sa, rank, lcp, log2;
    vector<vector<int>> st_lcp;

    SuffixArray(const string &t)
    {
        s = t + '\0';
        n = s.size();
        sa.resize(n);
        rank.resize(n);
        build_sa();
        build_lcp();
        build_sparse_table();
    }

    void build_sa()
    {
        vector<int> cnt(max(256, n), 0), tmp(n), c(n);
        for (char ch : s)
        {
            cnt[ch]++;
        }
        for (int i = 1; i < 256; i++)
        {
            cnt[i] += cnt[i - 1];
        }
        for (int i = 0; i < n; i++)
        {
            sa[--cnt[s[i]]] = i;
        }
        c[sa[0]] = 0;
        for (int i = 1; i < n; i++)
        {
            c[sa[i]] = c[sa[i - 1]] + (s[sa[i]] != s[sa[i - 1]]);
        }

        int k = 0;
        while ((1 << k) < n)
        {
            for (int i = 0; i < n; i++)
            {
                sa[i] = (sa[i] - (1 << k) + n) % n;
            }

            fill(cnt.begin(), cnt.begin() + n, 0);
            for (int x : c)
            {
                cnt[x]++;
            }
            for (int i = 1; i < n; i++)
            {
                cnt[i] += cnt[i - 1];
            }
            for (int i = n - 1; i >= 0; i--)
            {
                tmp[--cnt[c[sa[i]]]] = sa[i];
            }
            sa = tmp;

            tmp[sa[0]] = 0;
            for (int i = 1; i < n; i++)
            {
                pair<int, int> prev = {c[sa[i - 1]], c[(sa[i - 1] + (1 << k)) % n]};
                pair<int, int> curr = {c[sa[i]], c[(sa[i] + (1 << k)) % n]};
                tmp[sa[i]] = tmp[sa[i - 1]] + (curr != prev);
            }
            c = tmp;
            k++;
        }
    }

    void build_lcp()
    {
        lcp.resize(n - 1);
        for (int i = 0; i < n; i++)
        {
            rank[sa[i]] = i;
        }
        int k = 0;
        for (int i = 0; i < n; i++)
        {
            if (rank[i] == 0)
            {
                k = 0;
                continue;
            }
            int j = sa[rank[i] - 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k])
            {
                k++;
            }
            lcp[rank[i] - 1] = k;
            if (k > 0)
            {
                k--;
            }
        }
    }

    void build_sparse_table()
    {
        int m = lcp.size();
        log2.resize(m + 1);
        log2[1] = 0;
        for (int i = 2; i <= m; i++)
        {
            log2[i] = log2[i / 2] + 1;
        }

        int max_log = log2[m] + 1;
        st_lcp.resize(max_log);
        st_lcp[0] = lcp;

        for (int j = 1; j < max_log; j++)
        {
            int curr_len = m - (1 << j) + 1;
            if (curr_len <= 0)
            {
                st_lcp[j].clear();
                continue;
            }
            st_lcp[j].resize(curr_len);
            for (int i = 0; i + (1 << j) <= m; i++)
            {
                st_lcp[j][i] = min(st_lcp[j - 1][i], st_lcp[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    int get_lcp(int i, int j)
    {
        if (i == j)
        {
            return n - 1 - i;
        }
        int x = rank[i], y = rank[j];
        if (x > y)
        {
            swap(x, y);
        }
        int len = y - x;
        if (len == 0)
        {
            return n - 1 - i;
        }
        int k = log2[len];
        return min(st_lcp[k][x], st_lcp[k][y - (1 << k)]);
    }

    bool substring_search(const string &t)
    {
        int m = t.size();
        int l = 0, r = n - 1;
        while (l <= r)
        {
            int mid = (l + r) / 2;
            string suf = s.substr(sa[mid], min(n - sa[mid], m));
            if (suf == t)
            {
                return true;
            }
            if (suf < t)
            {
                l = mid + 1;
            }
            else
            {
                r = mid - 1;
            }
        }
        return false;
    }

int compare_substrings(int l1, int r1, int l2, int r2) {
    int len1 = r1 - l1 + 1, len2 = r2 - l2 + 1;
    int common = min({len1, len2, get_lcp(l1, l2)});
    
    if (common == len1 && common == len2) {
        return 0; // substrings are exactly equal
    }
    if (common == len1) {
        return -1; // s[l1..r1] is prefix of s[l2..r2]
    }
    if (common == len2) {
        return 1; // s[l2..r2] is prefix of s[l1..r1]
    }
    // Compare next character after common prefix
    return s[l1 + common] < s[l2 + common] ? -1 : 1;
}
};
