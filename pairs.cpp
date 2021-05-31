#include <iterator>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

// Va in time sui Test Case: # 10, 11, 12, 13, 14
int main() 
{
    int N, K; cin >> N >> K;
    vector<int> v(N);
    copy_n(istream_iterator<int>(cin), N, begin(v));
    
    auto cnt = 0;
    for (auto i : v)
    {
        for (auto j : v)
        {
            if (i-j == K)
                cnt++;
        }
    }   
    cout << cnt;
}

// Primo improvement: ammortizziamo il costo del ciclo più interno.
// Per ogni i, dobbiamo cercare j in questo modo:
// sapendo che 'i - j = K'
// allora 'j = i - K'
// per fare questa ricerca in maniera efficiente, ordiniamo il vettore
// e procediamo con ricerca binaria

int main() 
{
    int N, K; cin >> N >> K;
    vector<int> v(N);
    copy_n(istream_iterator<int>(cin), N, begin(v));
    
    sort(begin(v), end(v));
    
    auto cnt = 0;
    for (auto i : v)
    {
        if (binary_search(begin(v), end(v), i-K))
            cnt++;
    }
    cout << cnt;
}

// Secondo improvement: usiamo la libreria standard.
// Il ciclo for sta effettuando un "conteggio" di quanti elementi soddisfano
// una certa condizione.
// Possiamo usare un algoritmo standard che catturi questo concetto: count_if

int main() 
{
    int N, K; cin >> N >> K;
    vector<int> v(N);
    copy_n(istream_iterator<int>(cin), N, begin(v));
    sort(begin(v), end(v));
    cout << count_if(begin(v), end(v), [&](int i){
        return binary_search(begin(v), end(v), i-K);
    });
}

// Alternativa: usiamo una struttura dati per fare ricerche efficienti,
// come ad esempio unordered_set

int main() 
{
    int N, K; cin >> N >> K;
    unordered_set<int> v;
    copy_n(istream_iterator<int>(cin), N, inserter(v, end(v)));
    cout << count_if(begin(v), end(v), [&](int i){
       return v.count(i-K) > 0;  // > 0 only for clarity
    });
}

// Alternativa alla prima versione ottimizzata: possiamo ridurre via via lo spazio di ricerca.
// Osserviamo che se 'i - j = K', allora 'i = j + K'
// Se cerchiamo i invece che j possiamo fare una ricerca binaria in una spazio di ricerca
// che si "restringe" sempre di più perché in un vettore ordinato v[j] siamo certi che
// 'j + K' sarà sempre maggiore di '(j-1) + K'

// Questo approccio è davvero migliorativo? Perché non provi a fare qualche esperimento/misurazione?
// E se invece di semplici int, avessimo usato delle strutture più grandi?

int main() 
{
    int N, K; cin >> N >> K;
    vector<int> v(N);
    copy_n(istream_iterator<int>(cin), N, begin(v));
    
    sort(begin(v), end(v));
    
    auto cnt = 0;
    for (auto j = 0u; j<v.size(); ++j)
    {
        cnt += binary_search(begin(v) + j + 1, end(v), v[j] + K);
    }
    cout << cnt;
}
