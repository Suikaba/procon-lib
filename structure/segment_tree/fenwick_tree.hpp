template <typename T>
class fenwick_tree {
public:
    fenwick_tree(int n_) : n(n_), dat(n, 0) {}

    void add(int i, T value) {
        for(; i < n; i |= i + 1) {
            dat[i] += value;
        }
    }

    T sum(int i) const {
        T res = 0;
        for(; i >= 0; i = (i & (i + 1)) - 1) {
            res += dat[i];
        }
        return res;
    }
    // [l, r)
    T sum(int l, int r) const {
        return sum(r - 1) - sum(l - 1);
    }

private:
    const int n;
    std::vector<T> dat;
};
