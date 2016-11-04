#include <cassert>

namespace detail {

bool is_zero(double d) {
    return (std::abs(d) < 1e-9);
}
bool is_zero(long double d) {
    return (std::abs(d) < 1e-9);
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
    matrix(std::vector<std::vector<T>> const& v)
        : v_(v),
          row_(v.size()),
          column_((v.size() == 0 ? 0 : v[0].size()))
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

    bool operator==(matrix const& rhs) const {
        if(row_ != rhs.row_size() || column_ != rhs.column_size()) {
            return false;
        }
        for(int i=0; i<row_; ++i) {
            for(int j=0; j<column_; ++j) {
                if(std::abs(v_[i][j] - rhs[i][j]) > 1e-9) {
                    return false;
                }
            }
        }
        return true;
    }
    bool operator!=(matrix const& rhs) {
        return !(*this == rhs);
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
        for(int k=0; k<lhs.column_size(); ++k) {
            for(int j=0; j<ret.column_size(); ++j) {
                ret[i][j] += lhs[i][k] * rhs[k][j];
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

// verified
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
                a[j][k] += -a[r][k] * a[j][i];
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
matrix<T> eye(int n) {
    matrix<T> res(n, n);
    for(int i=0; i<n; ++i) {
        res[i][i] = 1;
    }
    return res;
}

// mulptile mod ver
template <typename T>
matrix<T> mul(matrix<T> const& a, matrix<T> const& b, long long M) {
    int R = a.row_size(), C = b.column_size();
    assert("matrix mul: row and column size does not match" && a.column_size() == b.row_size());
    matrix<T> ret(R, C);
    for(int i=0; i<R; ++i) {
        for(int k=0; k<a.column_size(); ++k) {
            for(int j=0; j<C; ++j) {
                ret[i][j] = (ret[i][j] + a[i][k] * b[k][j]) % M;
            }
        }
    }
    return ret;
}

template <typename T>
matrix<T> pow(matrix<T> x, long long y) {
    assert("matrix pow: matrix is not square" && x.column_size() == x.row_size());
    matrix<T> ret = eye<T>(x.column_size());
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
matrix<T> modpow(matrix<T> x, long long y, long long M) {
    assert("matrix pow: matrix is not square" && x.column_size() == x.row_size());
    matrix<T> ret = eye<T>(x.column_size());
    while(y > 0) {
        if(y & 1) {
            ret = mul(ret, x, M);
        }
        x = mul(x, x, M);
        y >>= 1;
    }
    return ret;
}



// LUP factorization(gauss)
// verified
// Ax = b
// note: A must be regular(non-singular)
// return: x
//         or size 0 vector (if x does not exist or not unique)
vector<double> gauss_jordan(matrix<double>& A, std::vector<double> const& b) {
    int n = A.row_size();
    matrix<double> B(n, n+1);
    for(int i=0; i<n; ++i) {
        for(int j=0; j<n; ++j) {
            B[i][j] = A[i][j];
        }
    }
    for(int i=0; i<n; ++i) {
        B[i][n] = b[i];
    }

    for(int i=0; i<n; ++i) {
        int pivot = i;
        for(int j=i; j<n; ++j) {
            if(std::abs(B[j][i]) > std::abs(B[pivot][i])) {
                pivot = j;
            }
        }
        std::swap(B[i], B[pivot]);
        if(std::abs(B[i][i]) < 1e-9) {
            return std::vector<double>();
        }

        for(int j=i+1; j<=n; ++j) {
            B[i][j] /= B[i][i];
        }
        for(int j=0; j<n; ++j) {
            if(i != j) {
                for(int k=i+1; k<=n; ++k) {
                    B[j][k] -= B[j][i] * B[i][k];
                }
            }
        }
    }

    std::vector<double> x(n);
    for(int i=0; i<n; ++i) {
        x[i] = B[i][n];
    }
    return x;
}


struct lu_data {
    matrix<double> A;
    std::vector<int> pi;
};

// LU decomposition 
// A = LU
// A: n * n
// Verified
lu_data lu_decomposition(matrix<double> A) {
    std::vector<int> pi;
    const int n = A.row_size();
    for(int i=0; i<n; ++i) {
        int pivot = i;
        for(int j=i+1; j<n; ++j) {
            if(std::fabs(A[pivot][i]) < std::fabs(A[j][i])) {
                pivot = j;
            }
        }
        pi.push_back(pivot);
        std::swap(A[pivot], A[i]);
        for(int j=i+1; j<n; ++j) {
            for(int k=i+1; k<n; ++k) {
                A[j][k] -= A[i][k] * A[j][i] / A[i][i];
            }
            A[j][i] /= A[i][i];
        }
    }
    return lu_data{A, pi};
}

// solve Ax = LUx = b
std::vector<double> lu_solve(lu_data LU, std::vector<double> b) {
    matrix<double>& A = LU.A;
    std::vector<int> &pi = LU.pi;
    const int n = A.row_size();
    for(int i=0; i<(int)pi.size(); ++i) {
        std::swap(b[i], b[pi[i]]);
    }
    // set c = Ux
    // solve Lc = b
    // (assign: b[i] <- c[i])
    for(int i=0; i<n; ++i) {
        for(int j=0; j<i; ++j) {
            b[i] -= A[i][j] * b[j];
        }
    }
    // solve Ux = c
    for(int i=n-1; i>=0; --i) {
        for(int j=i+1; j<n; ++j) {
            b[i] -= A[i][j] * b[j];
        }
        b[i] /= A[i][i];
    }
    return b;
}
