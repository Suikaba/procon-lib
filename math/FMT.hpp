using ll = long long;

int mod_inv(ll a, ll m) {
    ll b = m, u = 1, v = 0;
    while(b > 0) {
        ll t = a / b;
        a -= t * b; std::swap(a, b);
        u -= t * v; std::swap(u, v);
    }
    return (u % m + m) % m;
}

// 998244353 = 7 * 17 * 2^23 + 1
// PrimitiveRoot == 31 => need v.size() < 2^23 ~ 8.0 * 10^6
template <typename Polynomial, int Mod = 998244353, int PrimitiveRoot = 31>
class FMT {
    static int omega(int idx) {
        static const int omega_max = 23;
        assert(PrimitiveRoot == 31);
        assert(0 <= idx && idx <= omega_max);
        static std::vector<int> ws;
        if(ws.empty()) {
            ws.resize(omega_max + 1);
            for(int x = PrimitiveRoot, i = omega_max; i >= 0; --i) {
                ws[i] = x;
                x = (1LL * x * x) % Mod;
            }
        }
        return ws[idx];
    }
public:
    // assertion: v.size() == 2 ^ m
    static Polynomial fft(Polynomial v, bool inv) {
        const int n = v.size();
        assert((n ^ (n & -n)) == 0);
        int ww = [&] () {
                     int logn = 0, sz = 1;
                     while(sz < n) sz *= 2, logn++;
                     return omega(logn);
                 }();
        if(inv) ww = mod_inv(ww, Mod);
        for(int m = n; m >= 2; m >>= 1) {
            const int mh = m >> 1;
            int w = 1;
            for(int i = 0; i < mh; ++i) {
                for(int j = i; j < n; j += m) {
                    const int k = j + mh;
                    int x = v[j] - v[k];
                    if(x < 0) x += Mod;
                    v[j] += v[k];
                    if(v[j] >= Mod) v[j] -= Mod;
                    v[k] = (1LL * w * x) % Mod;
                }
                w = (1LL * w * ww) % Mod;
            }
            ww = (1LL * ww * ww) % Mod;
        }

        for(int i = 0, j = 1; j < n - 1; ++j) {
            for(int k = n >> 1; k > (i ^= k); k >>= 1);
            if(j < i) std::swap(v[i], v[j]);
        }
        if(inv) {
            const int inv_n = mod_inv(n, Mod);
            for(int j = 0; j < (int)v.size(); ++j) {
                v[j] = (1LL * v[j] * inv_n) % Mod;
            }
        }
        return v;
    }
    static Polynomial convolution(Polynomial f, Polynomial g) {
        int sz = 1;
        const int m = f.size() + g.size() - 1;
        while(sz < m) sz *= 2;
        f.resize(sz), g.resize(sz);
        f = fft(std::move(f), false); g = fft(std::move(g), false);
        for(int i = 0; i < sz; ++i) {
            f[i] = (1LL * f[i] * g[i]) % Mod;
        }

        return fft(std::move(f), true);
    }
};
