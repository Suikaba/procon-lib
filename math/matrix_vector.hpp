
template<typename T>
using matrix = std::vector<std::vector<T>>;

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
matrix<T> operator*(matrix<T> const& lhs, matrix<T> const& rhs) {
    if(lhs.size() == 1 && lhs[0].size() == 1) {
        return lhs[0][0] * rhs;
    }
    if(rhs.size() == 1 && rhs[0].size() == 1) {
        return rhs[0][0] * lhs;
    }
    assert(lhs[0].size() == rhs.size());
    matrix<T> res(lhs.size(), vector<T>(rhs[0].size()));
    for(int i=0; i<lhs.size(); ++i) {
        for(int j=0; j<rhs[0].size(); ++j) {
            for(int k=0; k<rhs.size(); ++k) {
                res[i][j] += lhs[i][k] * rhs[k][j];
            }
        }
    }
    return res;
}

template <typename T>
matrix<T> transpose(matrix<T> const& m) {
    const int R = m.size(), C = m[0].size();
    matrix<T> res(C, vector<T>(R));
    for(int i=0; i<C; ++i) {
        for(int j=0; j<R; ++j) {
            res[i][j] = m[j][i];
        }
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
            cout << (int)m[i][j];
        }
        cout << endl;
    }
}

