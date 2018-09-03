// Range Update and Range Min (update val >= 0)
struct RURM {
    using type1 = int; // data type (Monoid)
    using type2 = int; // lazy

    static type1 id1() {
        return std::numeric_limits<type1>::max();
    }
    static type2 id2() {
        return -1;
    }
    static type1 op1(type1 const& l, type1 const& r) {
        return std::min(l, r);
    }
    static type1 op2(type1 const& l, type2 const& r) {
        return r != id2() ? r : l;
    }
    static type2 op3(type2 const& l, type2 const& r) {
        return r != id2() ? r : l;
    }
};

// Range Update Range Sum
struct RURS {
    struct type1 {
        int val, size;
        type1(int v, int s) : val(v), size(s) {}
    };
    using type2 = int;

    static type1 id1() { return type1(0, 0); } // initialize: seg<RURS>(n, type1(0, 1))
    static type2 id2() { return std::numeric_limits<type2>::min() / 2; }

    static type1 op1(type1 const& l, type1 const& r) {
        return type1(l.val + r.val, l.size + r.size);
    }
    static type1 op2(type1 const& l, type2 const& r) {
        return r != id2() ? type1(r * l.size, l.size) : l;
    }
    static type2 op3(type2 const& l, type2 const& r) {
        return r != id2() ? r : l;
    }
};


template <typename Monoid>
class lazy_segment_tree {
    using T1 = typename Monoid::type1;
    using T2 = typename Monoid::type2;

public:
    lazy_segment_tree(std::vector<T1> const& init)
        : sz(init.size()), n(expand(init.size())), h(std::ceil(std::log2(init.size()))),
          data(n * 2, Monoid::id1()), lazy(n * 2, Monoid::id2())
    {
        std::copy(std::begin(init), std::end(init), std::begin(data) + n);
        for(int i = n - 1; i >= 1; --i) {
            data[i] = Monoid::op1(data[i * 2], data[i * 2 + 1]);
        }
    }
    lazy_segment_tree(int n_, T1 init = Monoid::id1())
        : lazy_segment_tree(std::vector<T1>(n_, init))
    {}

    T1 query(int l, int r) {
        l += n, r += n;
        for(int i = h; i > 0; --i) {
            push(l >> i), push(r >> i);
        }
        T1 res1 = Monoid::id1(), res2 = Monoid::id1();
        while(l < r) {
            if(l & 1) res1 = Monoid::op1(res1, Monoid::op2(data[l], lazy[l])), ++l;
            if(r & 1) --r, res2 = Monoid::op1(res2, Monoid::op2(data[r], lazy[r]));
            l /= 2, r /= 2;
        }
        return Monoid::op1(res1, res2);
    }

    void update(int l, int r, T2 val) {
        assert(0 <= l && l <= r && r <= sz);
        l += n, r += n;
        for(int i = h; i > 0; --i) {
            push(l >> i), push(r >> i);
        }
        int tl = l, tr = r;
        while(l < r) {
            if(l & 1) lazy[l] = Monoid::op3(lazy[l], val), ++l;
            if(r & 1) --r, lazy[r] = Monoid::op3(lazy[r], val);
            l /= 2, r /= 2;
        }
        while(tr /= 2, tl /= 2) {
            if(lazy[tl] == Monoid::id2()) update(tl);
            if(lazy[tr] == Monoid::id2()) update(tr);
        }
    }

private:
    int expand(int n_) const {
        int res = 1;
        while(res < n_) res <<= 1;
        return res;
    }

    void push(int node) {
        if(lazy[node] == Monoid::id2()) return;
        if(node < n) {
            lazy[node * 2] = Monoid::op3(lazy[node * 2], lazy[node]);
            lazy[node * 2 + 1] = Monoid::op3(lazy[node * 2 + 1], lazy[node]);
        }
        data[node] = Monoid::op2(data[node], lazy[node]);
        lazy[node] = Monoid::id2();
    }

    void update(int node) {
        if(node * 2 + 1 >= (int)data.size()) return; // for n == max_r
        data[node] = Monoid::op1(Monoid::op2(data[node * 2], lazy[node * 2]),
                                 Monoid::op2(data[node * 2 + 1], lazy[node * 2 + 1]));
    }

private:
    int const sz, n, h;
    std::vector<T1> data;
    std::vector<T2> lazy;
};
