#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> prefix_kmp(string &s)
{
    int n = s.size();
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++)
    {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j])
        {
            j = pi[j - 1];
        }
        if (s[i] == s[j])
        {
            j++;
        }
        pi[i] = j;
    }
    return pi;
}
void kmp_base(string &demo, string &pattern)
{
    vector<int> prefix_arr = prefix_kmp(pattern);
    int i = 0, j = 0;
    vector<int> positions;
    while (i < demo.size())
    {
        if (demo[i] == pattern[j])
        {
            i++;
            j++;
        }
        if (j == pattern.size())
        {
            int pos = i - j;
            positions.push_back(pos); // Store the start index of the match
            j = prefix_arr[j - 1];    // Continue searching for next occurrences
        }
        else if (i < demo.size() && demo[i] != pattern[j])
        {
            if (j != 0)
            {
                j = prefix_arr[j - 1];
            }
            else
            {
                i++;
            }
        }
    }
}
