template <typename T>
class range_add_range_sum {
public:
    range_add_range_sum(int n_) : n(n_), bit0(n), bit1(n) {}

    void add(int l, int r, T val) {
        bit0.add(l, -val * l);
        bit1.add(l, val);
        bit0.add(r, val * r);
        bit1.add(r, -val);
    }

    T sum(int p) const {
        return bit1.sum(p) * (p + 1) + bit0.sum(p);
    }
    T sum(int l, int r) const {
        return sum(r - 1) - sum(l - 1);
    }

private:
    const int n;
    fenwick_tree<T> bit0, bit1;
};

