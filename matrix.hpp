#include <cassert>

namespace detail {

bool is_zero(double d) {
    return (std::abs(d) < eps);
}
bool is_zero(long double d) {
    return (std::abs(d) < eps);
}

}

template <typename T>
class matrix {
public:
    matrix() = delete;
    matrix(int r, int c) 
        : v_(r, std::vector<T>(c)),
          row_(r),
          column_(c)
    {}
    matrix(matrix const&) = default;
    matrix(matrix&&) = default;
    matrix& operator=(matrix const&) = default;
    matrix& operator=(matrix&&) = default;
    ~matrix() = default;

    std::vector<T>& operator[](unsigned int i) {
        return v_[i];
    }
    const std::vector<T>& operator[](unsigned int i) const {
        return v_[i];
    }
    matrix& operator+=(matrix const& other) {
        assert("matrix operator+: row or column size does not match" && other.row_size() == row_ && other.column_size() == column_);

        for(int i=0; i<row_; ++i) {
            for(int j=0; j<column_; ++j) {
                v_[i][j] += other[i][j];
            }
        }
        return *this;
    }
    matrix& operator-=(matrix const& other) {
        assert("matrix operator-: row or column size does not match" && other.row_size() == row_ && other.column_size() == column_);

        for(int i=0; i<row_; ++i) {
            for(int j=0; j<column_; ++j) {
                v_[i][j] -= other[i][j];
            }
        }
        return *this;
    }

    int row_size() const {
        return row_;
    }
    int column_size() const {
        return column_;
    }

    // for debug
    void print() {
        for(int i=0; i<row_; ++i) {
            for(int j=0; j<column_; ++j) {
                std::cout << v_[i][j];
                if(j != column_ - 1) {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<std::vector<T>> v_;
    int row_, column_;
};

template <typename T>
matrix<T> operator-(matrix<T> m) {
    for(int i=0; i<m.row_size(); ++i) {
        for(int j=0; j<m.column_size(); ++j) {
            m[i][j] = -m[i][j];
        }
    }
}

template <typename T>
matrix<T> operator+(matrix<T> lhs, matrix<T> const& rhs) {
    lhs += rhs;
    return lhs;
}

template <typename T>
matrix<T> operator-(matrix<T> lhs, matrix<T> const& rhs) {
    lhs -= rhs;
    return lhs;
}

template <typename T>
matrix <T> operator*(matrix<T> const& lhs, matrix<T> const& rhs) {
    assert("matrix operator*: lhs.column_size() != rhs.row_size()" && lhs.column_size() == rhs.row_size());

    matrix<T> ret(lhs.row_size(), rhs.column_size());
    for(int i=0; i<ret.row_size(); ++i) {
        for(int j=0; j<ret.column_size(); ++j) {
            for(int k=0; k<lhs.column_size(); ++k) {
                ret[i][j] = std::fma(lhs[i][k], rhs[k][j], ret[i][j]);
            }
        }
    }
    return ret;
}

template <typename T>
matrix<T> transpose(matrix<T> const& m) {
    const int R = m.row_size(), C = m.column_size();
    matrix<T> ret(C, R);
    for(int i=0; i<C; ++i) {
        for(int j=0; j<R; ++j) {
            ret[i][j] = m[j][i];
        }
    }
    return ret;
}

template <typename T>
int rank_matrix(matrix<T> a) {
    const int r = a.row_size(), c = a.column_size();
    matrix<long double> b(r, c);
    for(int i=0; i<r; ++i) {
        for(int j=0; j<c; ++j) {
            b[i][j] = static_cast<long double>(a[i][j]);
        }
    }
    return rank_matrix<long double>(std::move(b));
}

template <>
int rank_matrix(matrix<long double> a) {
    const int R = a.row_size(), C = a.column_size();
    int r = 0;
    for(int i=0; i<C && r < R; ++i) {
        int pivot = r;
        for(int j=r+1; j<R; ++j) {
            if(std::abs(a[j][i]) > std::abs(a[pivot][i])) {
                pivot = j;
            }
        }
        std::swap(a[pivot], a[r]);
        if(detail::is_zero(a[r][i])) {
            continue;
        }
        for(int k=C-1; k>=i; --k) {
            a[r][k] = a[r][k] / a[r][i];
        }
        for(int j=r+1; j<R; ++j) {
            for(int k=C-1; k>=i; --k) {
                a[j][k] = std::fma(-a[r][k], a[j][i], a[j][k]);
            }
        }
        ++r;
    }
    return r;
}

// todo: only long double?
template <typename T>
T det(matrix<T> a) {
    const int R = a.row_size(), C = a.column_size();
    matrix<long double> b(R, C);
    for(int i=0; i<R; ++i) {
        for(int j=0; j<C; ++j) {
            b[i][j] = static_cast<long double>(a[i][j]);
        }
    }
    return static_cast<T>(det<long double>(std::move(b)));
}
template <>
long double det(matrix<long double> a) {
    assert("det(): matrix is not square." && a.column_size() == a.row_size());
    const int R = a.row_size(), C = a.column_size();
    long double ret = 1.0;
    for(int i=0; i<R; ++i) {
        int pivot = i;
        for(int j=i+1; j<R; ++j) {
            if(std::abs(a[j][i]) > std::abs(a[pivot][i])) {
                pivot = j;
            }
        }
        std::swap(a[pivot], a[i]);
        ret *= a[i][i] * (i != pivot ? -1.0 : 1.0);
        if(detail::is_zero(a[i][i])) {
            break;
        }
        for(int j=i+1; j<R; ++j) {
            for(int k=C-1; k>=i; --k) {
                a[j][k] -= a[i][k] * a[j][i] / a[i][i];
            }
        }
    }
    return ret;
}

template <typename T>
matrix<T> make_identity_matrix(int n) {
    matrix<T> ret(n, n);
    for(int i=0; i<n; ++i) {
        ret[i][i] = 1;
    }
    return ret;
}

// mod ver
template <typename T>
matrix<T> mul(matrix<T> const& a, matrix<T> const& b, long long M = mod) {
    int R = a.row_size(), C = b.column_size();
    assert("matrix mul: row and column size does not match" && a.column_size() == b.row_size());
    matrix<T> ret(R, C);
    for(int i=0; i<R; ++i) {
        for(int j=0; j<C; ++j) {
            for(int k=0; k<a.column_size(); ++k) {
                ret[i][j] = (ret[i][j] + ((a[i][k] * b[k][j]) % M)) % M;
            }
        }
    }
    return ret;
}

template <typename T>
matrix<T> pow(matrix<T> x, long long y) {
    assert("matrix pow: matrix is not square" && x.column_size() == x.row_size());
    matrix<T> ret = make_identity_matrix<T>(x.column_size());
    while(y > 0) {
        if(y & 1) {
            ret = ret * x;
        }
        x = x * x;
        y >>= 1;
    }
    return ret;
}

template <typename T>
matrix<T> pow(matrix<T> x, long long y, long long M) {
    assert("matrix pow: matrix is not square" && x.column_size() == x.row_size());
    matrix<T> ret = make_identity_matrix<T>(x.column_size());
    while(y > 0) {
        if(y & 1) {
            ret = mul(ret, x, M);
        }
        x = mul(x, x, M);
        y >>= 1;
    }
    return ret;
}



