
template <int M, bool IsPrime = false>
class modulo {
    using ll = long long;
public:
    modulo() : n(0) {}
    modulo(int m) : n(m) {
        if(n >= M) {
            n %= M;
        } else if(n < 0) {
            n = (n % M + M) % M;
        }
    }
    modulo(ll m) {
        if(m >= M) {
            m %= M;
        } else if(m < 0) {
            m = (m % M + M) % M;
        }
        n = m;
    }

    explicit operator int() const {
        return n;
    }
    explicit operator ll() const {
        return n;
    }
    bool operator==(modulo const& a) const {
        return n == a.n;
    }

    modulo& operator+=(modulo const& a) {
        n += a.n;
        if(n >= M) {
            n -= M;
        }
        return *this;
    }
    modulo& operator-=(modulo const& a) {
        n -= a.n;
        if(n < 0) {
            n += M;
        }
        return *this;
    }
    modulo& operator*=(modulo const& a) {
        n = (ll(n) * a.n) % M;
        return *this;
    }
    modulo operator^(int n) const {
        if(n == 0) {
            return modulo(1);
        }
        const modulo a = *this;
        modulo res = (a * a) ^ (n / 2);
        return n % 2 ? res * a : res;
    }
   
    // for C++11, enable_if_t -> typename enable_if::type
    std::enable_if_t<IsPrime, modulo> operator/(modulo const& a) const {
        return *this * modulo(inv(ll(a), M));
    }


private:
    int n;
    // ax %% 1 (mod p)
    // ex: 5x %% 1 (mod 13)
    // つまり 5x + 13y = 1
    // 両辺mod5を取れば，13y %% 1 (mod 5) つまり 3y %% 1 (mod 5)になる．
    // x = (1 - 13y) / 5 + 13 (+13は正にしたいから)なので，こういうyがほしいが，
    // 先ほどの 3y %% 1 (mod 5) 解けば良い．これはつまり，inv(13%5, 5) である．
    // ゆえに，x = (1 - 13*inv(13%5, 5))/5 + 13 となる．
    static std::enable_if_t<IsPrime, ll> inv(ll a, ll p) {
        return (a == 1 ? 1 : (1 - p*inv(p%a, a))/a + p);
    }
};

template <int M, bool IsPrime>
modulo<M, IsPrime> operator+(modulo<M, IsPrime> lhs, modulo<M, IsPrime> const& rhs) {
    lhs += rhs;
    return lhs;
}
template <int M, bool IsPrime>
modulo<M, IsPrime> operator-(modulo<M, IsPrime> lhs, modulo<M, IsPrime> const& rhs) {
    lhs -= rhs;
    return lhs;
}
template <int M, bool IsPrime>
modulo<M, IsPrime> operator*(modulo<M, IsPrime> lhs, modulo<M, IsPrime> const& rhs) {
    lhs *= rhs;
    return lhs;
}

// sw == false -> inv
template <int M>
modulo<M, true> fact(int n, bool sw = true) {
    static std::vector<modulo<M, true>> v1 = {1}, v2 = {1};
    if(n >= (int)v1.size()) {
        const int from = v1.size(), to = n + 1024;
        v1.reserve(to);
        v2.reserve(to);
        for(int i=from; i<to; ++i) {
            v1.push_back(v1.back() * modulo<M, true>(i));
            v2.push_back(v2.back() / modulo<M, true>(i));
        }
    }
    return sw ? v1[n] : v2[n];
}

template <int M>
modulo<M, true> comb(int a, int b) {
    if(b < 0 || b > a) {
        return fact<M>(0);
    }
    return fact<M>(a, true) * fact<M>(b, false) * fact<M>(a-b, false);
}


const int MOD = 1000000007;
using mod = modulo<MOD, true>;

