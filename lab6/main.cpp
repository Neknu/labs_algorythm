#include<iostream>
#include <functional>
#include <chrono>
#include <random>
using namespace std;

int rand_num(double max) {
    static std::random_device rd;
    static std::seed_seq seed { rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    std::uniform_real_distribution<double> dist(0, max);
    return dist(gen);
}

string rand_string(int N) {
    string res;
    for(int i = 0; i < N; i++)
        res += char(65+ rand_num(5));
    return res;
}

string swap_string(string s) {
    int first, second;
    int count_of_swaps = rand_num(s.length()) + s.length();
    for(int i = 0; i < count_of_swaps; i++) {
       first = rand_num(s.length());
       second = rand_num(s.length());
       swap(s[first], s[second]);
    }
    return s;
}

void getZarr(string str, int Z[]);

bool search(const string& text, const string& pattern)
{
    string concat = pattern + "$" + text + text;
    int l = concat.length();

    int Z[l];
    getZarr(concat, Z);

    for (int i = 0; i < l; ++i)
    {
        if (Z[i] == pattern.length())
            return true;
    }
    return false;
}

void getZarr(string str, int Z[])
{
    int n = str.length();
    int L, R, k;
    L = R = 0;
    for (int i = 1; i < n; ++i)
    {
        if (i > R)
        {
            L = R = i;

            while (R<n && str[R-L] == str[R])
                R++;
            Z[i] = R-L;
            R--;
        }
        else
        {

            k = i-L;

            if (Z[k] < R-i+1)
                Z[i] = Z[k];

            else
            {
                L = i;
                while (R<n && str[R-L] == str[R])
                    R++;
                Z[i] = R-L;
                R--;
            }
        }
    }
}

int main()
{
    for(int i = 0; i < 100; i++) {
        string text = rand_string(5);
        string pattern = swap_string(text);
        if (search(text, pattern) && text.length() == pattern.length())
            cout << "True - " << pattern << " is a cycle string for " << text << endl;
        else
            cout << "False - " << pattern << " is not a cycle string for " << text << endl;
    }
    return 0;
} 