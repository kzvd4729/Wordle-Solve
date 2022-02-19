/*
score to beat:
errichto: 3.42117

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

using namespace std;

const int WS = 5;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
vector<string> read_input()
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
    shuffle(ret.begin(), ret.end(), rng);
    // ret.resize(1000);

    return ret;
}

string find_best_guess(vector<string> &words)
{
    vector<int> fr(26, 0);
    for (auto x : words)
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
    for (auto x : words)
    {
        string copy = x;
        sort(x.begin(), x.end());
        x.erase(unique(x.begin(), x.end()), x.end());

        int score = 0;

        for (auto c : x)
            score += fr[c - 'a'];

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

int play(string hidden, const vector<string> &words, bool one)
{
    vector<string> current = words;

    for (int i = 1;; i++)
    {
        string guess = find_best_guess(current);

        if (one)
            cout << "Guess " << i << ": " << guess << " ";

        if (guess == hidden)
            return i;

        string color = feedback(guess, hidden);

        if (one)
            cout << color << endl;

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

    // play_all(words);

    play("scary", words, 1);

    printf("\n\nTime taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

    return 0;
}