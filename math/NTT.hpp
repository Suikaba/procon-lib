template <int Mod, int PrimitiveRoot>
class NTT {
public:
    // assertion: v.size() == 2 ^ m
    static std::vector<int> fft(std::vector<int> v, bool inv) {
        const int n = v.size();
        assert((n ^ (n & -n)) == 0);
        int ww = mod_pow(PrimitiveRoot, (Mod - 1) / n, Mod);
        if(inv) ww = mod_inv(ww, Mod);
        for(int m = n; m >= 2; m >>= 1) {
            const int mh = m >> 1;
            int w = 1;
            for(int i = 0; i < mh; ++i) {
                for(int j = i; j < n; j += m) {
                    const int k = j + mh;
                    int x = v[j] - v[k];
                    if(x < 0) x += Mod;
                    v[j] += - Mod + v[k];
                    if(v[j] < 0) v[j] += Mod;
                    v[k] = (1LL * w * x) % Mod;
                }
                w = (1LL * w * ww) % Mod;
            }
            ww = (1LL * ww * ww) % Mod;
        }

        int i = 0;
        for(int j = 1; j < n - 1; ++j) {
            for(int k = n >> 1; k > (i ^= k); k >>= 1);
            if(j < i) swap(v[i], v[j]);
        }
        if(inv) {
            const int inv_n = mod_inv(n, Mod);
            for(auto& x : v) {
                x = (1LL * x * inv_n) % Mod;
                assert(0 <= x && x < Mod);
            }
        }
        return v;
    }
    static std::vector<int> convolution(std::vector<int> f, std::vector<int> g) {
        int sz = 1;
        const int m = f.size() + g.size() - 1;
        while(sz < m) sz *= 2;
        f.resize(sz), g.resize(sz);
        f = NTT::fft(std::move(f), false); g = NTT::fft(std::move(g), false);
        for(int i = 0; i < sz; ++i) {
            f[i] = (1LL * f[i] * g[i]) % Mod;
        }

        return NTT::fft(std::move(f), true);
    }

    static int get_mod() {
        return Mod;
    }
};

using NTT_1 = NTT<167772161, 3>;  // 5 * 2^25 + 1
using NTT_2 = NTT<469762049, 3>;  // 7 * 2^26 + 1
using NTT_3 = NTT<1224736769, 3>; // 73 * 2^24 + 1

std::vector<int> mod_convolution(std::vector<int> f, std::vector<int> g, const int mod) {
    for(auto& x : f) x %= mod;
    for(auto& y : g) y %= mod;
    const auto v1 = NTT_1::convolution(f, g);
    const auto v2 = NTT_2::convolution(f, g);
    const auto v3 = NTT_3::convolution(f, g);

    vector<int> res(v1.size());
    vector<ll> m = {NTT_1::get_mod(), NTT_2::get_mod(), NTT_3::get_mod()};
    for(int i = 0; i < (int)v1.size(); ++i) {
        vector<ll> u = {v1[i], v2[i], v3[i]};
        res[i] = garner(m, u, mod);
    }

    return res;
}
