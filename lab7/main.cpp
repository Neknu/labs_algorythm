#include <bits/stdc++.h>
using namespace std;

// d is the number of characters in the input alphabet  
#define d 256

void search(char pat[], char txt[], int q)
{
    int M = strlen(pat);
    int N = strlen(txt);
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
                cout<<"Pattern found at index "<< i <<endl;
        }

        if ( i < N-M )
        {
            hash_text = (d*(hash_text - txt[i]*h) + txt[i+M])%q;

            if (hash_text < 0)
                hash_text = (hash_text + q);
        }
    }
}

int main()
{
    char txt[] = "GEEKS FOR GEEKS";
    char pat[] = "GEEK";
    int q = 101;
    search(pat, txt, q);
    return 0;
}  