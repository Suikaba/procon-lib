
class kitamasa {
public:
    // d: coefficient
    kitamasa(std::vector<long long> const& d_, long long m = 1e9+7)
        : k(d_.size()),
          d(d_),
          res(d_.size(), 0),
          res_2k(2*k-1)
          M(m)
    {
        // [0, k)
        for(int i=0; i<k; ++i) {
            res_2k[i].resize(k, 0);
            res_2k[i][i] = d[i];
        }
        // [k, k+1)
        res_2k[k].resize(k, 0);
        std::copy(d.begin(), d.end(), res_2k[k].begin());

        // [k+1, 2k-1)
        std::copy(d.begin(), d.end(), res.begin());
        for(int i=k+1; i<2*k-1; ++i) {
            res_2k[i].resize(k, 0);
            inc();
            for(int j=0; j<k; ++j) {
                res_2k[i][j] = res[j];
            }
        }
    }

    void inc() {
        std::rotate(res.begin(), res.begin()+(k-1), res.begin()+k);
        long long l = res[0];
        res[0] = 0;
        for(int i=0; i<k; ++i) {
            res[i] += d[i] * l;
            res[i] %= M;
        }
    }
    void dbl() { 
        std::vector<long long> buf(2*k-1, 0);
        for(int i=0; i<k; ++i) {
            for(int j=0; j<k; ++j) {
                buf[i+j] += res[i]*res[j];
                buf[i+j] %= M;
            }
        }
        for(int i=k; i<k*2-1; ++i) {
            for(int j=0; j<k; ++j) {
                buf[j] += buf[i]*res_2k[i][j];
                buf[j] %= M;
            }
        }
        std::copy(buf.begin(), buf.begin()+k, res.begin());
    }

    // calc a_n
    long long execute(long long n) const {
        std::fill(res.begin(), res.end(), 0);
        res[0] = 1;
        for(int i=62; i>=0; --i) { // sometimes i=32 ok
            dbl();
            if(n & (1LL << i)) {
                inc();
            }
        }

        long long ans = 0;
        for(int i=0; i<k; ++i) {
            ans += res[i]*d[i];
            ans %= M;
        }
        return ans;
    }

private:
    int k;
    std::vector<long long> d, res;
    std::vector<std::vector<long long>> res_2k;
    long long M;
};
