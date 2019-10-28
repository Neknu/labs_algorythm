#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <chrono>

using std::string;
using std::cout;
using std::endl;

#define d 256
#define q 101

string bin_to_str(const string& data) {
    std::stringstream sstream(data);
    string output;
    while(sstream.good())
    {
        std::bitset<8> bits;
        sstream >> bits;
        char c = char(bits.to_ulong());
        output += c;
    }
    return output.substr(0,output.length() - 1); // delete last empty symbol
}

void search_naive(string pat, string txt)
{
    int M = pat.length();
    int N = txt.length();

    for (int i = 0; i <= N - M; i++) {
        int j;

        for (j = 0; j < M; j++)
            if (txt[i + j] != pat[j])
                break;

        if (j == M) j = j;
//            cout << "Pattern found at index "
//                 << i << endl;
    }
}

void search_karp(const string& pat, const string& txt)
{
    int M = pat.length();
    int N = txt.length();
    int i, j;
    int hash_pattern= 0;
    int hash_text = 0;
    int h = 1;

    for (i = 0; i < M - 1; i++)
        h = (h * d) % q;

    for (i = 0; i < M; i++)
    {
        hash_pattern= (d * hash_pattern+ pat[i]) % q;
        hash_text = (d * hash_text + txt[i]) % q;
    }

    for (i = 0; i <= N - M; i++)
    {

        if ( hash_pattern== hash_text )
        {

            for (j = 0; j < M; j++)
            {
                if (txt[i+j] != pat[j])
                    break;
            }

            if (j == M) j = j;
//                cout << "Pattern found at index "
//                     << i << endl;
        }

        if ( i < N-M )
        {
            hash_text = (d*(hash_text - txt[i]*h) + txt[i+M])%q;

            if (hash_text < 0)
                hash_text = (hash_text + q);
        }
    }
}

void getZarr(string str, int Z[]);

void search_zfunction(const string& pattern, const string& text)
{
    string concat = pattern + "$" + text;
    int l = concat.length();

    int Z[l];
    getZarr(concat, Z);

    for (int i = 0; i < l; ++i)
    {
        if (Z[i] == pattern.length()) i = i;
//            cout << "Pattern found at index "
//                 << i - pattern.length() - 1 << endl;
    }
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
    string test_bin = "01110100011001010111001101110100";
    for(int i = 0 ; i < 10; i++) {
        test_bin += test_bin;
    }
    string es_bin = "0110010101110011";
    string test = bin_to_str(test_bin);
    string es = bin_to_str(es_bin);

    cout << test << endl;
    cout << es << endl;

    float t;
    using namespace std::chrono;
    duration<double> time_span;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    cout << "NAIVE algorythm:" << endl;
    search_naive(es_bin, test_bin);
    cout << endl;
    search_naive(es, test);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t2 - t1);
    t = time_span.count();
    cout << "NAIVE tooks me " <<t << "seconds" << endl;
    cout << endl;

    t1 = high_resolution_clock::now();
    cout << "RABIN-KARP algorythm:" << endl;
    search_karp(es_bin, test_bin);
    cout << endl;
    search_karp(es, test);
    t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t2 - t1);
    t = time_span.count();
    cout << "KARP tooks me " <<t << "seconds" << endl;
    cout << endl;

    t1 = high_resolution_clock::now();
    cout << "Z-function algorythm:" << endl;
    search_zfunction(es_bin, test_bin);
    cout << endl;
    search_zfunction(es, test);
    t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t2 - t1);
    t = time_span.count();
    cout << "Z-function tooks me " <<t << "seconds" << endl;
    cout << endl;
   return 0;
}