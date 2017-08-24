
// M が固定で，速度が必要なら，M をグローバルに constexpr で定義するとよい．
class kitamasa {
public:
    // d: 係数
    kitamasa(std::vector<long long> const& d_, long long m = 1e9 + 7)
        : k(d_.size()),
          d(d_),
          M(m)
    {}

    void inc(std::vector<long long>& v) {
        std::rotate(v.begin(), v.begin() + (k - 1), v.begin() + k);
        long long l = v[0];
        v[0] = 0;
        for(int i = 0; i < k; ++i) {
            (v[i] += d[i] * l) %= M;
        }
    }
    void dbl(std::vector<long long>& v) { 
        auto buf = v;
        std::vector<long long> res(k);
        for(int i = 0; i < k; ++i) {
            for(int j = 0; j < k; ++j) {
                (res[j] += buf[j] * v[i]) %= M;
            }
            inc(buf);
        }
        v = std::move(res);
    }

    // calc a_n
    long long calc(long long n) {
        std::vector<long long> res(k);
        res[0] = 1;
        int j = 63;
        while(!(n & (1 << j))) {
            --j;
        }
        for(int i = j + 1; i >= 0; --i) {
            dbl(res);
            if(n & (1LL << i)) {
                inc(res);
            }
        }

        long long ans = 0;
        for(int i = 0; i < k; ++i) {
            (ans += res[i] * d[i]) %= M;
        }
        return ans;
    }

private:
    int k;
    std::vector<long long> d;
    long long const M = 1e9 + 7;
};

