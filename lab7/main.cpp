#include <bits/stdc++.h>
using namespace std;

// d is the number of characters in the input alphabet  
#define d 256
#define q 101

int search(const string& pat, const string& txt)
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

            if (j == M)
                return i;
        }

        if ( i < N-M )
        {
            hash_text = (d*(hash_text - txt[i]*h) + txt[i+M])%q;

            if (hash_text < 0)
                hash_text = (hash_text + q);
        }
    }
    return -1;
}

std::pair<int,int> search_in_matrices(string pattern[], string text[])
{
    int index;

    for (int i = 0; i < (int) text[0].length(); ++i)
    {
        if (text[0].length() - i < pattern[0].length())
        {
            return std::make_pair(-1,-1);;
        }

        index = search(pattern[0], text[i]);

        while (index >= 0)
        {
            int counter = 1;

            for (int j = i + 1; j < (int) text[0].length(); j++)
            {
                if (pattern[counter] == text[j].substr(index, pattern[0].length()))
                {
                    counter++;
                } else {
                    break;
                }

                if (counter == pattern[0].length())
                {
                    return std::make_pair(i,index);
                }
            }

            index = search(pattern[0], text[i].substr(index + 1, text[0].length() - index - 1));
        }
    }
    return std::make_pair(-1,-1);
}

int main()
{

    int N;
    int M;

    cout << "Enter N (size of text array NxN)" << endl;
    cin >> N;
    cout << "Enter M (size of pattern array MxM)" << endl;
    cin >> M;

    auto *text = new string[N];
    auto *pattern = new string[M];

    cout << "Enter an text array NxN" << endl;
    for (int i = 0; i < N; ++i)
    {
        cin >> text[i];
    }

    cout << "Enter an pattern array MxM" << endl;
    for (int i = 0; i < M; ++i)
    {
        cin >> pattern[i];
    }

    std::pair<int,int> result = search_in_matrices(pattern, text);

    cout << "Finded on " << "i: " << result.first << " j: " << result.second;

    return 0;
}  