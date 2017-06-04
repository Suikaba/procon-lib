
//struct rmq { // min
//    using type = int;
//    static bool comp(type const& l, type const& r) {
//        return l < r;
//    }
//};

template<typename Policy>
class rmq_sparse_table {
    using T = typename Policy::type;

public:
    rmq_sparse_table(std::vector<T> const& v)
        : a(v), n(v.size()), log_n(v.size()+1)
    {
        for(int i=2; i<n+1; ++i) {
            log_n[i] = log_n[i >> 1] + 1;
        }

        table = std::vector<std::vector<int>>(n, std::vector<T>(log_n[n] + 1));
        for(int i=0; i<n; ++i) {
            table[i][0] = i;
        }
        for(int k=1; (1<<k) <= n; ++k) {
            for(int i=0; i + (1 << k) <= n; ++i) {
                int first = table[i][k-1],
                    second = table[i + (1 << (k-1))][k-1];
                if(Policy::comp(a[first], a[second])) {
                    table[i][k] = first;
                } else {
                    table[i][k] = second;
                }
            }
        }
    }

    // [l..r]
    int query(int l, int r) {
        int k = log_n[r - l + 1];
        if(Policy::comp(a[table[l][k]], a[table[r - (1 << k) + 1][k]])) {
            return table[l][k];
        } else {
            return table[r - (1 << k) + 1][k];
        }
    }

private:
    const int n;
    std::vector<int> log_n;
    std::vector<T> a;
    std::vector<std::vector<int>> table;
};

// usage: rmq_sparse_table<rmq> st(A);

