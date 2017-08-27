
// 1-indexed
// s[1...n]
class rolling_hash {
public:
    rolling_hash(std::string const& s)
        : n(s.size()),
          mod({999999937LL, 1000000007LL})
    {
        for(int i = 0; i < 2; ++i) {
            hs[i].resize(n + 1);
            p[i].resize(n + 1);
            hs[i][0] = 0;
            p[i][0] = 1;
            for(int j = 0; j < n; ++j) {
                hs[i][j + 1] = (hs[i][j] * base + s[j]) % mod[i];
                p[i][j + 1] = p[i][j] * base % mod[i];
            }
        }
    }

    // s[i...]
    long long query(int idx, int i) const {
        return hs[i][idx];
    }
    // s[l + 1...r]
    long long query(int l, int r, int i) const {
        return ((hs[i][r] - hs[i][l] * p[i][r - l]) % mod[i] + mod[i]) % mod[i];
    }

private:
    int n;
    std::vector<long long> hs[2];
    std::vector<long long> p[2];

    const std::array<long long, 2> mod;
    static const long long base = 29LL;
};

