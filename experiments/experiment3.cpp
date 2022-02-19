/*
score to beat:
errichto: 3.42117

small_list = 2315 words
big_list = 12972 words


score got so far:
small_list = 3.68   run_time = 6.19s
big_list = 4.77     run_time = 189.8s

small_list = 3.65   run_time = 8.65s
big_list = 4.45     run_time = 311.08s

*/
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <map>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

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
    // ret.resize(1000);

    return ret;
}

string find_best_guess(vector<string> &current, vector<string> &words, vector<bool> bad)
{
    vector<int> fr(26, 0);
    for (auto x : current)
    {
        sort(x.begin(), x.end());
        x.erase(unique(x.begin(), x.end()), x.end());

        for (auto c : x)
            fr[c - 'a']++;
    }

    vector<pair<int, char>> ch;
    for (int i = 0; i < 26; i++)
        ch.push_back({fr[i], (char)(i + 'a')});

    sort(ch.begin(), ch.end());

    // for (int i = 25; i >= 0; i--)
    //     cout << ch[i].second << " " << ch[i].first << endl;

    int best = -1;
    string ret = "";

    vector<string> now = words;
    if (current.size() < 10)
        now = current;

    for (auto x : now)
    {
        string copy = x;
        sort(x.begin(), x.end());
        x.erase(unique(x.begin(), x.end()), x.end());

        int score = 0;

        for (auto c : x)
            score += fr[c - 'a'] * (bad[c - 'a'] ^ 1);

        if (score > best)
        {
            best = score;
            ret = copy;
        }
    }
    return ret;
}

string feedback(string guess, string hidden)
{
    string color(WS, 'b');
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

void print_word(string word, string color = "")
{
    for (int i = 0; i < word.size(); i++)
        word[i] = word[i] - 'a' + 'A';

    for (int i = 0; i < WS; i++)
    {
        if (color[i] == 'g')
            cout << "\033[32m" << word[i] << "\033[39m";
        else if (color[i] == 'y')
            cout << "\033[33m" << word[i] << "\033[39m";
        else
            cout << word[i];
    }
    cout << endl;
}

int play(string hidden, vector<string> &words, bool one)
{
    vector<string> current = words;
    vector<bool> bad(26, 0);
    for (int i = 1;; i++)
    {
        string guess = find_best_guess(current, words, bad);

        string color = feedback(guess, hidden);

        for (int j = 0; j < WS; j++)
        {
            if ((color[j] == 'g') || (color[j] == 'y'))
                bad[guess[j] - 'a'] = 1;
        }

        if (one)
            print_word(guess, color);

        if (color == "ggggg")
            return i;

        vector<string> tmp;
        for (auto x : current)
        {
            if (feedback(guess, x) == color)
                tmp.push_back(x);
        }

        if (tmp.size() == 0)
            assert(0);

        current = tmp;
    }
}

void play_small_list(vector<string> &words)
{
    ifstream cin("resources/small_list.txt");
    vector<string> ret;
    string s;
    while (cin >> s)
    {
        // for (auto &x : s)
        //     x = x - 'a' + 'A';
        ret.push_back(s);
    }

    cout << endl;
    double total = 0;

    vector<int> frequency(100, 0);

    int cnt = 0;
    vector<int> percent;
    for (double i = 0.1; i <= 1; i += 0.1)
        percent.push_back((int)(ret.size() * i));
    percent.push_back(-1);

    int pt = 0;
    for (auto x : ret)
    {
        int gs = play(x, words, 0);
        total += gs;
        frequency[gs]++;

        cnt++;

        if (cnt == percent[pt])
        {
            cout << "done: " << (pt + 1) * 10 << "%" << endl;
            pt++;
        }
    }
    cout << endl
         << endl;
    cout << "Words = " << ret.size() << endl;
    cout << "Average = " << total / ret.size() << endl;

    for (int i = 1; i < frequency.size(); i++)
    {
        if (frequency[i])
            cout << i << " --> " << frequency[i] << endl;
    }
}

void play_all(vector<string> &words)
{
    cout << endl;
    double total = 0;

    vector<int> frequency(100, 0);

    int cnt = 0;
    vector<int> percent;
    for (double i = 0.1; i <= 1; i += 0.1)
        percent.push_back((int)(words.size() * i));
    percent.push_back(-1);

    int pt = 0;
    for (auto x : words)
    {
        int gs = play(x, words, 0);
        total += gs;
        frequency[gs]++;

        cnt++;

        if (cnt == percent[pt])
        {
            cout << "done: " << (pt + 1) * 10 << "%" << endl;
            pt++;
        }
    }
    cout << endl
         << endl;
    cout << "Words = " << words.size() << endl;
    cout << "Average = " << total / words.size() << endl;

    for (int i = 1; i < frequency.size(); i++)
    {
        if (frequency[i])
            cout << i << " --> " << frequency[i] << endl;
    }
}

int main()
{
    clock_t tStart = clock();

    vector<string> words = read_input();

    // cout << words.size() << endl;
    // play_all(words);
    play_small_list(words);
    // play("every", words, 1);

    printf("\n\nTime taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

    return 0;
}