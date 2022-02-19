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
vector<string> read_wordle_answer()
{
    ifstream cin("resources/small_list.txt");
    vector<string> wordle_answer;
    string s;
    while (cin >> s)
    {
        // for (auto &x : s)
        //     x = x - 'a' + 'A';
        wordle_answer.push_back(s);
    }
    // shuffle(wordle_answer.begin(), wordle_answer.end(), rng);
    //  ret.resize(1000);

    return wordle_answer;
}
vector<string> read_wordle_guess()
{
    ifstream cin("resources/big_list.txt");
    vector<string> wordle_guess;
    string s;
    while (cin >> s)
    {
        // for (auto &x : s)
        //     x = x - 'a' + 'A';
        wordle_guess.push_back(s);
    }
    // shuffle(wordle_guess.begin(), wordle_guess.end(), rng);
    //  ret.resize(1000);

    return wordle_guess;
}
vector<int> read_worde_frequency()
{
    ifstream cin("resources/big_list_frequency.txt");
    vector<int> word_frequency;
    int s;
    while (cin >> s)
    {
        // for (auto &x : s)
        //     x = x - 'a' + 'A';
        word_frequency.push_back(s);
    }
    // shuffle(word_frequency.begin(), word_frequency.end(), rng);
    //  ret.resize(1000);

    return word_frequency;
}

int main()
{
    clock_t tStart = clock();

    vector<string> wordle_answer = read_wordle_answer();
    vector<string> wordle_guess = read_wordle_guess();
    vector<int> word_frequency = read_worde_frequency();

    vector<pair<int, string>> word_with_fre;
    for (int i = 0; i < wordle_guess.size(); i++)
        word_with_fre.push_back({word_frequency[i], wordle_guess[i]});
    sort(word_with_fre.begin(), word_with_fre.end());
    reverse(word_with_fre.begin(), word_with_fre.end());

    map<string, int> mp;
    for (int i = 0; i < word_with_fre.size(); i++)
    {
        // cout << word_with_fre[i].second << " " << word_with_fre[i].first << endl;
        mp[word_with_fre[i].second] = i + 1;
    }

    int mx = 0;
    vector<int> ord;
    for (int i = 0; i < wordle_answer.size(); i++)
    {
        mx = max(mx, mp[wordle_answer[i]]);

        ord.push_back(mp[wordle_answer[i]]);
    }
    cout << endl
         << endl;
    cout << mx << endl;
    sort(ord.begin(), ord.end());
    for (auto x : ord)
        cout << x << endl;

    printf("\n\nTime taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

    return 0;
}