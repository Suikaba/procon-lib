class rolling_hash {
    using ll = long long;
public:
    rolling_hash(std::string const& s)
        : n(s.size()), hs1(n + 1), hs2(n + 1), p1(n + 1, 1), p2(n + 1, 1)
    {
        for(int i = 0; i < n; ++i) {
            hs1[i + 1] = (hs1[i] * b1 + s[i]) % mod1;
            hs2[i + 1] = (hs2[i] * b2 + s[i]) % mod2;
            p1[i + 1] = p1[i] * b1 % mod1;
            p2[i + 1] = p2[i] * b2 % mod2;
        }
    }

    std::pair<ll, ll> query(int l, int r) const {
        return std::make_pair(((hs1[r] - hs1[l] * p1[r - l]) % mod1 + mod1) % mod1,
                              ((hs2[r] - hs2[l] * p2[r - l]) % mod2 + mod2) % mod2);
    }

private:
    const int n;
    std::vector<ll> hs1, hs2, p1, p2;
    static const ll b1 = 17LL;
    static const ll b2 = 19LL;
    static const ll mod1 = 999999937LL;
    static const ll mod2 = 1000000007LL;
};

