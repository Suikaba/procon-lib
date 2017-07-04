
// example:
//struct rmq {
//    using type = int;
//    static type id() {
//        return std::numeric_limits<int>::max();
//    }
//    static type op(type const& l, type const& r) {
//        return std::min(l, r);
//    }
//};

template<typename Monoid>
class segment_tree {
    using T = typename Monoid::type;

public:
    segment_tree(std::vector<T> const& init)
        : sz(init.size()),
          n(expand(init.size()))
    {
        dat.assign(n*2, Monoid::id());
        std::copy(begin(init), end(init), begin(dat) + n);
        for(int i = n - 1; i >= 0; --i) {
            dat[i] = Monoid::op(dat[i * 2], dat[i * 2 + 1]);
        }
    }

    segment_tree(int const n, T const& init = Monoid::id())
        : segment_tree(std::vector<T>(n, init))
    {}

    void update(int p, T val) {
        assert(0 <= p && p < size);
        dat[p += n] = val;
        while(p /= 2) {
            dat[p] = Monoid::op(dat[p * 2], dat[p * 2 + 1]);
        }
    }

    // [l, r)
    T query(int l, int r) const {
        assert(0 <= l && l < r && r <= size);
        l += n;
        r += n;
        T res1 = Monoid::id(),
          res2 = Monoid::id();
        while(l != r) {
            if(l & 1) {
                res1 = Monoid::op(res1, dat[l++]);
            }
            if(r & 1) {
                res2 = Monoid::op(dat[--r], res2);
            }
            l /= 2;
            r /= 2;
        }
        return Monoid::op(res1, res2);
    }

    int size() const {
        return sz;
    }

private:
    int expand(int n) const {
        return n == 1 ? n : expand((n + 1) / 2) * 2;
    }

private:
    const int sz;
    const int n;
    std::vector<T> dat;
};
