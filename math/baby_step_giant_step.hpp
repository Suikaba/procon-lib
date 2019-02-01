using ll = long long;

ll modpow(ll x, ll n, ll mod) {
    ll res = 1;
    while(n > 0) {
        if(n & 1) (res *= x) %= mod;
        (x *= x) %= mod;
        n >>= 1;
    }
    return res;
}

// Problem: find x s.t. a^x = b (mod p)
// Output: x (if not exists such x, return -1)
// Complexity: O(sqrt(p)log(p))
// Requirments: p is prime number
ll baby_step_giant_step(ll a, ll b, ll p) {
    ll res = -1;
    const ll m = std::sqrt(p) + 1; // ceil(sqrt(p))
    std::map<ll, ll> table;
    for(ll i = 0, z = 1, t = modpow(a, m, p); i < m; ++i) {
        table[z] = i;
        z = (z * t) % p;
    }

    ll z = b * modpow(a, p - m, p) % p;
    for(int i = m - 1; i >= 0; --i) {
        if(table.count(z) == 1) {
            res = m * table[z] + i;
            break;
        }
        z = (z * a) % p;
    }

    return res;
}
