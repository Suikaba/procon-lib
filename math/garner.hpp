using ll = long long;

ll mod_inv(ll a, ll m) {
    ll b = m, u = 1, v = 0;
    while(b > 0) {
        ll t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    return (u % m + m) % m;
}

// solve: find x which statisfies (x mod m[i]) == u[i]
// require: m[i] are co-prime
ll garner(std::vector<ll> m, std::vector<ll> u, int mod) {
    const int n = m.size();
    std::vector<ll> inv_prod(n);
    for(int i = 1; i < n; ++i) {
        ll prod = m[0] % m[i];
        for(int j = 1; j < i; ++j) {
            prod = (prod * m[j]) % m[i];
        }
        inv_prod[i] = mod_inv(prod, m[i]);
    }

    std::vector<ll> v(n);
    v[0] = u[0];
    for(int i = 1; i < n; ++i) {
        ll tmp = v[i - 1];
        for(int j = i - 2; j >= 0; --j) {
            tmp = (tmp * m[j] + v[j]) % m[i];
        }
        v[i] = ((u[i] - tmp) * inv_prod[i]) % m[i];
        if(v[i] < 0) v[i] += m[i];
    }

    ll res = v[n - 1];
    for(int i = n - 2; i >= 0; --i) {
        res = (res * m[i] + v[i]) % mod;
    }
    return res;
}
