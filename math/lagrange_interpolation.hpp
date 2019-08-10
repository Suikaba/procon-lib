using ll = long long;

ll modpow(ll x, ll n, const ll mod) {
    ll res = 1;
    while(n > 0) {
        if(n & 1) (res *= x) %= mod;
        (x *= x) %= mod;
        n >>= 1;
    }
    return res;
}
ll inverse(ll x, const ll mod) {
    return modpow(x, mod - 2, mod);
}

// calc c[n - 1], ..., c[0],
//   where P(x) := c[n-1]*x^{p-1} + ... + c[0], P(xs[i]) = ys[i] (mod m) (i = 0, \ldots, n - 1)
// Complexity: O(n^2)
std::vector<ll> lagrange_interpolation(std::vector<ll> xs, std::vector<ll> ys, const int m) {
    const int n = xs.size();
    for(int i = 0; i < n; ++i) {
        xs[i] %= m;
        ys[i] %= m;
    }
    std::vector<ll> all_c(n + 1);
    all_c[0] = 1;
    for(int i = 0; i < n; ++i) {
        std::vector<ll> nxt(n + 1);
        for(int j = 0; j < n; ++j) {
            nxt[j + 1] = all_c[j];
        }
        for(int j = 0; j < n; ++j) {
            nxt[j] = (m + nxt[j] - xs[i] * all_c[j] % m) % m;
        }
        all_c = std::move(nxt);
    }

    std::vector<ll> c(n);
    for(int i = 0; i < n; ++i) {
        ll qi = 1;
        for(int j = 0; j < n; ++j) {
            if(i == j) continue;
            qi = qi * (m + xs[i] - xs[j]) % m;
        }
        qi = inverse(qi, m) * ys[i] % m;

        auto tmp_c = all_c;
        for(int j = n - 1; j >= 0; --j) {
            c[j] = (c[j] + qi * tmp_c[j + 1]) % m;
            tmp_c[j] = (tmp_c[j] + tmp_c[j + 1] * xs[i]) % m;
        }
    }
    return c;
}
