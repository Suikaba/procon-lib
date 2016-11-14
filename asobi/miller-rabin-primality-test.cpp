#include <iostream>
#include <limits>
#include <random>
using namespace std;
using ll = long long;


ll modpow(ll x, ll n, const ll mod) {
    ll res = 1;
    while(n > 0) {
        if(n & 1) {
            res = (__int128)res * x % mod;
        }
        x = (__int128)x * x % mod;
        n >>= 1;
    }
    return res;
}

// cf fermat test
// check a^n === a (mod n)
// return n is prime number(not exactly)
bool fermat_test(ll n) {
    if(n == 2) {
        return true;
    }
    const int cnt = 4; // check count
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<ll> uid(2, n-1);
    bool res = true;
    for(int i=0; i<cnt; ++i) {
        ll a = uid(mt);
        res &= modpow(a, n-1, n) == 1;
    }
    return res;
}

bool miller_rabin(ll n) {
    if(n == 2) {
        return true;
    } else if(n % 2 == 0) {
        return false;
    }
    const int cnt = 4;
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<ll> uid(2, n-2);
    auto factor2 = [](ll n) -> pair<ll, ll> {
        ll s = 0, d = 0;
        while(n % 2 == 0) {
            n /= 2;
            s++;
        }
        d = n;
        return make_pair(s, d);
    };
    bool res = true;
    auto sd = factor2(n-1);
    for(int i=0; i<cnt; ++i) {
        bool res2 = false;
        ll a = uid(mt);
        ll a_exp_d = modpow(a, sd.second, n);
        res2 |= a_exp_d == 1;
        if(!res2) {
            for(int j=0; j<sd.first; ++j) {
                if(a_exp_d == n-1) {
                    res2 = true;
                    break;
                }
                a_exp_d = modpow(a_exp_d, 2, n);
            }
        }
        res &= res2;
    }
    return res;
}


int main() {
    cout << "Miller-Rabin primality test" << endl;
    cout << "input: natural number n. n < INT_MAX/2" << endl;
    cout << "If you input non-natural, exit.\n" << endl;
    ll n;
    while(cin >> n, n > 0) {
        if(n < 2 || numeric_limits<ll>::max() / 2 < n) {
            break;
        }
        cout << "fermat test:  ";
        if(fermat_test(n)) {
            cout << "prime number" << endl;
        } else {
            cout << "not prime" << endl;
        }
        cout << "miller rabin test:  ";
        if(miller_rabin(n)) {
            cout << "prime number" << endl;
        } else {
            cout << "not prime" << endl;
        }
        cout << endl;
    }
    cout << "exit" << endl;
}
