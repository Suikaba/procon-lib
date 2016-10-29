
class rolling_hash {
public:
    rolling_hash(std::string const& s)
        : n(s.size()),
          mod({1000000000000037LL, 1000000000000091LL})
    {
        for(int i=0; i<2; ++i) {
            hs[i].assign(n+1, 0);
            p[i].assign(n+1, 1);
            for(int j=0; j<n; ++j) {
                hs[i][j+1] = (hs[i][j] * (__int128)base + s[j]) % mod[i];
                p[i][j+1] = p[i][j] * (__int128)base % mod[i];
            }
        }
    }

    // (l, r]
    long long query(int l, int r, int i) {
        return ((hs[i][r] - (__int128)hs[i][l]*p[i][r-l]) % mod[i] + mod[i]) % mod[i];
    }

private:
    int n;
    std::vector<long long> hs[2];
    std::vector<long long> p[2];

    const std::array<long long, 2> mod;
    static const long long base = 29LL;
};
