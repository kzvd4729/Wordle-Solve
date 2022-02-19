#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <map>

using namespace std;

const int WS = 5;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
vector<string> read_input()
{
    ifstream cin("resources/big_list.txt");
    vector<string> ret;
    string s;
    while (cin >> s)
    {
        // for (auto &x : s)
        //     x = x - 'a' + 'A';
        ret.push_back(s);
    }
    shuffle(ret.begin(), ret.end(), rng);
    ret.resize(100);

    return ret;
}

vector<char> cmap = {'b', 'y', 'g'};
void generate_color_mask(string s, vector<string> &mask)
{
    if (s.size() == WS)
    {
        mask.push_back(s);
        return;
    }
    for (int i = 0; i < 3; i++)
    {
        s.push_back(cmap[i]);
        generate_color_mask(s, mask);
        s.pop_back();
    }
}

string feedback(string guess, string hidden)
{
    string color;
    for (int i = 0; i < WS; i++)
        color.push_back('b');

    vector<int> vis(WS, 0);

    for (int i = 0; i < hidden.size(); i++)
    {
        if (hidden[i] == guess[i])
        {
            color[i] = 'g';
            vis[i] = 1;
        }
    }
    for (int i = 0; i < guess.size(); i++)
    {
        if (color[i] == 'g')
            continue;
        for (int j = 0; j < hidden.size(); j++)
        {
            if (vis[j])
                continue;
            if (guess[i] == hidden[j])
            {
                color[i] = 'y';
                vis[j] = 1;
                break;
            }
        }
    }
    return color;
}

void dfs(vector<string> words)
{
    if (words.size() == 1)
        return;
    for (auto x : words)
    {
        map<string, vector<string>> mp;
        for (auto y : words)
        {
            mp[feedback(x, y)].push_back(y);
        }
        for (auto x : mp)
            dfs(x.second);
    }
}

int main()
{
    vector<string> words = read_input();

    vector<string> mask;
    generate_color_mask("", mask);

    dfs(words);

    cout << "end of programm" << endl;
    return 0;
}