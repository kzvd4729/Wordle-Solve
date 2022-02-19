#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <iomanip>

using namespace std;

const int WS = 5;

template <typename Data>
vector<Data> read_input(string path)
{
    ifstream cin(path);
    vector<Data> vec;
    Data s;
    while (cin >> s)
    {
        // for (auto &x : s)
        //     x = x - 'a' + 'A';
        vec.push_back(s);
    }
    // shuffle(wordle_answer.begin(), wordle_answer.end(), rng);
    //  ret.resize(1000);

    return vec;
}

string make_lower(string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] - 'A' + 'a';
    }
    return s;
}

void print_word_with_color(string word, string color)
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

void print_word_with_color_(string word, string color)
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
}

string feedback(string guess, string ans)
{
    guess = make_lower(guess);
    ans = make_lower(ans);

    string color(WS, 'b');
    vector<int> vis(WS, 0);
    for (int i = 0; i < ans.size(); i++)
    {
        if (ans[i] == guess[i])
        {
            color[i] = 'g';
            vis[i] = 1;
        }
    }
    for (int i = 0; i < guess.size(); i++)
    {
        if (color[i] == 'g')
            continue;
        for (int j = 0; j < ans.size(); j++)
        {
            if (vis[j])
                continue;
            if (guess[i] == ans[j])
            {
                color[i] = 'y';
                vis[j] = 1;
                break;
            }
        }
    }
    return color;
}

vector<string> sort_dict_by_freq(vector<string> &dict, vector<int> &freq)
{
    vector<pair<int, string>> tmp;
    if (dict.size() != freq.size())
    {
        cout << "Somthing is not right!!" << endl;
        assert(0);
    }

    for (int i = 0; i < dict.size(); i++)
        tmp.push_back({freq[i], dict[i]});

    sort(tmp.begin(), tmp.end());
    reverse(tmp.begin(), tmp.end());

    vector<string> ret;
    for (int i = 0; i < tmp.size(); i++)
        ret.push_back(tmp[i].second);
    return ret;
}

string find_best_guess(vector<string> &current, vector<string> &dict, vector<bool> bad)
{
    if (current.size() < 30)
    {
        return current[0];
    }

    vector<int> fr(26, 0);
    int ith = current.size();
    for (auto x : current)
    {
        for (auto c : x)
            fr[c - 'a'] += (ith / 3);
        ith--;
    }

    int best = -1;
    string ret = "";

    for (auto x : dict)
    {
        string copy = x;

        vector<bool> vs(26, 0);

        int score = 0;
        for (auto c : x)
        {
            if (vs[c - 'a'])
                continue;
            score += fr[c - 'a'] * (bad[c - 'a'] ^ 1);
            vs[c - 'a'] = 1;
        }

        if (score > best)
        {
            best = score;
            ret = copy;
        }
    }
    return ret;
}

vector<string> find_best5_guess(vector<string> &current, vector<string> &dict, vector<bool> bad)
{
    vector<string> ret;
    if (current.size() < 30)
    {
        for (int i = 0; i < min(5, (int)current.size()); i++)
            ret.push_back(current[i]);

        return ret;
    }

    vector<int> fr(26, 0);
    int ith = current.size();
    for (auto x : current)
    {
        for (auto c : x)
            fr[c - 'a'] += (ith / 3);
        ith--;
    }

    vector<pair<int, string>> vec;
    for (auto x : dict)
    {
        string copy = x;

        vector<bool> vs(26, 0);

        int score = 0;
        for (auto c : x)
        {
            if (vs[c - 'a'])
                continue;
            score += fr[c - 'a'] * (bad[c - 'a'] ^ 1);
            vs[c - 'a'] = 1;
        }
        vec.push_back({score, copy});
    }
    sort(vec.begin(), vec.end());
    reverse(vec.begin(), vec.end());

    for (int i = 0; i < min(5, (int)vec.size()); i++)
        ret.push_back(vec[i].second);

    return ret;
}

