// ここは変わることがあるかも？
constexpr int mod = 7 * 17 * (1 << 23) + 1; // == 998244353 !!

//int mod_inv(ll a, ll m) {
//    ll b = mod, u = 1, v = 0;
//    while(b > 0) {
//        ll t = a / b;
//        a -= t * b; swap(a, b);
//        u -= t * v; swap(u, v);
//    }
//    return (u % m + m) % m;
//}

class FMT {
public:
    FMT() {
        omega.resize(omega_max + 1);
        int x = primitive_root;
        for(int i = omega_max; i >= 0; --i) {
            omega[i] = x;
            x = (1LL * x * x) % mod;
        }
    }

    // assertion: v.size() == 2 ^ m
    std::vector<int> fft(std::vector<int> v, bool inv) const {
        const int n = v.size();
        int logn = 0;
        {
            int sz = 1;
            while(sz < n) sz *= 2, logn++;
        }
        assert(1 << logn == n);
        assert(omega_max >= logn);
        int ww = omega[logn];
        if(inv) ww = mod_inv(ww, mod);
        for(int m = n; m >= 2; m >>= 1) {
            const int mh = m >> 1;
            int w = 1;
            for(int i = 0; i < mh; ++i) {
                for(int j = i; j < n; j += m) {
                    const int k = j + mh;
                    int x = v[j] - v[k];
                    if(x < 0) x += mod;
                    v[j] += v[k];
                    if(v[j] >= mod) v[j] -= mod;
                    v[k] = (1LL * w * x) % mod;
                }
                w = (1LL * w * ww) % mod;
            }
            ww = (1LL * ww * ww) % mod;
        }

        int i = 0;
        for(int j = 1; j < n - 1; ++j) {
            for(int k = n >> 1; k > (i ^= k); k >>= 1);
            if(j < i) swap(v[i], v[j]);
        }
        if(inv) {
            const int inv_n = mod_inv(n, mod);
            for(auto& x : v) {
                x = (1LL * x * inv_n) % mod;
            }
        }
        return v;
    }
    std::vector<int> convolution(std::vector<int> f, std::vector<int> g) const {
        int sz = 1;
        const int m = f.size() + g.size() - 1;
        while(sz < m) sz *= 2;
        f.resize(sz), g.resize(sz);
        f = this->fft(std::move(f), false); g = this->fft(std::move(g), false);
        for(int i = 0; i < sz; ++i) {
            f[i] = (1LL * f[i] * g[i]) % mod;
        }

        return this->fft(std::move(f), true);
    }

private:
    static const int omega_max = 23;
    static const int primitive_root = 31;
    std::vector<int> omega;
};
