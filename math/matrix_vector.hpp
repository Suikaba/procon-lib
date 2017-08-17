
template<typename T>
using matrix = std::vector<std::vector<T>>;

template<typename T>
matrix<T> make_matrix(int n, int m, T id = 0) {
    return matrix<T>(n, std::vector<T>(m, id));
}

template<typename T>
matrix<T> operator-(matrix<T> m) {
    for(int i=0; i<m.size(); ++i) {
        for(int j=0; j<m[i].size(); ++j) {
            m[i][j] = -m[i][j];
        }
    }
    return m;
}

template<typename T>
matrix<T> operator+(matrix<T> lhs, matrix<T> const& rhs) {
    assert(lhs.size() == rhs.size() && lhs[0].size() == rhs[0].size());
    for(int i=0; i<lhs.size(); ++i) {
        for(int j=0; j<lhs[i].size(); ++j) {
            lhs[i][j] += rhs[i][j];
        }
    }
    return lhs;
}

template<typename T>
matrix<T> operator-(matrix<T> lhs, matrix<T> const& rhs) {
    assert(lhs.size() == rhs.size() && lhs[0].size() == rhs[0].size());
    for(int i=0; i<lhs.size(); ++i) {
        for(int j=0; j<lhs[i].size(); ++j) {
            lhs[i][j] -= rhs[i][j];
        }
    }
    return lhs;
}

template<typename T>
matrix<T> operator*(T lhs, matrix<T> const& rhs) {
    auto res = rhs;
    for(int i=0; i<res.size(); ++i) {
        for(int j=0; j<res[i].size(); ++j) {
            res[i][j] *= lhs;
        }
    }
    return res;
}

template<typename T>
matrix<T> operator*(matrix<T> a, std::vector<T> const& b) {
    const int n = a.size();
    const int m = b.size();
    std::vector<T> res(n);
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            res[i] += a[i][j] * b[j];
        }
    }
    return res;
}

template<typename T>
matrix<T> operator*(matrix<T> const& lhs, matrix<T> const& rhs) {
    if(lhs.size() == 1 && lhs[0].size() == 1) {
        return lhs[0][0] * rhs;
    }
    if(rhs.size() == 1 && rhs[0].size() == 1) {
        return rhs[0][0] * lhs;
    }
    assert(lhs[0].size() == rhs.size());
    matrix<T> res(lhs.size(), std::vector<T>(rhs[0].size()));
    for(int i=0; i<(int)lhs.size(); ++i) {
        for(int k=0; k<(int)rhs.size(); ++k) {
            for(int j=0; j<(int)rhs[0].size(); ++j) {
                res[i][j] += lhs[i][k] * rhs[k][j];
            }
        }
    }
    return res;
}

template <typename T>
matrix<T> modmul(matrix<T> const& lhs, matrix<T> const& rhs, long long const M) {
    matrix<T> res(lhs.size(), std::vector<T>(rhs[0].size()));
    for(int i = 0; i < (int)lhs.size(); ++i) {
        for(int k = 0; k < (int)rhs.size(); ++k) {
            for(int j = 0; j < (int)rhs[0].size(); ++j) {
                res[i][j] += lhs[i][k] * rhs[k][j] % M;
                res[i][j] %= M;
            }
        }
    }
    return res;
}

template <typename T>
std::vector<T> modmul(matrix<T> const& a, std::vector<T> const& b, long long const M) {
    const int n = a.size();
    const int m = b.size();
    std::vector<T> res(n);
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            res[i] = res[i] + (a[i][j] * b[j]) % M;
            res[i] %= M;
        }
    }
    return res;
}

template <typename T>
matrix<T> transpose(matrix<T> const& m) {
    const int R = m.size(), C = m[0].size();
    matrix<T> res(C, std::vector<T>(R));
    for(int i=0; i<C; ++i) {
        for(int j=0; j<R; ++j) {
            res[i][j] = m[j][i];
        }
    }
    return res;
}

template<typename T>
matrix<T> eye(int n) {
    matrix<T> res(n, std::vector<T>(n));
    for(int i = 0; i < n; ++i) {
        res[i][i] = 1;
    }
    return res;
}

template <typename T>
matrix<T> modpow(matrix<T> x, long long n, long long M) {
    auto res = eye<T>(x.size());
    while(n > 0) {
        if(n & 1) {
            res = modmul(res, x, M);
        }
        x = modmul(x, x, M);
        n >>= 1;
    }
    return res;
}

template<typename T>
void print(matrix<T> const& m) {
    for(int i=0; i<m.size(); ++i) {
        for(int j=0; j<m[i].size(); ++j) {
            if(j != 0) {
                cout << ' ';
            }
            cout << (T)m[i][j];
        }
        cout << endl;
    }
}