int play_with_computer(string hidden, vector<string> &dict, bool one)
{
    vector<string> current = dict;
    vector<bool> bad(26, 0);
    for (int i = 1;; i++)
    {
        string guess = find_best_guess(current, dict, bad);

        string color = feedback(guess, hidden);

        for (int j = 0; j < WS; j++)
        {
            if ((color[j] == 'g') || (color[j] == 'y'))
                bad[guess[j] - 'a'] = 1;
        }

        if (one)
            print_word_with_color(guess, color);

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

void test_accuracy(vector<string> &test, vector<string> &dict)
{
    double total = 0;
    vector<int> frequency(100, 0);

    int cnt = 0;
    vector<int> percent;
    for (double i = 0.1; i <= 1; i += 0.1)
        percent.push_back((int)(test.size() * i));
    percent.push_back(-1);

    int pt = 0;
    for (auto x : test)
    {
        int gs = play_with_computer(x, dict, 0);
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
    cout << "Words = " << test.size() << endl;
    cout << "Average = " << total / test.size() << endl;

    cout << setprecision(4) << fixed;
    int acc = 0;
    for (int i = 1; i < frequency.size(); i++)
    {
        if (frequency[i])
        {
            acc += frequency[i];
            cout << i << " --> " << (double)acc / (double)test.size() << "%" << endl;
        }
    }
}

void print_instructions()
{
    cout << "Instructions: The program will suggest 5 best possible words for you to start. You can choose one of those or any other valid word. After that you have tell the program the word you entered in the wordle game and color you got." << endl;
    cout << "Enter 'B' for Black, 'G' for Green and 'Y' for Yellow" << endl;
    cout << endl;
    cout << "For example: " << endl;
    cout << "You have entered MAKER and got color ";
    print_word_with_color("maker", "byggb");

    cout << "Then enter: MAKER BYGGB" << endl;
    cout << endl;
    cout << "You can enter everything in Upper Case or Lower Case letters." << endl;
    // cout << " --> byggb" << endl;

    cout << endl
         << endl
         << endl;
}

bool validate_word(string s, vector<string> &dict)
{
    string msg = "Word is not correct. Try again.\n";
    if (s.size() != 5)
    {
        cout << msg << endl;
        return false;
    }
    for (auto x : dict)
    {
        if (x == s)
            return true;
    }
    cout << msg << endl;
    return false;
}
bool validate_color(string s)
{
    string msg = "Color is not correct. Try again.\n";
    if (s.size() != 5)
    {
        cout << msg << endl;
        return false;
    }
    for (auto x : s)
    {
        if ((x != 'b') && (x != 'g') && (x != 'y'))
        {
            cout << msg << endl;
            return false;
        }
    }
    return true;
}

void play_with_human(vector<string> &dict)
{
    // string ans = "heavy";
    print_instructions();

    vector<string> current = dict;
    vector<bool> bad(26, 0);
    vector<pair<string, string>> board;
    for (int i = 1;; i++)
    {
        vector<string> guess_list = find_best5_guess(current, dict, bad);

        string guess, color;
        while (true)
        {
            cout << "Try one of those: (";
            print_word_with_color_(guess_list[0], "bbbbb");
            for (int i = 1; i < guess_list.size(); i++)
            {
                cout << ", ";
                print_word_with_color_(guess_list[i], "bbbbb");
            }
            cout << ")" << endl;
            cout << "Enter word and color: ";

            cin >> guess >> color;

            // {
            //     cin >> guess;
            //     color = feedback(guess, ans);
            // }
            guess = make_lower(guess);
            color = make_lower(color);

            if (validate_word(guess, dict) && validate_color(color))
                break;
        }

        board.push_back({guess, color});

        cout << "\nBoard so far:" << endl;
        for (auto x : board)
            print_word_with_color(x.first, x.second);
        cout << endl;

        if (color == "ggggg")
        {
            cout << "Congratulations!! You've solved it." << endl;
            return;
        }

        for (int j = 0; j < WS; j++)
        {
            if ((color[j] == 'g') || (color[j] == 'y'))
                bad[guess[j] - 'a'] = 1;
        }

        vector<string> tmp;
        for (auto x : current)
        {
            if (feedback(guess, x) == color)
                tmp.push_back(x);
        }

        if (tmp.size() == 0)
        {
            cout << "Found no words in the dictionary with your specifications. Please re-run the program." << endl;
            return;
        }

        current = tmp;
    }
}

int main()
{
    clock_t tStart = clock();
    cout << endl;

    vector<string> dict = read_input<string>("resources/big_list.txt");
    vector<string> test = read_input<string>("resources/small_list.txt");
    vector<int> freq = read_input<int>("resources/big_list_frequency.txt");

    dict = sort_dict_by_freq(dict, freq);

    // test_accuracy(test, dict);
    //  play_with_computer("curch", dict, 1);

    play_with_human(dict);

    printf("\n\nTime taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

    return 0;
}