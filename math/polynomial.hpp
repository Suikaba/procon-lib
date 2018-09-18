using ll = long long;

template <typename VT>
class polynomial {
public:
    polynomial(int n) : coef(n) {}
    polynomial(std::vector<VT> const& c) : coef(c) {}

    int size() const { return coef.size(); }
    int degree() const { return coef.size(); } // return x^n => n + 1

    polynomial& operator+=(polynomial const& other) {
        const int d = resize(other.degree());
        for(int i = 0; i < d; ++i) coef[i] += other[i];
        canonicalize();
        return *this;
    }
    polynomial& operator-=(polynomial const& other) {
        const int d = resize(other.degree());
        for(int i = 0; i < d; ++i) coef[i] -= other[i];
        canonicalize();
        return *this;
    }
    polynomial& operator*=(polynomial const& other) { // verified
        coef = convolution(std::move(coef), other.coef); // require FFT
        canonicalize();
        return *this;
    }
    polynomial operator+(polynomial const& other) const { return polynomial(*this) += other; }
    polynomial operator-(polynomial const& other) const { return polynomial(*this) -= other; }
    polynomial operator*(polynomial const& other) const { return polynomial(*this) *= other; }

    void resize(int sz) {
        coef.resize(sz);
    }

    // require NTT
    std::enable_if_t<std::is_same<VT, int>::value, polynomial> inverse() {
        using fmt = FMT<polynomial>;
        {
            int sz = 1;
            while(sz < degree()) sz *= 2;
            coef.resize(sz);
        }
        const int n = degree();
        polynomial r(1); r[0] = mod_inv(coef[0], mod);
        for(int k = 2; k <= n; k <<= 1) {
            auto nr = fmt::convolution(fmt::convolution(r, r), polynomial(std::vector<int>(std::begin(coef), std::begin(coef) + k)));
            nr.resize(k);
            for(int i = 0; i < k / 2; ++i) {
                nr[i] = (2 * r[i] - nr[i] + mod) % mod;
                nr[i + k / 2] = (mod - nr[i + k / 2]) % mod;
            }
            r = std::move(nr);
        }
        return polynomial(r);
    }
    std::enable_if_t<std::is_same<VT, int>::value, polynomial> sqrt() {
        using fmt = FMT<polynomial>;
        const int inv2 = (mod + 1) / 2;
        {
            int sz = 1;
            while(sz < degree()) sz *= 2;
            coef.resize(sz);
        }
        const int n = degree();
        polynomial s(1); s[0] = 1;
        for(int k = 2; k <= n; k <<= 1) {
            s.resize(k);
            auto ns = fmt::convolution(s.inverse(), polynomial(std::vector<int>(std::begin(coef), std::begin(coef) + k)));
            ns.resize(k);
            for(int i = 0; i < k; ++i) {
                s[i] = 1LL * (s[i] + ns[i]) * inv2 % mod;
            }
        }
        return s;
    }

    VT& operator[](int i) {
        assert(i < degree());
        return coef[i];
    }
    const VT operator[](int i) const { return i >= degree() ? VT() : coef[i]; }

    void canonicalize() {
        int i = static_cast<int>(coef.size());
        while(i > 0 && coef[i - 1] == VT()) --i;
        coef.resize(i);
    }

    // evaluate coef[n - 1] * x^[n - 1] + ... + coef[0]
    VT eval(VT x) const {
        const int d = degree();
        VT res = 0;
        for(int i = d - 1; i >= 0; --i) {
            res = res * x + coef[i];
        }
        return res;
    }

private:
    // coef[n - 1] * x^{n - 1} + ... + coef[1] * x + coef[0]
    std::vector<VT> coef;
};
